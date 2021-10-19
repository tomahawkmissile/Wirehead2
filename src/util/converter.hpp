#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

class Converter {
public:
	double convertIEEE754toFloat(unsigned long number, char isDoublePrecise);
};

#endif