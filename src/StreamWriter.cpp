/**
 * StreamWriter.cpp
 * Copyright © 2014 kbinani
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
#include "../include/libvsq/StreamWriter.hpp"
#include "../include/libvsq/FileOutputStream.hpp"

VSQ_BEGIN_NAMESPACE

StreamWriter::StreamWriter(std::string const& filePath)
{
	try {
		stream = new FileOutputStream(filePath);
	} catch (OutputStream::IOException) {
		throw TextOutputStream::IOException();
	}
	deleteInDestructor = stream;
}

StreamWriter::StreamWriter(OutputStream* stream)
{
	this->stream = stream;
	this->deleteInDestructor = 0;
}

StreamWriter::~StreamWriter()
{
	close();
}

void StreamWriter::close()
{
	if (stream) {
		stream->close();
		stream = 0;
	}
	if (deleteInDestructor) {
		delete deleteInDestructor;
		deleteInDestructor = 0;
	}
}

void StreamWriter::write(std::string const& text)
{
	if (stream) { stream->write(text.c_str(), 0, text.length()); }
}

void StreamWriter::writeLine(std::string const& text)
{
	write(text);
	if (stream) { stream->write(0x0A); }
}

VSQ_END_NAMESPACE
