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


/// \file   DTDownloaderUtils.cpp

#include "DTHeadersDownload.h"
#include "DTDownloaderUtils.h"

#include <iostream>
#include <string>
#include <algorithm>


#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <urdl/istream.hpp>
#include <urdl/http.hpp>

#include <openmedia/DTString.h>

using namespace std;
using namespace boost;

namespace openmedia { namespace downloader {

const string userAgent = "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)";

void parse_http_headers(const string &HttpHeaderString, vector<HttpHeader> &Result)
{
    vector<HttpHeader> resultVal;
    regex re("^([^:]+):\\s+(.+?)$", regex_constants::match_not_dot_newline);
    int index[] = {1,2};
    sregex_token_iterator i(HttpHeaderString.begin(), HttpHeaderString.end(), re, index);
    sregex_token_iterator j;
    while(i != j)
    {
        const string name = (*i++).str();
        const string value = (*i++).str();
        resultVal.push_back( HttpHeader( name, value) );
    }
    Result.swap(resultVal);
}

bool is_media_content(const vector<HttpHeader> &HttpHeadersArray, string & MediaType, uint64_t & ContentSize)
{
    ContentSize = (numeric_limits<uint64_t>::max)();
    MediaType = "";

    for (size_t i = 0; i < HttpHeadersArray.size(); ++i)
    {
        if ("Content-Type" == HttpHeadersArray[i].Name &&
            algorithm::contains(HttpHeadersArray[i].Value, "video"))
        {
            MediaType = HttpHeadersArray[i].Value;
        }

        if ("Content-Length" == HttpHeadersArray[i].Name)
            ContentSize = lexical_cast<uint64_t>(HttpHeadersArray[i].Value); 
    }
    return MediaType.length() > 0;    
}

string extract_cookie(const string & CookieContent)
{
    regex re("\\s*([^=]+)\\s*=\\s*([^;]*);");
    int index[] = {1,2};
    sregex_token_iterator i(CookieContent.begin(), CookieContent.end(), re, index);
    sregex_token_iterator j;
    if (i != j)
    {
        const string name = (*i++).str();
        const string value = (*i++).str();
        return name + "=" + value;
    }
    return "";
}

string use_cookies(const vector<HttpHeader> &HttpHeadersArray)
{
    string cookies = "";
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

bool extract_content_range(string RangeStr, HttpContentRange & Range)
{
    regex re("\\s*bytes\\s*([\\d]+)-([\\d]+)/([\\d]+)");

    int index[] = {1,2,3};
    sregex_token_iterator i(RangeStr.begin(), RangeStr.end(), re, index);
    sregex_token_iterator j;
    if (i != j)
    {
        const string start = (*i++).str();
        const string end = (*i++).str();
        const string size = (*i++).str();
        try 
        {
            const size_t startVal = lexical_cast<size_t>(start);
            const size_t endVal = lexical_cast<size_t>(end);
            const size_t sizeVal = lexical_cast<size_t>(size);
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

bool get_content_range(const vector<HttpHeader> &HttpHeadersArray, HttpContentRange & Range)
{
    for (size_t i = 0; i < HttpHeadersArray.size(); ++i)
    {
        if ("Content-Range" == HttpHeadersArray[i].Name)
            return extract_content_range(HttpHeadersArray[i].Value, Range);
    }
    return false;
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

string url_encode(const string & url)
{
    char encode_buf[4];
    string result;

    encode_buf[0] = '%';
    result.reserve(url.size());

    for (size_t pos = 0; pos < url.size(); ++pos) 
    {
        switch(url[pos]) 
        {
        default:
            if (url[pos] >= 32 && url[pos] < 127) 
            {
                // character does not need to be escaped
                result += url[pos];
                break;
            }
            // else pass through to next case

        case '$': case '&': case '+': case ',': case '/': case ':':
        case ';': case '=': case '?': case '@': case '"': case '<':
        case '>': case '#': case '%': case '{': case '}': case '|':
        case '\\': case '^': case '~': case '[': case ']': case '`': case ' ':
            // the character needs to be encoded
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

string url_decode(const string & str)
{
    char decode_buf[3];
    string result;
    result.reserve(str.size());

    for (size_t pos = 0; pos < str.size(); ++pos) 
    {
        switch(str[pos]) {
            case '+':
                // convert to space character
                result += ' ';
                break;
            case '%':
                // decode hexidecimal value
                if (pos + 2 < str.size()) 
                {
                    decode_buf[0] = str[++pos];
                    decode_buf[1] = str[++pos];
                    decode_buf[2] = '\0';
                    result += static_cast<char>( strtol(decode_buf, 0, 16) );
                } else
                {
                    // recover from error by not decoding character
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

bool download_page(const std::string & url,
                   std::string & headers,
                   std::string & content,

                   const std::vector<HttpHeader> & additionalHeaders)
{
    stringstream outStrm;

    urdl::istream is;
//    is.set_option(urdl::http::connection_type(connectionType));
    is.set_option(urdl::http::user_agent(userAgent));

    urdl::http::additional_headers additional_headers;

    BOOST_FOREACH(const HttpHeader & h, additionalHeaders)
    {
        additional_headers.get().push_back(
            urdl::http::additional_headers::header(h.Name, h.Value)
            );
    }
    
    if (!additionalHeaders.empty())
    {
        is.set_option(additional_headers);
    }

    is.open(url);

    if (!is)
    {
        return false;
    }

    headers = is.headers();

    outStrm << is.rdbuf();
    content = outStrm.str();

    return true;    
}

bool download_page(const string & url,
                   string & headers, string & content, const string & cookies, const string & connectionType)
{
    stringstream outStrm;

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
        //cerr << "[openmedia::downloader::download_page] unable to open URL: ";
        //cerr << is.error().message() << endl;
        return false;
    }

    headers = is.headers();

    outStrm << is.rdbuf();
    content = outStrm.str();

    return true;
}

bool download_bytes(const string & url, string & headers, 
                    string & content, 
                    const string & cookies, 
                    const string & connectionType, size_t Bytes)
{
    stringstream outStrm;

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
        //cerr << "[openmedia::downloader::download_bytes] unable to open URL: ";
        //cerr << is.error().message() << endl;
        return false;
    }

    headers = is.headers();
    size_t bytesWrited = 0;

    while (is && outStrm && bytesWrited < Bytes)
    {
        size_t writed;
        char buffer[1024] = "";
        size_t readed = (min)( sizeof(buffer), (size_t)(Bytes - bytesWrited) ); 
        is.read(buffer, readed);
        outStrm.write(buffer, writed = is.gcount());
        bytesWrited += writed;
    } 

    //outStrm << is.rdbuf();
    content = outStrm.str();

    return true;
}

bool download_page_header(const string & url, string & headers, const string & cookies, const string & connectionType)
{
    return download_page_header_ref(url, headers, cookies, "", connectionType);
}

bool download_page_header(const std::string & url,
                          std::string & headers,
                          const std::string & cookies,
                          const std::string & connectionType,
                          const std::vector<HttpHeader> & additionalHeaders)
{
    urdl::istream is;
    is.set_option(urdl::http::request_method("HEAD"));
    is.set_option(urdl::http::connection_type(connectionType));
    is.set_option(urdl::http::user_agent(userAgent));

    urdl::http::additional_headers additional_headers;
    
    if (cookies.length())
    {
        additional_headers.get().push_back(
            urdl::http::additional_headers::header("Cookie", cookies)
            );
    }

    BOOST_FOREACH(const HttpHeader & h, additionalHeaders)
    {
        if (!h.Value.empty())
        {
            additional_headers.get().push_back(
                urdl::http::additional_headers::header(h.Name, h.Value)
                );
        }
    }

    if (additional_headers.get().size())
    {
        is.set_option(additional_headers);
    }
    
    is.open(url);

    if (!is)
    {
        //cerr << "[openmedia::downloader::download_page_header_ref] unable to open URL: ";
        //cerr << is.error().message() << endl;
        return false;
    }

    headers = is.headers();
    return true;    
}

bool download_page_header_ref(const string & url, string & headers, const string & cookies, const string & Referer, const string & connectionType)
{
    std::vector<HttpHeader> additional_headers;
    additional_headers.push_back(HttpHeader("Referer",Referer));
    return download_page_header(url, headers, cookies, connectionType, additional_headers);    
}

void replace_xml_esc(string & Str)
{
    // &amp; (&), &lt; (<), &gt; (>), &apos; ('), &quot; (")    
    algorithm::replace_all(Str, "&amp;",    "&" );
    algorithm::replace_all(Str, "&lt;",     "<" );
    algorithm::replace_all(Str, "&gt;",     ">" );
    algorithm::replace_all(Str, "&apos;",   "'" );
    algorithm::replace_all(Str, "&quot;",   "\"" );
}

struct replace_fmt 
{
    string operator () (smatch const & what)
    {
        try 
        {
            string s = string("0x") + what[1].str();
            unsigned int x;   
            stringstream ss;
            ss << hex << s;
            ss >> x;
            wstring str = wstring(L"") + wchar_t(x);
            return  utf16_to_utf8(str);
            //return wstring("") + (wchar_t)(lexical_cast<int>(s ));
        } catch(...)
        {
            return string("X");
        }
    }
};

struct replace_fmt_6 
{
    string operator () (smatch const & what)
    {
        try {
            string s = string("0x") + what[1].str();
            unsigned int x;   
            stringstream ss;
            ss << hex << s;
            ss >> x;
            wstring str = wstring(L"") + wchar_t(x - 9472);
            return  utf16_to_utf8(str);
        }
        catch(...)
        {
            return string("X");
        }
    }
};

string replace_u_codes(const string & input)
{
    return regex_replace(input, regex( string("\\\\u([0-9a-zA-Z]+)") ), 
        replace_fmt(), match_default | format_all );
}

string replace_u_codes_6(const string & input)
{
    return regex_replace(input, regex( string("\\\\u([0-9a-zA-Z]{6})") ), 
        replace_fmt_6(), match_default | format_all );
}

string replace_u_codes_4(const string & input)
{
    return regex_replace(input, regex( string("\\\\u([0-9a-zA-Z]{4})") ), 
        replace_fmt(), match_default | format_all );
}

} } //namespace openmedia::downloader
