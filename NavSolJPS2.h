#ifndef _NavSolJps2_NavSolJps2_h
#define _NavSolJps2_NavSolJps2_h

#include <Core/Core.h>
#include <cmath>


using namespace Upp;
using namespace std;

#include "FunMatrix.h"

class InputData : public Moveable <InputData>{
public:
	int satNum;
	double jd;
	double psRangLen;
	
	//Конструкторы
	InputData() {}
	InputData(int sn, double pseudoRangeLength, double JD){
		satNum = sn;
		psRangLen = pseudoRangeLength;
		jd = JD;
	}
};

class Ephemeris : public Moveable <Ephemeris>{
public:
	double jd;
	double x;
	double y;
	double z;
	
	Ephemeris() {}
	Ephemeris(double JD, double X, double Y, double Z)
	{
		jd = JD;
		x = X;
		y = Y;
		z = Z;
	}
};

class Coord : public Moveable <Coord>{
	double phi;
	double gam;
};

class hLine : public Moveable <hLine>{
public:
	double xx;
	double yy;
	double zz;
	double one;
	
	hLine() {one = 1;}
	hLine(double XX, double YY, double ZZ){
		xx = XX;
		yy = YY;
		zz = ZZ;
		one = 1;
	}
};

//Функция, которая решает навигационную задачу
Coord NavProblemPsRang(Vector <InputData> &inData, Vector <Ephemeris> &eph);
Coord NewNavProb(Vector <InputData> &inData, Vector <Ephemeris> &eph);
void HtHPowCalc(Vector <hLine> &H, double HtH[4][4]);
void SqrMatrVectMultiply(float m[100][100], float v[100], int order);
void MatrMultiply(double a[100][100], double b[100][100]);
void RazMatr(double a[100][100], double b[100][100]);
//void TransposeMatrix(Vector <hLine> &H,float b[100][100]);
#endif
