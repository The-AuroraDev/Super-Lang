#ifndef SUPER_MATH_H
#define SUPER_MATH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

double mathSin(double x);
double mathCos(double x);
double mathTan(double x);
double mathAsin(double x);
double mathAcos(double x);
double mathAtan(double x);
double mathSqrt(double x);
double mathPow(double x, double y);
double mathExp(double x);
double mathLog(double x);
double mathFloor(double x);
double mathCeil(double x);
double mathFabs(double x);
double mathFmod(double x, double y);
long mathRand(void);
void mathSrand(unsigned int seed);

#ifdef __cplusplus
}
#endif

#endif