/**
 * @file PublicForUnitTest.hpp
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
 * @def LIBVSQ_PRIVATE_BUT_PUBLIC_FOR_UNITTEST
 * @brief リリース版では private だが, それ以外では単体テストのため public となるマクロ.
 * @details NDEBUG が define されていれば private に, それ以外では public となる.
 */
#if defined NDEBUG
#    define LIBVSQ_PRIVATE_BUT_PUBLIC_FOR_UNITTEST private
#else
#    define LIBVSQ_PRIVATE_BUT_PUBLIC_FOR_UNITTEST public
#endif
