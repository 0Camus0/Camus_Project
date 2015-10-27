#include "../Include/Utilities.h"

void MatrixMultiplication(float* matrixA, float* matrixB, float* out)
{
	float temparray[9];
	float val;
	for(int i = 0; i < 3 ; i++ )
	{	
		for (int j = 0; j < 3 ; j++)
		{
			val = (matrixA[(i*3)]*matrixB[(j)])+(matrixA[(i*3)+1]*matrixB[(j+3)])+(matrixA[(i*3)+2]*matrixB[(j+6)]);
			temparray[i+j];
		}
	}

	out = temparray;

}

void MatrixMultiplication4(float* matrixA, float* matrixB, float* out)
{
	float temparray[16];
	float val;
	for(int i = 0; i < 4 ; i++ )
	{	
		for (int j = 0; j < 4 ; j++)
		{
			val = (matrixA[(i*4)]*matrixB[(j)])+(matrixA[(i*4)+1]*matrixB[(j+4)])+(matrixA[(i*4)+2]*matrixB[(j+8)])+(matrixA[(i*4)+3]*matrixB[(j+12)]);
			temparray[i+j];
		}
	}

	out = temparray;
}