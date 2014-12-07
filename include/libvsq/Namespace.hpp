/**
 * @file Namespace.hpp
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
#pragma once

/**
 * @def LIBVSQ_NS
 * @brief libvsq のルート名前空間.
 */
#define LIBVSQ_NS vsq

/**
 * @def LIBVSQ_BEGIN_NAMESPACE
 * @brief libvsq のルート名前空間を開始する namespace ブロックを挿入する.
 */
#define LIBVSQ_BEGIN_NAMESPACE	namespace LIBVSQ_NS {

/**
 * @def LIBVSQ_END_NAMESPACE
 * @brief libvsq のルート名前空間を終了する "}" を挿入する.
 */
#define LIBVSQ_END_NAMESPACE	}
