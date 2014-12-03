/**
 * CP932Converter.hpp
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
#ifndef __CP932Converter_hpp__
#define __CP932Converter_hpp__

#include "vsqglobal.hpp"
#include <map>
#include <sstream>
#include <vector>

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * @brief UTF8 から CP932 への文字コード変換ユーティリティ
 */
class CP932Converter
{
private:

public:
	/**
	 * @brief UTF8 の文字列を CP932 の文字列に変換する
	 * @param utf8 (string) 変換前の文字列
	 * @return (string) 変換後の文字列
	 */
	static string convertFromUTF8(const string& utf8)
	{
		vector<vector<int> > utf8codes = _getUnicodeBytesFromUTF8String(utf8);
		ostringstream result;
		static int _unicode_to_cp932[256][256][2];
		static int initialized = 0;
		if (initialized == 0) {
			initializeUnicodeToCp932Dictionary(_unicode_to_cp932);
			initialized = 1;
		}
		for (int i = 0; i < utf8codes.size(); i++) {
			vector<int> r = utf8codes[i];
			if (r.size() == 1) {
				result << (char)r[0];
			} else if (r.size() == 2) {
				int firstByte = r[0];
				if (_unicode_to_cp932[firstByte] != 0) {
					int secondByte = r[1];
					if (_unicode_to_cp932[firstByte][secondByte][0] != 0) {
						result << (char)(0xff & _unicode_to_cp932[firstByte][secondByte][0]);
						if (_unicode_to_cp932[firstByte][secondByte][1] != 0) {
							result << (char)(0xff & _unicode_to_cp932[firstByte][secondByte][1]);
						}
					}
				}
			}
		}
		return result.str();
	}

	/**
	 * @brief CP932のバイト列をUTF8の文字列に変換します
	 * @param cp932 変換する CP932 のバイト列
	 * @return 変換後の UTF8 文字列
	 */
	static std::string convertToUTF8(const std::string& cp932)
	{
		static int dict[0xFFFF];
		static int initialized = 0;
		if (initialized == 0) {
			initializeCP932ToUTF8Dictionary(dict);
			initialized = 1;
		}
		std::ostringstream result;
		int i = 0;
		int length = cp932.size();
		while (i < length) {
			int b1 = 0xFF & cp932[i];
			int b2 = 0;
			if (i + 1 < length) { b2 = 0xFF & cp932[i + 1]; }
			int b1b2 = (0xFF00 & (b1 << 8)) | (0xFF & b2);
			int value;
			if ((value = dict[b1]) != 0) {
				result << (char)value;
				i++;
			} else if ((value = dict[b1b2]) != 0) {
				result << (char)(0xFF & (value >> 16));
				result << (char)(0xFF & (value >> 8));
				result << (char)(0xFF & value);
				i += 2;
			} else {
				i++;
			}
		}
		return result.str();
	}

protected:
	/**
	 * @brief UTF8 の文字列を unicode のバイト列に変換する
	 * @param s (string) 変換前の文字列
	 * @return (table) 変換後の unicode のバイト列
	 */
	static vector<vector<int> > _getUnicodeBytesFromUTF8String(const string& s)
	{
		vector<vector<int> > result;
		int i = 0;
		while (i < s.size()) {
			int b = 0xff & (int)s[i];
			int byteCount = 1;
			vector<int> utf8bytes;
			utf8bytes.push_back(b);
			if (b <= 0x7f) {
				// 1byte
				byteCount = 1;
			} else if (b <= 0xdf) {
				// 2byte
				byteCount = 2;
			} else if (b <= 0xef) {
				// 3byte
				byteCount = 3;
			} else if (b <= 0xf7) {
				// 4byte
				byteCount = 4;
			} else if (b <= 0xfb) {
				// 5byte
				byteCount = 5;
			} else {
				// 6byte
				byteCount = 6;
			}
			for (int j = 1; j < byteCount; j++) {
				int a = 0xff & (int)s[i + j];
				utf8bytes.push_back(a);
			}

			vector<int> r = _getUnicodeBytesFromUTF8Bytes(utf8bytes);
			result.push_back(r);

			i = i + byteCount;
		}
		return result;
	}

	/**
	 * @brief UTF8 のバイト列を Unicode のバイト列に変換する
	 * @param (table) utf8 UTF8のバイト列
	 * @return (table) Unicode のバイト列
	 */
	static vector<int> _getUnicodeBytesFromUTF8Bytes(const vector<int>& utf8)
	{
		if (utf8.size() == 1) {
			// 0xxx xxxx
			vector<int> result;
			result.push_back(utf8[0] & 0x7f);
			return result;
		} else if (utf8.size() == 2) {
			// 110yyy yx | 10xx xxxx
			//    [2] [7     65 432]
			vector<int> result;
			result.push_back(((utf8[0] >> 2) & 0x7));
			// 0x80
			result.push_back((((utf8[0] & 0x3) << 6) | (utf8[1] & 0x3f)));
			if (result[0] == 0) {
				result.erase(result.begin());
			}
			return result;
		} else if (utf8.size() == 3) {
			// 1110yyyy | 10yxxx xx | 10xx xxxx
			//     [765     432] [7     65 432]
			vector<int> result;
			result.push_back((((utf8[0] & 0xf) << 4) | ((utf8[1] >> 2) & 0xf)));
			result.push_back((((utf8[1] & 0x3) << 6) | (utf8[2] & 0x3f)));
			return result;
		} else if (utf8.size() == 4) {
			// 11110y yy | 10yy xxxx | 10xxxx xx | 10xx xxxx
			//      [ 43     2] [765     432] [7     65 432]
			vector<int> result;
			result.push_back((((utf8[0] & 0x7) << 2) | ((utf8[1] & 0x30) >> 4)));
			result.push_back((((utf8[1] & 0xf) << 4) | ((utf8[2] & 0x3c) >> 2)));
			result.push_back((((utf8[2] & 0x3) << 6) | (utf8[3] & 0x3f)));
			return result;
		} else if (utf8.size() == 5) {
			// 111110yy | 10yyyx xx | 10xx xxxx | 10xxxx xx | 10xx xxxx
			//       []     [765 43     2] [765     432] [7     65 432]
			vector<int> result;
			result.push_back((utf8[0] & 0x3));
			result.push_back((((utf8[1] & 0x3f) << 2) | ((utf8[2] & 0x30) >> 4)));
			result.push_back((((utf8[2] & 0xf) << 4) | ((utf8[3] & 0x3c) >> 2)));
			result.push_back((((utf8[3] & 0x3) << 6) | (utf8[4] & 0x3f)));
			if (result[0] == 0) {
				result.erase(result.begin());
			}
			return result;
		} else if (utf8.size() == 6) {
			// 1111110y | 10yy yyxx | 10xxxx xx | 10xx xxxx | 10xxxx xx | 10xx xxxx
			//        [     65 432]     [765 43     2] [765     432] [7     65 432]
			vector<int> result;
			result.push_back((((utf8[0] & 0x1) << 6) | (utf8[1] & 0x3f)));
			result.push_back((((utf8[2] & 0x3f) << 2) | ((utf8[3] & 0x30) >> 4)));
			result.push_back((((utf8[3] & 0xf) << 4) | ((utf8[4] & 0x3c) >> 2)));
			result.push_back((((utf8[4] & 0x3) << 6) | (utf8[5] & 0x3f)));
			return result;
		} else {
			vector<int> result;
			return result;
		}
	}

private:
	/**
	 * @brief Unicode から CP932 への変換テーブルを初期化する
	 */
	static void initializeUnicodeToCp932Dictionary(int dict[256][256][2])
	{
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 256; j++) {
				dict[i][j][0] = 0;
				dict[i][j][1] = 0;
			}
		}
#include "CP932ConverterData.hpp"
	}

	/**
	 * @brief CP932 から UTF8 への変換テーブルを初期化する
	 */
	static void initializeCP932ToUTF8Dictionary(int dict[0xFFFF])
	{
		int baseDictionary[256][256][2];
		initializeUnicodeToCp932Dictionary(baseDictionary);
		for (int i = 0; i < 0xFFFF; i++) {
			dict[i] = 0;
		}
		for (int firstByte = 0; firstByte < 256; firstByte++) {
			for (int secondByte = 0; secondByte < 256; secondByte++) {
				int key = baseDictionary[firstByte][secondByte][0];
				if (key == 0) { continue; }
				if (baseDictionary[firstByte][secondByte][1] != 0) {
					key = (0xFF00 & (key << 8)) | (0xFF & baseDictionary[firstByte][secondByte][1]);
				}
				int value = 0;
				if (0 == firstByte) {
					value = secondByte;
				} else {
					int c1 = 0xE0 | (0xFF & (firstByte >> 4));
					int c2 = 0x80 | (0x3C & (firstByte << 2)) | (0xFF & (secondByte >> 6));
					int c3 = 0x80 | (0x3F & secondByte);
					value = (0xFF0000 & (c1 << 16)) | (0x00FF00 & (c2 << 8)) | (0x0000FF & c3);
				}
				dict[key] = value;
			}
		}
	}
};

VSQ_END_NAMESPACE

#endif
