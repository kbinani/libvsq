/**
 * @file FileUtil.hpp
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
#pragma once

#include <libvsq/Namespace.hpp>
#include <string>
#include <iostream>

LIBVSQ_BEGIN_NAMESPACE

class FileUtil
{
public:
	FileUtil() = delete;
	
	template <class Stream>
	static void open(Stream& stream, std::wstring const& filePath, std::ios_base::openmode mode)
	{
#ifdef _MSC_VER
		stream.open(filePath, mode);
#else
		std::string path = convert(filePath);
		stream.open(path, mode);
#endif
	}
	
private:
	static std::string convert(std::wstring const& s);
};

LIBVSQ_END_NAMESPACE
