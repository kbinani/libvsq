/**
 * @file VSQXFileReader.hpp
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
#include <libvsq/Sequence.hpp>
#include <memory>

LIBVSQ_BEGIN_NAMESPACE

class Sequence;
class InputStream;

/**
 * @brief VSQX ファイルのシーケンスを読み込み, Sequence オブジェクトを作成するクラス.
 */
class VSQXFileReader
{
public:
	VSQXFileReader();
	
	~VSQXFileReader();
	
	/**
	 * @brief 読み込みストリームから, VSQ ファイルを読み込む.
	 * @param sequence 読み込み結果の格納先.
	 * @param stream 読み込みストリーム.
	 */
	void read(Sequence& sequence, InputStream& stream) const;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

LIBVSQ_END_NAMESPACE
