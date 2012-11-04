/**
 * PhoneticSymbolElement.hpp
 * Copyright © 2012 kbinani
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
#ifndef LIBVSQ_PHONETICSYMBOLELEMENT_HPP_
#define LIBVSQ_PHONETICSYMBOLELEMENT_HPP_

#include "vsqglobal.hpp"
#include "StringUtil.hpp"
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief Dictionary item used in PhoneticSymbolConverter.
 */
class PhoneticSymbolElement{
private:
    /**
     * @brief A word. In case English, phrases are separated by '-' letter.
     */
    std::string _word;
    /**
     * @brief A string of phonetic symbols, contains '\t'.
     */
    std::string _rawSymbol;
    /**
     * @brief A string of phonetic symbols, doesn't contain '\t'.
     */
    std::string _symbol;

public:
    /**
     * @brief Initialize an element.
     * @param word A word. In case English, phrases are separated by '-' letter.
     * @param symbol A string of phonetic symbols, contains '\t'.
     */
    PhoneticSymbolElement(const std::string &word, const std::string &symbol){
        _word = word;
        _rawSymbol = symbol;
        _symbol = StringUtil::replace(symbol, "\t", " ");
    }

    /**
     * @brief Get a word. In case English, phrases are separated by '-' letter.
     */
    std::string word()const{
        return _word;
    }

    /**
     * @brief Get a string of phonetic symbols, contains '\t'.
     */
    std::string rawSymbol()const{
        return _rawSymbol;
    }

    /**
     * @brief Get a string of phonetic symbols, doesn't contain '\t'.
     */
    std::string symbol()const{
        return _symbol;
    }
};

VSQ_END_NAMESPACE

#endif


