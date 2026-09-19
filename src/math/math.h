#ifndef SUPER_MATH_H
#define SUPER_MATH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

double math_sin(double x);
double math_cos(double x);
double math_tan(double x);
double math_asin(double x);
double math_acos(double x);
double math_atan(double x);
double math_sqrt(double x);
double math_pow(double x, double y);
double math_exp(double x);
double math_log(double x);
double math_floor(double x);
double math_ceil(double x);
double math_fabs(double x);
double math_fmod(double x, double y);
long math_rand(void);
void math_srand(unsigned int seed);

#ifdef __cplusplus
}
#endif

#endif