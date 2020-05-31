//skasrali
//pa2
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"
#include "Matrix.h"

#define MAX_LEN 200

int main(int argc, char * argv[]){
	int n, count = 0;
	FILE *in;
	FILE *out;

	char line[MAX_LEN];

	if(argc != 3){
		printf("Usage: %s <input file><output file>\n", argv[0]);
		exit(1);
	}

	//open files for reading and writing
	in  = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	if(in == NULL){
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	if(out == NULL){
		printf("Unable to open file %s for writing\n", argv[2]);
		exit(1);
	}
	
	int rows=0;
	int nnzA=0;
	int nnzB=0;


	fscanf("%d %d %d", &rows, &nnzA, &nnzB);

	Matrix A = newMatrix(rows);
	Matrix B = newMatrix(rows);

	while(i < nnzA) {
		fscanf("%d %d %lf", row_num, col_num, data);
		mChangeEntry(A, row_num, col_num, data);
	}

	while(i < nnzB) {
		fscanf("%d %d %lf", row_num, col_num, data);
		mChangeEntry(A, row_num, col_num, data);
	}

	fclose(in);
	fclose(out);

	return(0);
}

