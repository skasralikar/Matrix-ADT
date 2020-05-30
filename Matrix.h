//skasrali
//pa2
//Matrix.h
//matrix module

#ifndef _MATRIX_H_INCLUDED
#define _MATRIX_H_INCLUDED
#include <stdio.h>

//Defines pointer to List object
typedef struct MatrixObj* Matrix;

//newMatrix()
Matrix newMatrix(int n);

//freeMatrix()
void freeMatrix(Matrix* pM);

//Access functions

//size()
int mGetSize(Matrix M);

//NNZ
//returns number of non-zero elements in Matrix M
int NNZ(Matrix M);

//equals
int equals(Matrix A, Matrix B);

//Manipulation procedures

//makeZero()
void makeZero(Matrix M);

//changeEntry()
void changeEntry(Matrix M, int i, int j, double x);

//Matrix Arithmetic operations

//copy()
Matrix copy(Matrix A);

//transpose()
Matrix transpose(Matrix A);

//scalarMult()
Matrix scalarMult(double x, Matrix A);

//sum()
Matrix sum(Matrix A, Matrix B);

//diff()
Matrix diff(Matrix A, Matrix B);

//product
Matrix product(Matrix A, Matrix B);

//printMatrix()
void printMatrix(FILE* out, Matrix M);

#endif
