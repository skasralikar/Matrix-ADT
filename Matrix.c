#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"

struct Entry {
   int column;
   double data;
};

struct MatrixObj {
   List *row;
   int numRows;
   int nnz;
};

static char tempString[80];

struct Entry* newEntry(int column1, double data1) {
   struct Entry *entry = malloc(sizeof(struct Entry)); 
   entry->column = column1;
   entry->data = data1;
   return entry;
 }

char* entryToString(struct Entry *entry) {
   sprintf(tempString, " %d,  %f ", entry->column, entry->data);
   return tempString;
}

int entryEquals(struct Entry *this, struct Entry *x) {
   int equal = 0;
   equal = (this->column == x->column && this->data == x->data);
   return equal;
}

Matrix newMatrix(int n) 
{
   Matrix M;
   if (n < 1) {
       printf("number of entries < 1\n");
       return NULL;
   }
   // allocate list of rows
   M = malloc(n * sizeof(List));
   // now allocate space for all rows
   for(int i=0; i<n; i++) {
      M->row = malloc(sizeof(List));
   }
   return M;
}

void freeMatrix(Matrix *pM) 
{
   Matrix M = *pM;

   // free each row list 
   for(int i=0; i<mGetSize(M); i++) {
      if (M->row) 
          freeList(M->row);
   }
   free(M);
}

void printMatrix(FILE *out, Matrix M) {
   for(int i = 0; i < mGetSize(M); i++) {
      if(length(M->row[i]) > 0)
      {
         printf("%d", i);
         printList(out, M->row[i]);
      }
   }
}

int mGetSize(Matrix M) {
   return M->numRows;
}
   
int NNZ(Matrix M) {
   int NNZ = 0;
   for(int i = 0; i < mGetSize(M); i++) {
      NNZ += length(M->row[i]);
   }
   return NNZ;
}
   
int equals(Matrix A, Matrix B) {
  if (mGetSize(A) != mGetSize(B)) {
     return 0;
  }
  if (NNZ(A) != NNZ(B)) {
      return 0;
  }
  for(int i = 0; i < mGetSize(A); i++) {
      if(!(equal(A->row[i], B->row[i]))) {
         return 0;
      }
   }
   return 1;
}
   
void makeZero(Matrix M) {
   for(int i = 0; i < mGetSize(M); i++) {
      clear(M->row[i]);
   }
}

List mAddSub(List P, List Q, int go) {
   List list = newList();
   moveFront(P);
   moveFront(Q);

   double ret;
   if(go) {
   	ret = 1.0;
   }
   else {
   	ret = -1.0;
   }

   while(index(P) >= 0 || index(P) >= 0) {
      if(index(P) >= 0 && index(Q) >= 0) {
         struct Entry a = *(struct Entry *) get(P);
         struct Entry b = *(struct Entry *) get(Q);
         if(a.column > b.column) {
         	append(list, newEntry(b.column, (ret) * b.data));
            moveNext(Q);
         } else if(a.column < b.column) {
         	append(list, newEntry(a.column, a.data));
            moveNext(P);
         } else if(a.column == b.column) {
            if((go && a.data + b.data != 0) || (!go && a.data - b.data != 0)) {
         	   if(go) {
         		   ret = a.data + b.data;
         	   }
         	   else {
         		   ret = a.data - b.data;
         	   }
            }
            append(list, newEntry(a.column, (ret)));
            moveNext(P);
            moveNext(Q);
         }
      } else if(index(P) >= 0) {
         struct Entry a = *(struct Entry *) get(P);
         append(list, newEntry(a.column, a.data));
         moveNext(P);
      } else {
         struct Entry b = *(struct Entry *) get(Q);
         append(list, newEntry(b.column, (ret) * b.data));
         moveNext(Q);
      }
   }
   return list;
}

// computes the vector dot product of two matrix rows represented by Lists P and Q
static double vectorDot(List P, List Q) {
   double product = 0.0;
   moveFront(P);
   moveFront(Q);
   while(index(P) >= 0 && index(Q) >= 0) 
   {
      struct Entry entry1 = *(struct Entry *) get(P);
      struct Entry entry2 = *(struct Entry *) get(Q);
      if(entry1.column > entry2.column) {
         moveNext(Q);
      } 
      else if(entry1.column < entry2.column) {
         moveNext(P);
      } 
      else 
      {
         product += (entry1.data * entry2.data);
         moveNext(P);
         moveNext(Q);
      }
   }
   return product;
}

void changeEntry(Matrix M, int i, int j, double x) {
   int found = 0;
   if(i < 1 || i > mGetSize(M) || j < 1 || j > mGetSize(M)) {
       printf("error 1\n");
   }
   moveFront(M->row[i]);
   while(index(M->row[i]) >= 0) {
      struct Entry entry = *(struct Entry *)get(M->row[i]);
      if(entry.column == j) {
     	   found = 1;
      }
      else {
     	   found = 0;
      }
      if(found) {
         if(x == 0.0) {
            delete(M->row[i]);
            return;
         } 
         else { 
            entry.data = x; 
            return;
         }
      }
      moveNext(M->row[i]);
   }

   // insert a new entry
   if(!found && x != 0.0) 
   {
      moveFront(M->row[i]);
      if(index(M->row[i]) == -1) {
         append(M->row[i], newEntry(j, x)); 
         return;
      } 
      else {
         while(index(M->row[i]) > -1 && (*(struct Entry *)get(M->row[i])).column < j) {
            moveNext(M->row[i]);
         }
         if(index(M->row[i]) > -1) {
            insertBefore(M->row[i], newEntry(j, x)); 
            return;
         } 
         else {
            append(M->row[i], newEntry(j, x)); 
            return;
         }
      }
   }
}

Matrix copy(Matrix A) {
   Matrix matrix = newMatrix(mGetSize(A));
   for(int i = 0; i <= mGetSize(A); i++) {
      moveFront(A->row[i]);
      while(index(A->row[i]) >= 0) {
         struct Entry temp = *(struct Entry *) get(A->row[i]);
         changeEntry(matrix, i, temp.column, temp.data);
         moveNext(A->row[i]);
      }
   }
   return matrix;
}

Matrix transpose(Matrix A)
{
   Matrix matrix = newMatrix(mGetSize(A));
   for(int i = 0; i < mGetSize(A); i++) {
      moveFront(A->row[i]);
      while(index(A->row[i]) >= 0) {
         changeEntry(matrix,(*(struct Entry *)get(A->row[i])).column, i, (*(struct Entry *)get(A->row[i])).data);
         moveNext(A->row[i]);
      }
   }
   return matrix;
}

Matrix scalarMult(double x, Matrix A) {
   Matrix matrix = copy(A);
   for(int i = 0; i < mGetSize(A); i++) {
      moveFront(matrix->row[i]);
      while(index(matrix->row[i]) >= 0) {
         struct Entry temp = *(struct Entry *)get(matrix->row[i]);
         changeEntry(matrix, i, temp.column, (x * temp.data));
         moveNext(matrix->row[i]);
      }
   }
   return matrix;
}

Matrix sum(Matrix A, Matrix B) {
   if(mGetSize(A) != mGetSize(B)) {
      printf("Matrix: add() called on Matrix\n");
   }
   if(B == A) {
      return copy(scalarMult(2, A));
   }
   Matrix matrix = newMatrix(mGetSize(A));
   for(int i = 0; i < mGetSize(A); i++) {
       matrix->row[i] = mAddSub(A->row[i], B->row[i], 1);
   }
   return matrix;
}

Matrix diff(Matrix A, Matrix B) {
   if(mGetSize(A) != mGetSize(B)) {
      printf("Matrix: sub() called on Matrix\n");
   }
   if(B == A) {
      return newMatrix(mGetSize(A));
   }
   Matrix matrix = newMatrix(mGetSize(A));
   for(int i = 0; i < mGetSize(A); i++) {
       matrix->row[i] = mAddSub(A->row[i], B->row[i], 0);
   }
   return matrix;
}

Matrix product(Matrix A, Matrix B) {
   if(mGetSize(A) != mGetSize(B)) {
      printf("Matrix: mult() called on Matrix\n");
   }
   Matrix matrix = newMatrix(mGetSize(A));
   Matrix temp = transpose(B);
   for(int i = 0; i < mGetSize(temp); i++) {
      if(length(temp->row[i]) == 0) continue;
      for(int j = 0; j < mGetSize(temp); j++) {
         if(length(temp->row[j]) == 0) continue;
         changeEntry(matrix, i, j, vectorDot(B->row[i], temp->row[j]));
      }
   }
   return matrix;   
}
