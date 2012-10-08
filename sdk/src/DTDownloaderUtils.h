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


#ifndef _DT_DOWNLOADERUTILS_H_
#define _DT_DOWNLOADERUTILS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <vector>
#include <boost/cstdint.hpp>

namespace openmedia { namespace downloader {

extern const ::std::string userAgent;

struct HttpHeader
{
    HttpHeader(const std::string & name, const std::string & value) : Name(name), Value(value)
    {}

    std::string Name;
    std::string Value;
};

/// Example:
/// Input:
/// "
/// Date: Wed, 30 Mar 2011 08:57:34 GMT
/// Server: wiseguy/0.6.10
/// "
/// Output:
/// [{"Date", "Wed, 30 Mar 2011 08:57:34 GMT"}, {"Server", "wiseguy/0.6.10"} ]
///
void parse_http_headers(const std::string &HttpHeaderString, std::vector<HttpHeader> &Result);

bool is_media_content(const std::vector<HttpHeader> &HttpHeadersArray, std::string & MediaType, boost::uint64_t & ContentSize);

/// Example:
/// input: "VISITOR_INFO1_LIVE=f-taRZ_8kj8; path=/; domain=.youtube.com; expires=Fri, 25-Nov-2011 08:57:34 GMT"
/// output: "VISITOR_INFO1_LIVE=f-taRZ_8kj8"
/// 
std::string extract_cookie(const std::string & CookieContent);

struct HttpContentRange
{
    HttpContentRange(size_t _First = 0, size_t _Last = 0, size_t _Size = 0) :
        First(_First), Last(_Last), Size(_Size)
    {}
    size_t First;
    size_t Last;
    size_t Size;
};

/// Example:
/// Input [ {"Content-Range", "bytes 21010-47021/47022"} ]
/// HttpContentRange = { First: 21010; Last: 47021; Size: 47022 }
bool get_content_range(const std::vector<HttpHeader> &HttpHeadersArray, HttpContentRange & Range);

/// Example: 
/// Input: [ {"Date", "Wed, 30 Mar 2011 08:57:34 GMT"},
///          {"Server", "wiseguy/0.6.10"}, 
///          {"Set-Cookie", "PREF=f1=50000000; path=/; domain=.youtube.com; expires=Sat, 27-Mar-2021 08:57:34 GMT"} 
///          {"Set-Cookie", "VISITOR_INFO1_LIVE=f-taRZ_8kj8; path=/; domain=.youtube.com; expires=Fri, 25-Nov-2011 08:57:34 GMT"} ]
///
/// Output: "PREF=f1=50000000; VISITOR_INFO1_LIVE=f-taRZ_8kj8"
///
std::string use_cookies(const std::vector<HttpHeader> &HttpHeadersArray);

/// see http://en.wikipedia.org/wiki/URL_encoding
std::string url_encode(const std::string & url);

/// see http://en.wikipedia.org/wiki/URL_encoding
std::string url_decode(const std::string & str);

bool download_page(const std::string & url, std::string & headers, std::string & content, const std::string & cookies, const std::string & connectionType);
bool download_page(const std::string & url, std::string & headers, std::string & content, const std::vector<HttpHeader> & additionalHeaders);
bool download_page_header(const std::string & url, std::string & headers, const std::string & cookies, const std::string & connectionType);
bool download_page_header_ref(const std::string & url, std::string & headers, const std::string & cookies, const std::string & Referer, const std::string & connectionType);


bool download_page_header(const std::string & url,
                          std::string & headers,
                          const std::string & cookies,
                          const std::string & connectionType,
                          const std::vector<HttpHeader> & additionalHeaders);

bool download_bytes(const std::string & url, std::string & headers, 
                    std::string & content, 
                    const std::string & cookies, 
                    const std::string & connectionType, size_t Bytes);

void replace_xml_esc(std::string & Str);

std::string replace_u_codes(const std::string & input);
std::string replace_u_codes_6(const std::string & input);
std::string replace_u_codes_4(const std::string & input);

} }

#endif // #ifndef _DT_DOWNLOADERUTILS_H_
