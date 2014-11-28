/**
 * Handle.hpp
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

#include "./BasicTypes.hpp"
#include "./VibratoBPList.hpp"
#include "./HandleType.hpp"
#include "./ArticulationType.hpp"
#include "./Lyric.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief ハンドルを取り扱います. ハンドルには LyricHandle, VibratoHandle, SingerHandle および NoteHeadHandle がある.
 * @todo get, set 両方あるプロパティについては, fieldにしてよいのでは.
 */
class Handle
{
public:
	/**
	 * @brief メタテキストに出力されるこのオブジェクトの ID.
	 * @todo VSQ への書き出し時にのみ使っているので, うまいことして削除する.
	 */
	int index;

	/**
	 * @brief ハンドルを特定するための文字列.
	 */
	std::string iconId;

	/**
	 * @brief ハンドルの名前.
	 */
	std::string ids;

	/**
	 * @brief ハンドルのオリジナル.
	 */
	int original;

	/**
	 * @brief 歌手の歌唱言語を表す番号(バンクセレクト). 歌手ハンドルでのみ使われる.
	 */
	int language;

	/**
	 * @brief 歌手の種類を表す番号(プログラムチェンジ). 歌手ハンドルでのみ使われる.
	 */
	int program ;

	/**
	 * @brief 歌詞・発音記号列の前後にクォーテーションマークを付けるかどうか.
	 */
	bool addQuotationMark;

	/**
	 * @brief キャプション.
	 */
	std::string caption;

	/**
	 * @brief Dyn の開始値.
	 */
	int startDyn;

	/**
	 * @brief Dyn の終了値.
	 */
	int endDyn;

	/**
	 * @brief Depth.
	 */
	int depth;

	/**
	 * @brief Duration.
	 */
	int duration;

	/**
	 * @brief Rate の開始値.
	 */
	int startRate;

	/**
	 * @brief Depth の開始値.
	 */
	int startDepth;

	/**
	 * @brief Depth のカーブ.
	 */
	VibratoBPList depthBP;

	/**
	 * @brief Rate のカーブ.
	 */
	VibratoBPList rateBP;

	/**
	 * @brief Dyn のカーブ.
	 */
	VibratoBPList dynBP;

protected:
	HandleType::HandleTypeEnum _type;

	ArticulationType::ArticulationTypeEnum _articulation;

	/**
	 * @brief 歌詞一覧.
	 */
	std::vector<Lyric> _lyrics;

	/**
	 * @brief 長さ.
	 */
	int _length;

public:
	/**
	 * @brief 強弱記号の場合の, IconId の最初の5文字.
	 * @return IconId の接頭辞.
	 */
	inline static const std::string getIconIdPrefixDynaff()
	{
		return "$0501";
	}

	/**
	 * @brief クレッシェンドの場合の, IconId の最初の5文字.
	 * @return IconId の接頭辞.
	 */
	inline static const std::string getIconIdPrefixCrescend()
	{
		return "$0502";
	}

	/**
	 * @brief デクレッシェンドの場合の, IconId の最初の5文字.
	 * @return IconId の接頭辞.
	 */
	inline static const std::string getIconIdPrefixDecrescend()
	{
		return "$0503";
	}

	/**
	 * @param type ハンドルの種類.
	 */
	explicit Handle(HandleType::HandleTypeEnum type = HandleType::UNKNOWN);

	/**
	 * @brief articulation の種類を取得する.
	 * @return articulation の種類.
	 */
	ArticulationType::ArticulationTypeEnum getArticulation() const;

	/**
	 * @brief このハンドルが強弱記号を表すものかどうかを表すブール値を取得する.
	 * @return このオブジェクトが強弱記号を表すものであれば <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool isDynaffType() const;

	/**
	 * @brief このハンドルがクレッシェンドを表すものかどうかを表すブール値を取得する
	 * @return このオブジェクトがクレッシェンドを表すものであれば <code>true</code> を, そうでなければ <code>false</code> を返す
	 */
	bool isCrescendType() const;

	/**
	 * @brief このハンドルがデクレッシェンドを表すものかどうかを表すブール値を取得する.
	 * @return このオブジェクトがデクレッシェンドを表すものであれば <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool isDecrescendType() const;

	/**
	 * @brief Tick 単位の長さを取得する.
	 */
	tick_t getLength() const;

	/**
	 * @brief 長さを設定する.
	 * @param value Tick単位の長さ.
	 */
	void setLength(tick_t value);

	/**
	 * @brief 指定した位置にある歌詞を取得する.
	 * @param index 取得する要素のインデックス(最初のインデックスは0).
	 * @return 歌詞.
	 */
	const Lyric getLyricAt(int index) const;

	/**
	 * @brief 指定した位置にある歌詞を指定した要素で置き換える.
	 * @param index 置き換える要素のインデックス(最初のインデックスは0).
	 * @param value 置き換える要素.
	 */
	void setLyricAt(int index, Lyric const& value);

	/**
	 * @brief 歌詞を追加する.
	 * @param lyric 追加する歌詞.
	 */
	void addLyric(Lyric lyric);

	/**
	 * @brief 歌詞の個数を返す.
	 * @return 歌詞の個数.
	 */
	int getLyricCount() const;

	/**
	 * @brief Display String 値を取得する.
	 * @return Display String 値.
	 */
	const std::string getDisplayString() const;

	/**
	 * @brief ハンドルのタイプを取得する.
	 * @return ハンドルのタイプ.
	 */
	HandleType::HandleTypeEnum getHandleType() const;

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	Handle clone() const;

	/**
	 * @brief ハンドル指定子（例えば"h#0123"という文字列）からハンドル番号を取得する.
	 * @param s ハンドル指定子.
	 * @return ハンドル番号.
	 */
	static int getHandleIndexFromString(std::string const& s);

protected:
	void init();

	/**
	 * @brief ビブラートハンドルとして初期化を行う.
	 */
	void _init_vibrato();

	/**
	 * @brief 強弱記号ハンドルとして初期化を行う.
	 */
	void _init_icon_dynamics();

	/**
	 * @brief 歌詞ハンドルとして初期化を行う.
	 */
	void _init_lyric();
};

VSQ_END_NAMESPACE
