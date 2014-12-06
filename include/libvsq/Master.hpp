/**
 * @file Master.hpp
 * Copyright © 2012,2014 kbinani
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

#include "./Namespace.hpp"
#include <string>

LIBVSQ_BEGIN_NAMESPACE

class TextStream;

/**
 * @brief VSQ ファイルのメタテキストの [Master] に記録される内容を取り扱うクラス.
 */
class Master
{
public:
	/**
	 * @brief プリメジャーの長さ(小節数).
	 */
	int preMeasure;

	Master();

	/**
	 * @brief プリメジャーを指定し, 初期化を行う.
	 * @param preMeasure プリメジャーの長さ(小節数).
	 */
	explicit Master(int preMeasure);

	/**
	 * @brief テキストストリームから読み込むことで初期化を行う.
	 * @param stream 読み込むテキストストリーム.
	 * @param lastLine 読み込んだ最後の行.
	 */
	Master(TextStream& stream, std::string& lastLine);

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	Master clone() const;

	/**
	 * @brief テキストストリームに出力する.
	 * @param stream (TextStream) 出力先.
	 */
	void write(TextStream& stream) const;
};

LIBVSQ_END_NAMESPACE
