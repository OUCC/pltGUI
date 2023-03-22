﻿#pragma once


/// @brief T型の値と、bool値を保持
/// @brief pltのオプションの値とは別にそれを使用するかどうかに使う
template <class T>
class WithBool {
public:
	bool b = false;
	T v;
};

/// @brief void型の時の完全特殊化
template <>
class WithBool <void> {
public:
	bool b = false;
};


/// @brief タイトルなどグラフ全体の設定
class WholeSetting {
public:
	WithBool<TextEditState> title;
	WithBool<TextEditState> xrange_min;
	WithBool<TextEditState> xrange_max;
	WithBool<TextEditState> yrange_min;
	WithBool<TextEditState> yrange_max;
	WithBool<void> logscale_x;
	WithBool<void> logscale_y;
	WithBool<TextEditState> sample;
};



/// @brief 色などグラフごとの設定
class GraphSetting {
	enum class LineTypeE;
public:
	int graph_index = 0;
	WithBool<TextEditState> graph_func;
	WithBool<TextEditState> graph_data;
	WithBool<TextEditState> using_x;
	WithBool<TextEditState> using_y;
	WithBool<TextEditState> title;
	WithBool<HSV> linecolor;
	WithBool<void> withlines;
	WithBool<TextEditState> linewidth;
	WithBool<TextEditState> linetype;
	WithBool<void> withpoints;
	WithBool<TextEditState> pointsize;
	WithBool<TextEditState> pointtype;
};
