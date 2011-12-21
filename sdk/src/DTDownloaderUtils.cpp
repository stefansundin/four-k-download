/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/


#include <openmedia/DTHeaders.h>

#include "DTDownloaderUtils.h"
#include <openmedia/DTString.h>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <urdl/istream.hpp>
#include <urdl/http.hpp>

#include <iostream>
#include <string>
#include <algorithm>

namespace openmedia { namespace downloader {

const ::std::string userAgent = "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)";

void parse_http_headers(const std::string &HttpHeaderString, std::vector<HttpHeader> &Result)
{
    std::vector<HttpHeader> resultVal;
    boost::regex re("^([^:]+):\\s+(.+?)$", boost::regex_constants::match_not_dot_newline);
    int index[] = {1,2};
    boost::sregex_token_iterator i(HttpHeaderString.begin(), HttpHeaderString.end(), re, index);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        const std::string name = (*i++).str();
        const std::string value = (*i++).str();
        resultVal.push_back( HttpHeader( name, value) );
    }
    Result.swap(resultVal);
}

bool is_media_content(const std::vector<HttpHeader> &HttpHeadersArray, std::string & MediaType, boost::uint64_t & ContentSize)
{
    ContentSize = (std::numeric_limits<boost::uint64_t>::max)();
    MediaType = "";

    for (size_t i = 0; i < HttpHeadersArray.size(); ++i)
    {
        if ("Content-Type" == HttpHeadersArray[i].Name)
        {
            if (boost::algorithm::contains(HttpHeadersArray[i].Value, "video"))
            {
                MediaType = HttpHeadersArray[i].Value;
            }
        }

        if ("Content-Length" == HttpHeadersArray[i].Name)
        {
            ContentSize = boost::lexical_cast<boost::uint64_t>(HttpHeadersArray[i].Value); 
        }
    }
    return MediaType.length() > 0;    
}

std::string extract_cookie(const std::string & CookieContent)
{
    boost::regex re("\\s*([^=]+)\\s*=\\s*([^;]*);");
    int index[] = {1,2};
    boost::sregex_token_iterator i(CookieContent.begin(), CookieContent.end(), re, index);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        const std::string name = (*i++).str();
        const std::string value = (*i++).str();
        return name + "=" + value;
    }
    return "";
}

std::string use_cookies(const std::vector<HttpHeader> &HttpHeadersArray)
{
    std::string cookies = "";
    bool first = true;
    for (size_t i = 0; i < HttpHeadersArray.size(); ++i)
    {
        if ("Set-Cookie" == HttpHeadersArray[i].Name)
        {
            if (!first) cookies += "; ";
            first = false;
            cookies += extract_cookie(HttpHeadersArray[i].Value);
        }
    }
    return cookies;
}

bool extract_content_range(std::string RangeStr, HttpContentRange & Range)
{
    boost::regex re("\\s*bytes\\s*([\\d]+)-([\\d]+)/([\\d]+)");

    int index[] = {1,2,3};
    boost::sregex_token_iterator i(RangeStr.begin(), RangeStr.end(), re, index);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        const std::string start = (*i++).str();
        const std::string end = (*i++).str();
        const std::string size = (*i++).str();
        try 
        {
            const size_t startVal = boost::lexical_cast<size_t>(start);
            const size_t endVal = boost::lexical_cast<size_t>(end);
            const size_t sizeVal = boost::lexical_cast<size_t>(size);
            Range = HttpContentRange(startVal, endVal, sizeVal);
            return true;
        }
        catch(...)
        {
            return false;
        }
    }
    return false;
}

bool get_content_range(const std::vector<HttpHeader> &HttpHeadersArray, HttpContentRange & Range)
{
    for (size_t i = 0; i < HttpHeadersArray.size(); ++i)
    {
        if ("Content-Range" == HttpHeadersArray[i].Name)
        {
            return extract_content_range(HttpHeadersArray[i].Value, Range);
        }
    }
    return false;
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

std::string url_encode(const std::string & url)
{
    char encode_buf[4];
    std::string result;

    encode_buf[0] = '%';
    result.reserve(url.size());


    for (size_t pos = 0; pos < url.size(); ++pos) 
    {
        switch(url[pos]) 
        {
            default:
                if (url[pos] >= 32 && url[pos] < 127) 
                {
                    result += url[pos];
                    break;
                }

            case '$': case '&': case '+': case ',': case '/': case ':':
            case ';': case '=': case '?': case '@': case '"': case '<':
            case '>': case '#': case '%': case '{': case '}': case '|':
            case '\\': case '^': case '~': case '[': case ']': case '`':
                sprintf(encode_buf+1, "%02X", url[pos]);
                result += encode_buf;
                break;
        }
    }
    return result;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

std::string url_decode(const std::string & str)
{
    char decode_buf[3];
    std::string result;
    result.reserve(str.size());

    for (size_t pos = 0; pos < str.size(); ++pos) 
    {
        switch(str[pos]) {
            case '+':
                result += ' ';
                break;
            case '%':
                if (pos + 2 < str.size()) 
                {
                    decode_buf[0] = str[++pos];
                    decode_buf[1] = str[++pos];
                    decode_buf[2] = '\0';
                    result += static_cast<char>( strtol(decode_buf, 0, 16) );
                } else
                {
                    result += '%';
                }
                break;
            default:
                // character does not need to be escaped
                result += str[pos];
        }
    };

    return result;
} 

bool download_page(const std::string & url, std::string & headers, std::string & content, const std::string & cookies, const std::string & connectionType)
{
    std::stringstream outStrm;

    urdl::istream is;
    is.set_option(urdl::http::connection_type(connectionType));
    is.set_option(urdl::http::user_agent(userAgent));

    urdl::http::additional_headers additionalHeaders;
    
    if (!cookies.empty())
    {
        additionalHeaders.get().push_back(
            urdl::http::additional_headers::header("Cookie", cookies)
            );

        is.set_option(additionalHeaders);
    }

    is.open(url);

    if (!is)
    {
        std::cerr << "[openmedia::downloader::download_page] unable to open URL: ";
        std::cerr << is.error().message() << std::endl;
        return false;
    }

    headers = is.headers();

    outStrm << is.rdbuf();
    content = outStrm.str();

    return true;
}

bool download_bytes(const std::string & url, std::string & headers, 
                    std::string & content, 
                    const std::string & cookies, 
                    const std::string & connectionType, size_t Bytes)
{
    std::stringstream outStrm;

    urdl::istream is;
    is.set_option(urdl::http::connection_type(connectionType));
    is.set_option(urdl::http::user_agent(userAgent));

    urdl::http::additional_headers additionalHeaders;

    if (cookies.length())
    {
        additionalHeaders.get().push_back(
            urdl::http::additional_headers::header("Cookie", cookies)
            );

        is.set_option(additionalHeaders);
    }

    is.open(url);

    if (!is)
    {
        std::cerr << "[openmedia::downloader::download_bytes] unable to open URL: ";
        std::cerr << is.error().message() << std::endl;
        return false;
    }

    headers = is.headers();
    size_t bytesWrited = 0;

    while (is && outStrm && bytesWrited < Bytes)
    {
        size_t writed;
        char buffer[1024] = "";
        size_t readed = (std::min)( sizeof(buffer), (size_t)(Bytes - bytesWrited) ); 
        is.read(buffer, readed);
        outStrm.write(buffer, writed = is.gcount());
        bytesWrited += writed;
    } 

    //outStrm << is.rdbuf();
    content = outStrm.str();

    return true;
}

bool download_page_header(const std::string & url, std::string & headers, const std::string & cookies, const std::string & connectionType)
{
    return download_page_header_ref(url, headers, cookies, "", connectionType);
}

bool download_page_header_ref(const std::string & url, std::string & headers, const std::string & cookies, const std::string & Referer, const std::string & connectionType)
{
    urdl::istream is;
    is.set_option(urdl::http::request_method("HEAD"));
    is.set_option(urdl::http::connection_type(connectionType));
    is.set_option(urdl::http::user_agent(userAgent));

    urdl::http::additional_headers additionalHeaders;
    
    if (cookies.length())
    {
        additionalHeaders.get().push_back(
            urdl::http::additional_headers::header("Cookie", cookies)
            );
    }

    if (Referer.length())
    {
        additionalHeaders.get().push_back(
            urdl::http::additional_headers::header("Referer", Referer)
            );
    }

    if (additionalHeaders.get().size())
    {
        is.set_option(additionalHeaders);
    }
    
    is.open(url);

    if (!is)
    {
        std::cerr << "[openmedia::downloader::download_page_header_ref] unable to open URL: ";
        std::cerr << is.error().message() << std::endl;
        return false;
    }

    headers = is.headers();
    return true;
}

void replace_xml_esc(std::string & Str)
{
    // &amp; (&), &lt; (<), &gt; (>), &apos; ('), &quot; (")    
    boost::algorithm::replace_all(Str, "&amp;", "&" );
    boost::algorithm::replace_all(Str, "&lt;", "<" );
    boost::algorithm::replace_all(Str, "&gt;", ">" );
    boost::algorithm::replace_all(Str, "&apos;", "'" );
    boost::algorithm::replace_all(Str, "&quot;", "\"" );
}

struct replace_fmt 
{
    std::string operator () (boost::smatch const & what)
    {
        try {
            std::string s = std::string("0x") + what[1].str();
            unsigned int x;   
            std::stringstream ss;
            ss << std::hex << s;
            ss >> x;
            std::wstring str = std::wstring(L"") + wchar_t(x);
            return  utf16_to_utf8(str);
            //return std::wstring("") + (wchar_t)(boost::lexical_cast<int>(s ));
        } catch(...)
        {
            return std::string("X");
        }
    }
};

struct replace_fmt_6 
{
    std::string operator () (boost::smatch const & what)
    {
        try {
            std::string s = std::string("0x") + what[1].str();
            unsigned int x;   
            std::stringstream ss;
            ss << std::hex << s;
            ss >> x;
            std::wstring str = std::wstring(L"") + wchar_t(x - 9472);
            return  utf16_to_utf8(str);
            //return std::wstring("") + (wchar_t)(boost::lexical_cast<int>(s ));
        } catch(...)
        {
            return std::string("X");
        }
    }
};

std::string replace_u_codes(const std::string & input)
{
    return boost::regex_replace(input, boost::regex( std::string("\\\\u([0-9a-zA-Z]+)") ), 
        replace_fmt(), boost::match_default | boost::format_all );
    //return input;
}

std::string replace_u_codes_6(const std::string & input)
{
    return boost::regex_replace(input, boost::regex( std::string("\\\\u([0-9a-zA-Z]{6})") ), 
        replace_fmt_6(), boost::match_default | boost::format_all );
    //return input;
}


} } //namespace openmedia::downloader
