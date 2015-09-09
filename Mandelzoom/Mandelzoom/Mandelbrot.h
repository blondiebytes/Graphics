#pragma once
class Mandelbrot {
public: 
	double x1;
	double x2;
	double y1;
	double y2;

	Mandelbrot(double x1, double x2, double y1, double y2) :
		x1(x1), x2(x2), y1(y1), y2(y2) {};
};