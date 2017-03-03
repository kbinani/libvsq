/**
 * @file CP932Converter.cpp
 * Copyright © 2014,2017 kbinani
 *
 * This file is part of libvsq.
 *
 * libvsq is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * libvsq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <libvsq/CP932Converter.hpp>
#if defined _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#else
#include <iconv.h>
#include <sys/errno.h>
#endif
#include <vector>
#include <memory>

LIBVSQ_BEGIN_NAMESPACE

namespace {
#if defined(_MSC_VER)
    std::string convert(std::string const& str, int from, int to)
    {
        int numChars = MultiByteToWideChar(from, // CodePage
                                           0, // dwFlags,
                                           str.c_str(), // lpMultiByteStr
                                           str.size(), // cbMultiByte
                                           nullptr, // lpWideCharStr
                                           0);// cchWideChar
        if (numChars == 0) {
            return std::string();
        }
        std::vector<wchar_t> buffer(numChars);
        numChars = MultiByteToWideChar(from, // CodePage
                                       0, // dwFlags,
                                       str.c_str(), // lpMultiByteStr
                                       str.size(), // cbMultiByte
                                       buffer.data(), // lpWideCharStr
                                       buffer.size());// cchWideChar
        if (numChars == 0) {
            return std::string();
        }
        numChars = WideCharToMultiByte(to, // CodePage
                                       0, // dwFlags
                                       buffer.data(), // lpWideCharStr
                                       buffer.size(), // cchWideChar
                                       nullptr, // lpMultiByteStr
                                       0, // cbMultiByte
                                       nullptr, // lpDefaultChar
                                       nullptr); // lpUsedDefaultChar
        if (numChars == 0) {
            return std::string();
        }
        std::vector<char> out(numChars);
        numChars = WideCharToMultiByte(to, // CodePage
                                       0, // dwFlags
                                       buffer.data(), // lpWideCharStr
                                       buffer.size(), // cchWideChar
                                       out.data(), // lpMultiByteStr
                                       out.size(), // cbMultiByte
                                       nullptr, // lpDefaultChar
                                       nullptr); // lpUsedDefaultChar
        if (numChars == 0) {
            return std::string();
        }
        return std::string(out.begin(), out.end());
    }
#elif defined(__APPLE__)
	std::string convert(std::string const& str, CFStringEncoding from, CFStringEncoding to)
	{
		CFStringRef cfstr = CFStringCreateWithBytes(kCFAllocatorDefault,
												  (UInt8 const*)str.c_str(),
												  str.size(),
												  from,
												  false); // isExternalRepresentation
		if (cfstr == nullptr) {
			return std::string();
		}
		
		CFRange range = CFRangeMake(0, CFStringGetLength(cfstr));
		CFIndex usedBufLen = 0;
		CFIndex numOkChars = CFStringGetBytes(cfstr,
											  range,
											  to,
											  0, // lossByte
											  false, // isExternalRepresentation
											  nullptr, // buffer
											  0, // maxBufLen
											  &usedBufLen);
		if (numOkChars <= 0) {
			CFRelease(cfstr);
			return std::string();
		}
		std::vector<char> buffer(usedBufLen);
		numOkChars = CFStringGetBytes(cfstr,
									  range,
									  to,
									  0, // lossByte
									  false, // isExternalRepresentation
									  (UInt8*)buffer.data(), // buffer
									  buffer.size(), // maxBufLen
									  nullptr);
		if (numOkChars <= 0) {
			CFRelease(cfstr);
			return std::string();
		}
		
		CFRelease(cfstr);
		return std::string(buffer.begin(), buffer.end());
	}
#else
    std::string convert(std::string const& str, char const* from, char const* to)
    {
        std::shared_ptr<void> ic(iconv_open(to, from), iconv_close);
        if (ic.get() == (iconv_t)-1) {
            return std::string();
        }
        std::vector<char> src(str.begin(), str.end());
        std::vector<char> result(str.size() + 1);
        while (true) {
            size_t inbytesleft = src.size() * sizeof(char);
            char* inbuf = src.data();
            size_t outbytesleft = result.size();
            char* outbuf = result.data();

            size_t ret = iconv(ic.get(),
                               &inbuf, &inbytesleft,
                               &outbuf, &outbytesleft);
            int ecode = errno;
            if (ret == (size_t)-1) {
                if (ecode == E2BIG) {
                    result.resize(result.size() * 2);
                    continue;
            } else {
                    return std::string();
                }
            } else {
                int count = ret;
                if (count == 0 && result.size() > outbytesleft) {
                    count = result.size() - outbytesleft;
                }
                return std::string(result.begin(), result.begin() + count);
            }
        }

        return std::string();
    }
#endif
}

std::string CP932Converter::convertFromUTF8(std::string const& utf8)
{
#if defined _MSC_VER
    return convert(utf8, CP_UTF8, 932);
#elif defined __APPLE__
	return convert(utf8, kCFStringEncodingUTF8, CFStringConvertWindowsCodepageToEncoding(932));
#else
    return convert(utf8, "UTF8", "CP932");
#endif
}


std::string CP932Converter::convertToUTF8(std::string const& cp932)
{
#if defined _MSC_VER
    return convert(cp932, 932, CP_UTF8);
#elif defined __APPLE__
	return convert(cp932, CFStringConvertWindowsCodepageToEncoding(932), kCFStringEncodingUTF8);
#else
    return convert(cp932, "CP932", "UTF8");
#endif
}

LIBVSQ_END_NAMESPACE
