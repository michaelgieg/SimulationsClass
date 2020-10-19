// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// uniform_real_distribution
#include <iostream>
#include <random>
#include <math.h>
#include <vector>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>
#include <cstdio>
#include <ctime>


int main()
{
    std::clock_t start;
    double duration;

    start = std::clock();

    const int nrolls = 677959200;  // number of experiments

    double cum_mean[] = { 0,0 };
    double cum_square[2][2] = { {0,0},{0,0} };
    double value;
    double cov[] = { 4.0, 1.0, 1.0, 4.0 };

    gsl_vector* samples = gsl_vector_calloc(2);
    gsl_vector* y = gsl_vector_calloc(2);


    gsl_matrix_view COV = gsl_matrix_view_array(cov, 2, 2);
    gsl_matrix_view L = COV;

    gsl_linalg_cholesky_decomp1(&L.matrix);
    gsl_matrix_set(&L.matrix, 0, 1, 0);

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);


    for (int i = 0; i < nrolls; ++i) {
        double number = distribution(generator);
        gsl_vector_set(samples, 0, number);
        number = distribution(generator);
        gsl_vector_set(samples, 1, number);

        gsl_blas_dgemv(CblasNoTrans, 1.0, &L.matrix, samples, 0.0, y);
        //gsl_vector_fprintf(stdout, y, "%g");
        duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        if (duration >= 60) {
            std::cout << "Number of Sample Pairs in 60 sec: " << i << '\n';
            break;
        }
    }

    return 0;
}

/*
Output:
Number of Sample Pairs in 60 sec: 60978172

*/
