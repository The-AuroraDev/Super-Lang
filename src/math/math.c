#include "math.h"
#include <math.h>
#include <stdlib.h>

double mathSin(double x) { return sin(x); }
double mathCos(double x) { return cos(x); }
double mathTan(double x) { return tan(x); }
double mathAsin(double x) { return asin(x); }
double mathAcos(double x) { return acos(x); }
double mathAtan(double x) { return atan(x); }
double mathSqrt(double x) { return sqrt(x); }
double mathPow(double x, double y) { return pow(x, y); }
double mathExp(double x) { return exp(x); }
double mathLog(double x) { return log(x); }
double mathFloor(double x) { return floor(x); }
double mathCeil(double x) { return ceil(x); }
double mathFabs(double x) { return fabs(x); }
double mathFmod(double x, double y) { return fmod(x, y); }

long mathRand(void) { return rand(); }
void mathSrand(unsigned int seed) { srand(seed); }