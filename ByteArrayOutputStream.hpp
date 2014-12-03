#ifndef __ByteArrayOutputStream_h__
#define __ByteArrayOutputStream_h__

#include <string>
#include <cmath>
#include <vector>
#include "vsqglobal.hpp"
#include "OutputStream.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * データがバイト配列に書き込まれるストリームの実装
 * @class table
 * @name ByteArrayOutputStream
 */
class ByteArrayOutputStream : public VSQ_NS::OutputStream
{
protected:
	/**
	 * @brief 確保するバッファー・ブロックのサイズ(バイト単位)
	 */
	static const int UNIT_BUFFER_LENGTH = 512;

private:
	/**
	 * 現在のファイルポインタ
	 */
	int64_t _pointer;

	/**
	 * @brief 書き込み先のバイト列
	 */
	char* _array;

	/**
	 * @brief 確保されたバイト列の現在の長さ
	 */
	int64_t _arrayLength;

	/**
	 * @brief 書き込み済みバイト数
	 */
	int64_t _length;

public:
	/**
	 * @brief 初期化を行う
	 */
	ByteArrayOutputStream()
	{
		_pointer = 0;
		_arrayLength = UNIT_BUFFER_LENGTH;
		_array = (char*)::calloc(_arrayLength, sizeof(char));
		_length = 0;
	}

	~ByteArrayOutputStream()
	{
		close();
	}

	/**
	 * 指定されたバイト値をストリームに書きこむ
	 * @param byte (int) 書きこむバイト値
	 * @name write<! *1 *>
	 */
	void write(int byte)
	{
		ensureBufferLength(_pointer + 1);
		_array[_pointer] = (char)byte;
		_pointer++;
		_length = std::max(_length, _pointer);
	}

	/**
	 * @brief 指定された配列の、指定した範囲のバイト値をストリームに書きこむ
	 * @param array (table) 書きこむバイト列が格納された配列
	 * @param startIndex (int) 書き込み開始位置
	 * @param length (int) 書き込むバイト値の個数
	 * @name write<! *2 *>
	 */
	void write(const char* array, int64_t startIndex, int64_t length)
	{
		ensureBufferLength(_pointer + length);
		for (int i = 0; i < length; i++) {
			_array[_pointer + i] = array[startIndex + i];
		}
		_pointer += length;
		_length = std::max(_length, _pointer);
	}

	/**
	 * @brief バイト列を文字列に変換する
	 * @return (string) 変換された文字列
	 */
	const std::string toString()
	{
		std::string result(_array, _length);
		return result;
	}

	/**
	 * @brief 現在のファイルポインタを取得する
	 * @return (int) 現在のファイルポインタ
	 */
	int64_t getPointer()
	{
		return _pointer;
	}

	/**
	 * @brief ファイルポインタを指定した位置に変更する
	 * @param position (int) 新しいポインタ値
	 */
	void seek(int64_t position)
	{
		_pointer = (position < 0) ? 0 : position;
	}

	/**
	 * @brief ストリームを閉じる
	 */
	void close()
	{
		if (_array) {
			free(_array);
			_array = NULL;
		}
		_length = 0;
		_arrayLength = 0;
	}

private:
	/**
	 * @brief バッファーを指定した長さまで確保する
	 * @param length 確保するバッファーの長さ
	 */
	void ensureBufferLength(int length)
	{
		int amount = length - _arrayLength;
		if (0 < amount) {
			char* temporary = (char*)realloc(_array, length * sizeof(char));
			if (!temporary) {
				close();
				return;
			}
			_array = temporary;
			for (int i = _arrayLength; i < _arrayLength + amount; i++) {
				_array[i] = 0;
			}
			_arrayLength = length;
		}
	}
};

VSQ_END_NAMESPACE

#endif
