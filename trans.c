/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Luke Sorvik, Timofei Kachan 
 * NetID(s): lukesorv, timofeik
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
char transpose_submit_desc[] = "Transpose submission"; // do not change
void transpose_submit(int M, int N, int A[M][N], int B[N][M])
{

    // block size for cache = 32 or 8 ints per block
    // E = 1
    // 32 sets
    // so cache can hold 32 x 8 ints or 256 ints at a time
    // M =32, N =32 = 1024 ints in each array
    // 1024 ints /4 blocks = 256 ints per block

    int i, j, tmp, bsize, col, row, d;

    if (M == 32 && N == 32)
    {
        bsize = 8; // derived from n=4r for a nxn matrix r = n/4, so for 64 would be 16
        tmp = -420;
        d = 0;

        for (i = 0; i < M; i += bsize)
        {
            // iterate through rows

            for (j = 0; j < N; j += bsize)
            {
                // iterate through columns

                for (row = i; row < bsize + i; row++)
                {
                    // iterate through row of block
                    for (col = j; col < bsize + j; col++)
                    {
                        // this will iterate through each column of block
                        // starts at column j to signify a new block, j+= blocksize since it will move 8 ints foward to be a new

                        if (col == row){
                            // if diagonal in the block
                            d = row;           // get the row and col
                            tmp = A[row][col]; // get the current value in diagonal
                        }
                        else
                        {
                            B[col][row] = A[row][col]; // transpose
                        }
                    }

                    // within our row loop
                    //so after reads on row, goes to next row
                    if (i == j) {
                    //if the matrix row is equal to the matrix column(a true diagonal)
                        B[d][d] = tmp; // set to the last diagnonal found when iterating colums
                        //we do this after iterating through the columns so we dont replace the block in cache we were working with 
                        //to write new diagonal
                        /*ex of problem
                         A(1,1) Ablock1 in cache, b11 = A11 we put Bblock1 in cache but conflict with ablock1 so we replace 
                        Ablock1 with Bblock1 (miss), A12 Ablock 1 no longer in cash so miss

                        fix:
                        when see A11 instead of writing to B11 we copy the value of A11 to tmp,
                        then after A row 1 has been iterated through, if the matrix row == matrix col add the diagonal to the block,
                        but now since we are no longer using A row 1 we avoid a uneccessary cache miss 

                        */
                    }
                }
            }
        }
    }

    // if 64x64
    else
    {
        bsize = 4; // makes 8 by 8 blocks to scan through the matrixes
        tmp = -420;
        d = 0;

        for (i = 0; i < M; i += bsize)
        {
            // iterate through rows

            for (j = 0; j < N; j += bsize)
            {
                // iterate through columns

                for (row = i; row < bsize + i; row++)
                {
                    // iterate through row of block
                    for (col = j; col < bsize + j; col++)
                    {
                        // this will iterate through each column of block
                        // starts at column j to signify a new block, j+= blocksize since it will move 8 ints foward to be a new

                        if (col == row){
                            // if diagonal in the block
                            d = row;           // get the row and col
                            tmp = A[row][col]; // get the current value in diagonal
                        }
                        else
                        {
                            B[col][row] = A[row][col]; // transpose
                        }
                    }

                    // within our row loop
                    //so after reads on row, goes to next row
                    if (i == j) {
                    //if the matrix row is equal to the matrix column(a true diagonal)
                        B[d][d] = tmp; // set to the last diagnonal found when iterating colums
                        //we do this after iterating through the columns so we dont replace the block in cache we were working with 
                        //to write new diagonal
                        /*ex of problem
                         A(1,1) Ablock1 in cache, b11 = A11 we put Bblock1 in cache but conflict with ablock1 so we replace 
                        Ablock1 with Bblock1 (miss), A12 Ablock 1 no longer in cash so miss

                        fix:
                        when see A11 instead of writing to B11 we copy the value of A11 to tmp,
                        then after A row 1 has been iterated through, if the matrix row == matrix col add the diagonal to the block,
                        but now since we are no longer using A row 1 we avoid a uneccessary cache miss 

                        */
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
void trans(int M, int N, int A[M][N], int B[N][M])
{
    int i, j, tmp;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
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
void registerFunctions()
{
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
int is_transpose(int M, int N, int A[M][N], int B[N][M])
{
    int i, j;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
