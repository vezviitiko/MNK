#include "NavSolJps2.h"

/*
Original matr 
0.346717956016752 0.775945367363548 0.526968163974461 1
-0.477926871647419 0.250247531252998 0.841998858941681 1
0.430377509811999 0.0904845839778748 0.898102298800294 1
0.221310830036842 0.508016888051497 0.832430392262847 1
0.225916220919835 -0.660408806946972 0.716115960464629 1
Original matr HtH * H
0.633868858985396 0.151609279172268 0.512826547616666 0.746395645138009
0.151609279172268 1.36712345080747 0.6508304645393 0.964285563698946
0.512826547616666 0.6508304645393 2.99900769020713 3.81561567444391
0.746395645138009 0.964285563698946 3.81561567444391 5
0.633868858985396 0.151609279172268 0.512826547616666 0.746395645138009

Original matr test output
0.633868858985396 0.151609279172268 0.512826547616666 0.746395645138009
0.151609279172268 1.36712345080747 0.6508304645393 0.964285563698946
0.512826547616666 0.6508304645393 2.99900769020713 3.81561567444391
0.746395645138009 0.964285563698946 3.81561567444391 5

Reverce matr test output
2.06853340290117 0.0898079034768273 1.43377034691737 -1.4202522795655
0.0898079034768261 0.914441791363228 0.949982479440221 -0.914716657061569
1.43377034691737 0.94998247944022 13.3241439192348 -10.5652053441074
-1.4202522795655 -0.914716657061569 -10.5652053441074 8.65097625968786

coord test output
-3352.92219667853 0 
-2540.29272250176 0 
-6692.28195512037 0 
26971.7723292483 0 

pow(pow(obsVect[0][0], 2) + pow(obsVect[1][0], 2) + pow(obsVect[2][0], 2) , 0.5) = 7904.54376544306
*/
CONSOLE_APP_MAIN
{
	Vector <InputData> inDatVect;
	Vector <Ephemeris> eph;
	
	//Зададим массив входящих данных
	double inData[2][7];
	inData[0][0] = 20;
	inData[0][1] = 4;
	inData[0][2] = 21;
	inData[0][3] = 5;
	inData[0][4] = 22;
/*	
	inData[1][0] = 20317.7206562;
	inData[1][1] = 22309.8467601915;
	inData[1][2] = 19305.7438067749;
	inData[1][3] = 19344.8207480251;
	inData[1][4] = 23093.3796233308;
*/
	
	//Попробуем использовать модельные измерения
	double xx = -3782627.48060055 / 1000.;
	double yy = 706759.897227085 / 1000.;
	double zz = 6330779.72431116 / 1000.;

	//Эфемериды аппаратов
	double ephem[5][3];
	ephem[0][0] = 8852.877527; ephem[0][1] = 19812.499427; ephem[0][2] = 13455.272608;	//20-й аппарат
	ephem[1][0] = -12198.743176; ephem[1][1] = 6387.390091; ephem[1][2] = 21491.421479;	//4-й аппарат
	ephem[2][0] = 10976.098464; ephem[2][1] = 2307.666364; ephem[2][2] = 22904.680281;	//21
	ephem[3][0] = 5644.592226; ephem[3][1] = 12957.107325; ephem[3][2] = 21231.360978;	//5
	ephem[4][0] = 5752.717420; ephem[4][1] = -16816.611187; ephem[4][2] = 18235.134882;	//22
	
//	inData[1][0] = pow((pow((xx - ephem[0][0]), 2) + pow((yy - ephem[0][1]), 2) + pow((zz - ephem[0][2]), 2)), 0.5);
//	inData[1][1] = pow((pow((xx - ephem[1][0]), 2) + pow((yy - ephem[1][1]), 2) + pow((zz - ephem[1][2]), 2)), 0.5);
//	inData[1][2] = pow((pow((xx - ephem[2][0]), 2) + pow((yy - ephem[2][1]), 2) + pow((zz - ephem[2][2]), 2)), 0.5);
//	inData[1][3] = pow((pow((xx - ephem[3][0]), 2) + pow((yy - ephem[3][1]), 2) + pow((zz - ephem[3][2]), 2)), 0.5);
//	inData[1][4] = pow((pow((xx - ephem[4][0]), 2) + pow((yy - ephem[4][1]), 2) + pow((zz - ephem[4][2]), 2)), 0.5);

	inData[1][0] = 20317.7206562;
	inData[1][1] = 22309.8467601915;
	inData[1][2] = 19305.7438067749;
	inData[1][3] = 19344.8207480251;
	inData[1][4] = 23093.3796233308;
	
	//Заполним массивы входящих данных
	for (int i = 0; i < 5; i++){
		inDatVect.Add(InputData(inData[0][i], inData[1][i], 0.0));
		eph.Add(Ephemeris(0, ephem[i][0], ephem[i][1], ephem[i][2]));
	}
	
	LOG("Data test output");
	for (int i = 0; i < inDatVect.GetCount(); i++){
		RDUMP(inDatVect[i].psRangLen);
	}
	
	LOG("In the procedure");
	//Найдём координаты станции
	//Coord statPos = NavProblemPsRang(inDatVect, eph);
	Coord statPos = NewNavProb(inDatVect, eph);
}

//Новый алгоритм
Coord NewNavProb(Vector <InputData> &inData, Vector <Ephemeris> &eph){
	Coord outDat;
	
	//Зададим начальные данные
	double x[4] = {0, 0, 0, 0};
	Vector <double> roApr;
	Vector <hLine> H;
	
	//Вычислим вектор приближенных дальностей
	roApr.Clear();
	for (int i = 0; i < eph.GetCount(); i++){
		roApr.Add(pow((pow(eph[i].x - x[0], 2) + pow(eph[i].y - x[1], 2) + pow(eph[i].z - x[2], 2)), 0.5));
		//roApr.Add(pow(pow(eph[i].x - x[0], 2), 0.5));
	}
	
	//Вычислим матрицу топоцентрических направлений
	H.Clear();
	for (int i = 0; i < roApr.GetCount(); i++){
		double hX = (eph[i].x - x[0]) / roApr[i];
		double hY = (eph[i].y - x[1]) / roApr[i];
		double hZ = (eph[i].z - x[2]) / roApr[i];
		H.Add(hLine(hX, hY, hZ));
	}
	
	//Вычислим произведение транспонированной матрицы и квадрата матрицы коррекции
	double W[5][5] ={{1.,0,0,0,0},
					{0,1.,0,0,0},
					{0,0,1.,0,0},
					{0,0,0,1.,0},
					{0,0,0,0,1.}};
	double WMatr[100][100];
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
		WMatr[i][j]  = W[i][j];
		}
	}
	LOG("matr VES");
	for (int i = 0; i < 5; i++){
		LOG(AsString(WMatr[i][0]) << " " << AsString(WMatr[i][1]) << " " << AsString(WMatr[i][2]) << " " << AsString(WMatr[i][3]) << " " << AsString(WMatr[i][4]));
	}
	
	for (int ig = 0; ig < 10; ig++){
		/*
		w1 = 1796.83516077875
		w2 = 1276.08421901684
		w3 = 4138.66270935083
		w4 = 26971.7723292483
		
		w1 = 1792549.21117651
		w2 = 1517561.9378534
		w3 = 4470350.91362187
		w4 = 27188349.3342348
		*/
		
		
		LOG("Original matr ");
		for (int i = 0; i < H.GetCount(); i++){
			LOG(AsString(H[i].xx) << " " << AsString(H[i].yy) << " " << AsString(H[i].zz) << " " << AsString(H[i].one));
		}
		// транспонирование
		double Ht[100][100];
		
		Ht[0][0] = H[0].xx;
		Ht[1][0] = H[0].yy;
		Ht[2][0] = H[0].zz;
		Ht[3][0] = H[0].one;
		Ht[0][1] = H[1].xx;
		Ht[1][1] = H[1].yy;
		Ht[2][1] = H[1].zz;
		Ht[3][1] = H[1].one;
		Ht[0][2] = H[2].xx;
		Ht[1][2] = H[2].yy;
		Ht[2][2] = H[2].zz;
		Ht[3][2] = H[2].one;
		Ht[0][3] = H[3].xx;
		Ht[1][3] = H[3].yy;
		Ht[2][3] = H[3].zz;
		Ht[3][3] = H[3].one;
		Ht[0][4] = H[4].xx;
		Ht[1][4] = H[4].yy;
		Ht[2][4] = H[4].zz;
		Ht[3][4] = H[4].one;
		
		double HtMatr[100][100];
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 5; j++){
				HtMatr[i][j]  = Ht[i][j];
			}
		}
		//TransposeMatrix(H,Ht);
		
		LOG("Original Transpose matr ");
		for (int i = 0; i < 4; i++){
			LOG(AsString(HtMatr[i][0]) << " " << AsString(HtMatr[i][1]) << " " << AsString(HtMatr[i][2]) << " " << AsString(HtMatr[i][3]) << " " << AsString(HtMatr[i][4]));
		}
		
		// Ht*w
		LOG("matr VES pervoe * ");
		for (int i = 0; i < 5; i++){
			LOG(AsString(WMatr[i][0]) << " " << AsString(WMatr[i][1]) << " " << AsString(WMatr[i][2]) << " " << AsString(WMatr[i][3]) << " " << AsString(WMatr[i][4]));
		}
		MatrMultiply(HtMatr, WMatr);
		
		// Ht*w * H
		double HMatr[100][100];
		for (int i = 0; i < 5; i++){
			
				HMatr[i][0]  = H[i].xx;
				HMatr[i][1]  = H[i].yy;
				HMatr[i][2]  = H[i].zz;
				HMatr[i][3]  = H[i].one;
		}
		// Ht * H
		//MatrMultiply(HtMatr, HMatr);
		MatrMultiply(WMatr, HMatr);
		
		double HtHMatr[100][100];
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 4; j++){
				HtHMatr[i][j]  = HMatr[i][j];
			}
		}
		LOG("HtHMatr ");
		for (int i = 0; i < 4; i++){
			LOG(AsString(HtHMatr[i][0]) << " " << AsString(HtHMatr[i][1]) << " " << AsString(HtHMatr[i][2]) << " " << AsString(HtHMatr[i][3]) << " " << AsString(HtHMatr[i][4]));
		}
		
		
		//Находим обратную к HtH матрицу
		// (Ht*w * H)^-1
	
		double revHtH[100][100];
		inverse(HtHMatr, revHtH, 4, det(HtHMatr,4));
		
		// (Ht*w * H)^-1 * Ht
		
		MatrMultiply(revHtH, HtMatr);
		
		// (Ht*w * H)^-1 * Ht * W
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				WMatr[i][j]  = W[i][j];
			}
		}
		LOG("matr VES povtor");
		for (int i = 0; i < 5; i++){
			LOG(AsString(WMatr[i][0]) << " " << AsString(WMatr[i][1]) << " " << AsString(WMatr[i][2]) << " " << AsString(WMatr[i][3]) << " " << AsString(WMatr[i][4]));
		}
		
		MatrMultiply(HtMatr, WMatr);
		
		// (Ht*w * H)^-1 * Ht * W * B
		
		double obsVect[100][100];
		double bVect[100][100];
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < 100; j++){
				if (j == 0 && i < inData.GetCount()){
					obsVect[i][j] = inData[i].psRangLen;
					bVect[i][j] = inData[i].psRangLen;
				}else{
					obsVect[i][j] = 0.;
					bVect[i][j] = 0.;
				}
			}
		}
		
		MatrMultiply(WMatr, obsVect);
		
		LOG("Original matr test output");
		for (int i = 0; i < 4; i++){
			LOG(AsString(HtHMatr[i][0]) << " " << AsString(HtHMatr[i][1]) << " " << AsString(HtHMatr[i][2]) << " " << AsString(HtHMatr[i][3]));
		}
		
		LOG("\nReverce matr test output");
		for (int i = 0; i < 4; i++){
			LOG(AsString(revHtH[i][0]) << " " << AsString(revHtH[i][1]) << " " << AsString(revHtH[i][2]) << " " << AsString(revHtH[i][3]));
		}
	
		//Выведем результат
		LOG("coord test output");
		String outStr = "";
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 1; j++){
				outStr += AsString(obsVect[i][j]);
				outStr += " ";
			}
			outStr += "\n";
		}
		LOG(outStr);
		
		RDUMP(pow(pow(obsVect[0][0], 2) + pow(obsVect[1][0], 2) + pow(obsVect[2][0], 2) , 0.5));
		
		LOG("correction");
		MatrMultiply(HMatr, obsVect);
		
		LOG("A * x");
		outStr = "";
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 1; j++){
				outStr += AsString(obsVect[i][j]);
				outStr += " ";
			}
			outStr += "\n";
		}
		LOG(outStr);
		
		RazMatr(bVect,obsVect);
		LOG("R matrix");
		outStr = "";
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 1; j++){
				outStr += AsString(obsVect[i][j]);
				outStr += " ";
			}
			outStr += "\n";
		}
		LOG(outStr);
		
		W[0][0] = 1./abs(obsVect[0][0]);
		W[1][1] = 1./abs(obsVect[1][0]);
		W[2][2] = 1./abs(obsVect[2][0]);
		W[3][3] = 1./abs(obsVect[3][0]);
		W[4][4] = 1./abs(obsVect[4][0]);
		
		/*
		double W[5][5] = {{1./abs(obsVect[0][0]),0,0,0,0},
							{0,1./abs(obsVect[1][0]),0,0,0},
							{0,0,1./abs(obsVect[2][0]),0,0},
							{0,0,0,1./abs(obsVect[3][0]),0},
							{0,0,0,0,1./abs(obsVect[4][0])}};
		*/
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				WMatr[i][j]  = W[i][j];
			}
		}
		LOG("matr VES");
		for (int i = 0; i < 5; i++){
			LOG(AsString(WMatr[i][0]) << " " << AsString(WMatr[i][1]) << " " << AsString(WMatr[i][2]) << " " << AsString(WMatr[i][3]) << " " << AsString(WMatr[i][4]));
		}
		
		/*
		double w1 = sqrt(sqr(abs(obsVect[0][0])*1000.) - sqr(2830807.42226821));
		double w2 = sqrt(sqr(abs(obsVect[1][0])*1000.) - sqr(2196519.10576065));
		double w3 = sqrt(sqr(abs(obsVect[2][0])*1000.) - sqr(5259097.71206607));
		double w4 = sqrt(sqr(abs(obsVect[3][0])*1000.));
		RDUMP(w1);
		RDUMP(w2);
		RDUMP(w3);
		RDUMP(w4);
		
		
		/*
		lat = 55.9121311
		lon = 37.8090947
		hgt = 160
		
		car.x = 2830807.42226821
		car.y = 2196519.10576065
		car.z = 5259097.71206607
		sqrt(sqr(car.x) + sqr(car.y) + sqr(car.z)) = 6363668.40658742
		*/
	}
	
	return outDat;
}

void HtHPowCalc(Vector <hLine> &H, double HtH[4][4]){
	for (int i = 0; i < H.GetCount(); i++){
		HtH[0][0] += H[i].xx * H[i].xx;
		HtH[0][1] += H[i].xx * H[i].yy;
		HtH[0][2] += H[i].xx * H[i].zz;
		HtH[0][3] += H[i].xx * H[i].one;
		HtH[1][1] += H[i].yy * H[i].yy;
		HtH[2][2] += H[i].zz * H[i].zz;
		HtH[3][3] += H[i].one* H[i].one;
		HtH[1][2] += H[i].yy * H[i].zz;
		HtH[1][3] += H[i].yy * H[i].one;
		HtH[2][3] += H[i].zz * H[i].one;
	}
	HtH[1][0] = HtH[0][1];
	HtH[2][0] = HtH[0][2];
	HtH[2][1] = HtH[1][2];
	HtH[3][0] = HtH[0][3];
	HtH[3][1] = HtH[1][3];
	HtH[3][2] = HtH[2][3];
}

void SqrMatrVectMultiply(float m[100][100], float v[100], int order){
	float resVect[100];
	

	for (int i = 0; i < order; i++){
		resVect[i] = 0;
		for (int j = 0; j < order; j++){
			resVect[i] += m[j][i] * v[i];
		}
	}
	
	LOG("result vect is");
	for (int i = 0; i < order; i++){
		v[i] = resVect[i];
		RDUMP(v[i]);
	}
}

void RazMatr(double a[100][100], double b[100][100]){
	double res[100][100];
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			res[i][j] = 0.;
		}
	}
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
				res[i][j] = a[i][j] - b[i][j];
		}
	}
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			b[i][j] = res[i][j];
		}
	}
}

void MatrMultiply(double a[100][100], double b[100][100]){
	double res[100][100];
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			res[i][j] = 0.;
		}
	}
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			for (int k = 0; k < 100; k++){
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			b[i][j] = res[i][j];
		}
	}
}
