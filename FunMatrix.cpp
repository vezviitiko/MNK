#include "FunMatrix.h"

//-----------------------------------------------------
// show matrix : printf() in c
void printf(double a[100][100],int n,int show){
	int i,j;
	if(show == 1)
		for(i=0;i < n;i++){
			for(j=0;j < n;j++)
				RDUMP(a[i][j]);
		}
	else if(show == 2){
		LOG("\n\n The Inverse Of Matrix Is : \n\n");
		for (i=0;i<n;i++){
			for (j=0;j<n;j++)
				RDUMP(a[i][j]);
		}
	}
}

//---------------------------------------------------
//	calculate minor of matrix OR build new matrix : k-had = minor
void minor_f(double b[100][100],double a[100][100],int i,int n){
	int h=0;
	int k=0;
	for(int lg=1; lg<n; lg++){
		for(int j=0; j<n; j++){
			if(j == i)
			{
				continue;
			}
			b[h][k] = a[lg][j];
			k++;
			if(k == (n-1)){
				h++;
				k=0;
			}
		}
	}
}// end function

//---------------------------------------------------
//	calculate determinte of matrix
double det(double a[100][100],int n){
	int i;
	double b[100][100],sum=0;
	if (n == 1)
	{
		return a[0][0];
	}
	else if(n == 2)
	{
		return (a[0][0]*a[1][1]-a[0][1]*a[1][0]);
	}
	else {
		for(i=0;i<n;i++){
			minor_f(b,a,i,n);	// read function
			sum = (double) (sum+a[0][i]*pow(-1,i)*det(b,(n-1)));	// read function	// sum = determinte matrix
		}
	}
	return sum;
}// end function

//---------------------------------------------------
//	calculate transpose of matrix
void transpose(double c[100][100],double d[100][100],int n,double det){
	int i,j;
	double b[100][100];
	for (i=0;i<n;i++)
		for (j=0;j<n;j++)
			b[i][j] = c[j][i];
	for (i=0;i<n;i++)
		for (j=0;j<n;j++)
			d[i][j] = b[i][j]/det;	// array d[][] = inverse matrix
}// end function

//---------------------------------------------------
//	calculate cofactor of matrix
void cofactor(double a[100][100],double d[100][100],int n,double determinte){
	double b[100][100],c[100][100];
	int l,h,m,k,i,j;
	for (h=0;h<n;h++)
		for (l=0;l<n;l++){
			m=0;
			k=0;
			for (i=0;i<n;i++)
				for (j=0;j<n;j++)
					if (i != h && j != l){
						b[m][k]=a[i][j];
						if (k<(n-2))
							k++;
						else{
							k=0;
							m++;
						}
					}
			c[h][l] = (double) pow(-1,(h+l))*det(b,(n-1));	// c = cofactor Matrix
		}
	transpose(c,d,n,determinte);	// read function
}// end function

//---------------------------------------------------
//	calculate inverse of matrix
void inverse(double a[100][100],double d[100][100],int n,double det){
	if(det == 0)
		LOG("\nInverse of Entered Matrix is not possible\n");
	else if(n == 1)
		d[0][0] = 1;
	else
		cofactor(a,d,n,det);	// read function
}// end function
