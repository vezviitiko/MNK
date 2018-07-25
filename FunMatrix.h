#ifndef _kamaz_FunMatrix_h_
#define _kamaz_FunMatrix_h_

#include <Core/Core.h>
#include "cmath"
using namespace Upp;

void printf(double a[100][100],int n,int show);
void minor_f(double b[100][100],double a[100][100],int i,int n);
double det(double a[100][100],int n);
void transpose(double c[100][100],double d[100][100],int n,double det);
void cofactor(double a[100][100],double d[100][100],int n,double determinte);
void inverse(double a[100][100],double d[100][100],int n,double det);

#endif
