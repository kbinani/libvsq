/**
 * @file FileUtil.cpp
 * Copyright © 2017 kbinani
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
#include <libvsq/FileUtil.hpp>
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif
#include <vector>

LIBVSQ_BEGIN_NAMESPACE

class ScopedLocale
{
public:
	explicit ScopedLocale(std::string const& locale)
		: hasValidOldLocale_(false)
	{
		char* loc = setlocale(LC_CTYPE, nullptr);
		if (loc) {
			oldLocale_ = std::string(loc);
			hasValidOldLocale_ = true;
			setlocale(LC_CTYPE, locale.c_str());
		}
	}
	
	~ScopedLocale()
	{
		if (hasValidOldLocale_) {
			setlocale(LC_CTYPE, oldLocale_.c_str());
		}
	}
	
private:
	bool hasValidOldLocale_;
	std::string oldLocale_;
};

std::string FileUtil::convert(std::wstring const& str)
{
#if defined(_MSC_VER)
	size_t bufferSizeInBytes = 0;
	{
		errno_t e = wcstombs_s(&bufferSizeInBytes, nullptr, 0, str.c_str(), str.size() * sizeof(wchar_t));
		if (e != 0) {
			return std::string();
		}
	}
	std::vector<char> buffer(bufferSizeInBytes, 0);
	{
		errno_t e = wcstombs_s(&bufferSizeInBytes, buffer.data(), buffer.size(), str.c_str(), str.size() * sizeof(wchar_t));
		return std::string(buffer.data());
	}
#elif defined(__APPLE__)
	CFStringEncoding wcharEncoding = kCFStringEncodingUTF32LE;
	CFStringRef cfstr = CFStringCreateWithBytes(kCFAllocatorDefault,
												(UInt8 const*)str.data(),
												str.size() * sizeof(wchar_t),
												wcharEncoding,
												false /*has_external_representation*/);
	if (!cfstr) {
		return std::string();
	}
	
	CFIndex maxBufLen = CFStringGetMaximumSizeOfFileSystemRepresentation(cfstr);
	std::vector<char> buffer(maxBufLen, 0);
	
	if (!CFStringGetFileSystemRepresentation(cfstr, buffer.data(), maxBufLen)) {
		maxBufLen *= 2;
		buffer.resize(maxBufLen);
		std::fill(buffer.begin(), buffer.end(), 0);
		if (!CFStringGetFileSystemRepresentation(cfstr, buffer.data(), maxBufLen)) {
			CFRelease(cfstr);
			return std::string();
		}
	}
	CFRelease(cfstr);
	
	std::string result(buffer.data());
	return result;
#else
	ScopedLocale locale("");
	
	size_t bufferBytesRequired = wcstombs(nullptr, str.c_str(), 0);
	if (bufferBytesRequired == static_cast<size_t>(-1)) {
		return std::string();
	}
	std::vector<char> buffer(bufferBytesRequired + 1);
	if (wcstombs(buffer.data(), str.c_str(), buffer.size()) == static_cast<size_t>(-1)) {
		return std::string();
	}
	std::string result(buffer.data());
	return result;
#endif
}

LIBVSQ_END_NAMESPACE
