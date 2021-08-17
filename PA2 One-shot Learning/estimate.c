/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

double ** transpose(int rows, int cols, double **matrix) {
	int i, j;

	// creates temp
	double **temp = malloc(cols * sizeof(double *));
	for(i = 0; i < cols; i++) {
		temp[i] = malloc(rows * sizeof(double));
	}

	// finds the transpose
	for(i = 0; i < cols; i++) {
		for(j = 0; j < rows; j++) {
			temp[i][j] = matrix[j][i];
		}
	}

	return temp;
}

double **multiply(int rowsA, int colsA, int rowsB, int colsB, double **matrixA, double **matrixB) {
	int i,j,k;
	double count;

	//creates temp
	double **temp = malloc(rowsA * sizeof(double *));
	for(i = 0; i < rowsA; i++) {
		temp[i] = malloc(colsB * sizeof(double));
	}

	// multiplication
	for(i = 0; i < rowsA; i++) {
		for(j = 0; j < colsB; j ++) {
			count = 0;
			for(k = 0; k < colsA; k++) {
				count += matrixA[i][k] * matrixB[k][j];
			}

			temp[i][j] = count;

		}
	}

	return temp;
	
}

double ** inverse(int rows, int cols, double **matrixM) {
	
	
	// create a identity matrix
	int n = rows;

	// allocates space
	double **identityMatrix = malloc(sizeof(double *) * n);
	int i, j, k, p;
	double f = 0;
	for(i = 0; i < n; i++) {
		identityMatrix[i] = malloc(sizeof(double) * n);
	}	


	// filling identity matrix
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			if(i == j) {
				identityMatrix[i][j] = 1;
				continue;
			}
			identityMatrix[i][j] = 0;
		}
	}


	//matrixM
	// identityMatrix
	for(p = 0; p < n; p++) {
		f = matrixM[p][p];

		// dive Mp by f
		for(i = 0; i < n; i++) {
			matrixM[p][i] = matrixM[p][i] / f;
			identityMatrix[p][i] = identityMatrix[p][i] / f;
		}

		for(i = p+1; i < n; i++) {
			f = matrixM[i][p];

			// subtract Mp x f from matrix
			for(k = 0; k < n; k++) {
				matrixM[i][k] = matrixM[i][k] - (matrixM[p][k] * f);
				identityMatrix[i][k] = identityMatrix[i][k] - (identityMatrix[p][k] * f);
			}
		}
	}

	for(p = n-1; p >= 0; p--) {
		for(i = p-1; i >= 0; i--) {
			 f = matrixM[i][p];

			 // subtract Mp x f from Mi
			for(j = 0; j < n; j++) {
				matrixM[i][j] = matrixM[i][j] - (matrixM[p][j] * f);
				identityMatrix[i][j] = identityMatrix[i][j] - (identityMatrix[p][j] * f);
			}
		}
	}

	return identityMatrix;


}
	
	


int main(int argc, char **argv)
{
	
	// train.txt 
	FILE *new_file = fopen(argv[1], "r");

	// reads the train 
	fscanf(new_file, "train");

	// gets the attribute 
	int attribute = 0; //columns + 1
	int houseSets = 0; //rows

	fscanf(new_file, " %d%d", &attribute, &houseSets);
	
	
	//construct matrix ... columns of matrix are k + 1, rows are houseSets
	double **trainMatrixX = malloc(sizeof(double *) * houseSets);
	double **trainMatrixY = malloc(sizeof(double *) * houseSets);
	int i, k;
	double temp = 0;
	for(i = 0; i < houseSets; i++) {
		trainMatrixX[i] = malloc(sizeof(double) * (attribute + 1));
		trainMatrixY[i] = malloc(sizeof(double));
	}


	// fills up X and Y for the appropiate values
	for(i = 0; i < houseSets; i++) {
		for(k = 0; k < attribute + 2; k++) {
			if(k == 0) {
				trainMatrixX[i][0] = 1;
				continue;
			} 

			fscanf(new_file, "%lf", &temp);
			if(k == attribute + 1) {
				trainMatrixY[i][0] = temp;
			} else {
				trainMatrixX[i][k] = temp;
			}

		}
	}

	// data.txt
	FILE *data_file = fopen(argv[2], "r");

	// reads data
	fscanf(data_file, "data");
	int dataAttributes = 0;  // dataAttributes + 1 because adding in the column at 0
	int dataHouseSets = 0; // rows

	fscanf(data_file, "%d%d", &dataAttributes, &dataHouseSets);

	// construct matrix time
	double **dataMatrix = malloc(sizeof(double *) * dataHouseSets);
	for(i = 0; i < dataHouseSets; i++) {
		dataMatrix[i] = malloc(sizeof(double) * (dataAttributes + 1));
	}

	for(i = 0; i < dataHouseSets; i++) {
		for(k = 0; k < dataAttributes + 1; k++) {
				if(k == 0) {
					dataMatrix[i][0] = 1;
					continue;
				} 

				fscanf(data_file, "%lf", &temp);
				dataMatrix[i][k] = temp;
		}
	}

	/*
	X = trainMatrixX, rows = houseSets, columns = attribute + 1.
	Y = trainMatrixY, rows = houseSets, columns = 1 (index 0) 
	*/
	
	// (X^t * XW)^-1 * X^T * Y


	// x^t ... rows = attribute + 1, cols = housesets 
	double **transposeOfX = transpose(houseSets, attribute + 1, trainMatrixX);

	// X ... trainMatrixX rows = houseSets, cols = attribute + 1

	// X^t * X row = attribute + 1, cols = attribute + 1
	double **xtx = multiply(attribute + 1, houseSets, houseSets, attribute + 1, transposeOfX, trainMatrixX);

	// inverse of xtx
	double **inversed = inverse(attribute + 1, attribute + 1, xtx);

	// xt * y rows = attribute +1, cols = 1
	double **xty = multiply(attribute +1, houseSets, houseSets, 1, transposeOfX, trainMatrixY);

	// w is rows = attribute + 1, cols: 1
	double **w = multiply(attribute +1, attribute +1, attribute + 1, 1, inversed, xty);
	

	double **yprime = multiply(dataHouseSets, dataAttributes + 1, attribute + 1, 1, dataMatrix, w);
	


	for(i = 0; i < dataHouseSets; i++) {
		for(k = 0; k < 1; k++) {
			printf("%.0f", yprime[i][k]);

		}

		printf("\n");
	}
	



	
	return 0;
}