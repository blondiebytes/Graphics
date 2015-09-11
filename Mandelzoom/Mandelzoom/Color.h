#pragma once
class Color {
public:
	double r;
	double g;
	double b;

	Color(double r, double g, double b) :
		r(r), g(g), b(b){};

	Color() :
		r(1), g(1), b(1) {};
};