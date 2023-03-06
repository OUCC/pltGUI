#pragma once


/// @brief T型の値と、bool値を保持
/// @brief pltのオプションの値とは別にそれを使用するかどうかに使う
template <class T>
class WithBool {
public:
	bool b = false;
	T v;

	WithBool() {}
	WithBool(T _v) {
		v = _v;
	}
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
	WithBool<String> title;
	WithBool<String> xrange_min;
	WithBool<String> xrange_max;
	WithBool<String> yrange_min;
	WithBool<String> yrange_max;
	WithBool<void> logscale_x;
	WithBool<void> logscale_y;
	WithBool<String> sample;
};



/// @brief 色などグラフごとの設定
class GraphSetting {
	enum class LineTypeE;
public:
	String graph;
	WithBool<String> label;
	WithBool<HSV> linecolor;
};
