/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s):  
 * NetID(s): 
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission"; //do not change
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
  int i, j, ii, jj;
    int bsize = 32;  // Block size
    int temp1, temp2;

    // Copy matrix A to matrix B
    // Loop over each element in A and copy its transpose to B
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            B[j][i] = A[i][j];
        }
    }

    // Transpose the matrix B using blocking
    for (ii = 0; ii < M; ii += bsize) {
        for (jj = 0; jj < N; jj += bsize) {
            // Iterate over each block in B
            for (i = ii; i < ii + bsize && i < M; i++) {
                for (j = jj; j < jj + bsize && j < N; j++) {
                    // Swap non-diagonal elements of the block to achieve the transpose
                    // This helps improve cache locality by accessing elements in a more favorable order
                    if (i != j) {
                        //if they are not the same, ie if we are not in a diagonal
                        temp1 = B[j][i];  // Store B[j][i] in temp1
                        temp2 = B[i][j];  // Store B[i][j] in temp2
                        B[j][i] = temp2;   // Swap B[j][i] with B[i][j]
                        B[i][j] = temp1;   // Swap B[i][j] with B[j][i]
                    }
                }
            }
        }
    }
}



// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
