#include "converter.hpp"

/*
Credit to: https://stackoverflow.com/questions/36960456/how-to-convert-hex-to-ieee-754-32-bit-float-in-c
*/
double Converter::convertIEEE754toFloat(unsigned long number, char isDoublePrecise) {
	int mantissaShift = isDoublePrecise ? 52 : 23;
	unsigned long exponentMask = isDoublePrecise ? 0x7FF0000000000000 : 0x7f800000;
	int bias = isDoublePrecise ? 1023 : 127;
	int signShift = isDoublePrecise ? 63 : 31;

	int sign = (number >> signShift) & 0x01;
	int exponent = ((number & exponentMask) >> mantissaShift) - bias;

	int power = -1;
	double total = 0.0;
	for (int i = 0; i < mantissaShift; i++) {
		int calc = (number >> (mantissaShift - i - 1)) & 0x01;
		total += calc * pow(2.0, power);
		power--;
	}
	double value = (sign ? -1 : 1) * pow(2.0, exponent) * (total + 1.0);
	return value;
}