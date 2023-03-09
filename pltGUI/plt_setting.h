#pragma once


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
	GraphSetting() { graph.b = true; }
	WithBool<TextEditState> graph;
	WithBool<TextEditState> title;
	WithBool<HSV> linecolor;
};
