#pragma once

template<class T>
class WithBool :public T {
public:
	bool enabled;
	WithBool() :enabled(false) {}
	WithBool(T value, bool enabled = false) :T(value), enabled(enabled) {}
	operator T&() {
		return this;
	}
};

class double_ {
public:
	double value;
	bool enabled;
	double_() :value(0), enabled(false) {}
	double_(double value, bool enabled = false) :value(value), enabled(enabled) {}
	operator double&() {
		return value;
	}
};

//typedef WithBool<int> int_;
//typedef WithBool<double> double_;
typedef WithBool<HSV> HSV_;
