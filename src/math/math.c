#include "math.h"
#include <math.h>
#include <stdlib.h>

double math_sin(double x) { return sin(x); }
double math_cos(double x) { return cos(x); }
double math_tan(double x) { return tan(x); }
double math_asin(double x) { return asin(x); }
double math_acos(double x) { return acos(x); }
double math_atan(double x) { return atan(x); }
double math_sqrt(double x) { return sqrt(x); }
double math_pow(double x, double y) { return pow(x, y); }
double math_exp(double x) { return exp(x); }
double math_log(double x) { return log(x); }
double math_floor(double x) { return floor(x); }
double math_ceil(double x) { return ceil(x); }
double math_fabs(double x) { return fabs(x); }
double math_fmod(double x, double y) { return fmod(x, y); }

long math_rand(void) { return rand(); }
void math_srand(unsigned int seed) { srand(seed); }