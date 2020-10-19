// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// uniform_real_distribution
#include <iostream>
#include <random>
#include <math.h>
#include <vector>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>


int main()
{
    const int nrolls = int(pow(10, 6));  // number of experiments

    double cum_mean[] = { 0,0 };
    double cum_square[2][2] = { {0,0},{0,0} };
    double value;
    double cov[] = { 4.0, 1.0, 1.0, 4.0 };
    double d[] = { 0,0,0,0 };

    gsl_matrix_view COV = gsl_matrix_view_array(cov, 2, 2);
    gsl_matrix_view L = COV;


    gsl_matrix* samples = gsl_matrix_alloc(2, nrolls);
    gsl_matrix* C = gsl_matrix_calloc(2, nrolls);
    gsl_matrix* dum = gsl_matrix_calloc(2, 2);
    gsl_permutation* p = gsl_permutation_calloc(2);


    gsl_linalg_pcholesky_decomp(&L.matrix, p);
    d[0] = sqrt(gsl_matrix_get(&L.matrix, 0, 0));
    d[3] = sqrt(gsl_matrix_get(&L.matrix, 1, 1));
    gsl_matrix_view D = gsl_matrix_view_array(d, 2, 2);



    gsl_matrix_set(&L.matrix, 0, 0, 1);
    gsl_matrix_set(&L.matrix, 1, 1, 1);
    gsl_matrix_set(&L.matrix, 0, 1, 0);




    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &L.matrix, &D.matrix, 0.0, dum);

    L.matrix = *dum;
    //    std::cout << "L = " << std::endl;
    //    gsl_matrix_fprintf(stdout, &L.matrix, "%g");


        //double cum_var;
        //v.reserve(nrolls);

    std::default_random_engine generator;
    //std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::normal_distribution<double> distribution(0.0, 1.0);


    for (int i = 0; i < nrolls; ++i) {
        double number = distribution(generator);
        //cum_mean += number;
        //cum_square += number * number;
        gsl_matrix_set(samples, 0, i, number);
        number = distribution(generator);
        gsl_matrix_set(samples, 1, i, number);

    }

    //cum_mean = cum_mean / nrolls;
    //cum_square = cum_square / nrolls - cum_mean * cum_mean;
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &L.matrix, samples, 0.0, C);


    samples = C;
    //std::cout << "samples = " << std::endl;
    //gsl_matrix_fprintf(stdout, L, "%g");
    /*
    std::cout << "uniform_real_distribution (0.0,1.0):" << std::endl;
    std::cout << std::fixed; std::cout.precision(3);
    std::cout << cum_mean << std::endl;
    std::cout << cum_square << std::endl;
    */

    for (int i = 0; i < nrolls; ++i) {
        cum_mean[0] = cum_mean[0] + gsl_matrix_get(samples, 0, i);
        cum_mean[1] = cum_mean[1] + gsl_matrix_get(samples, 0, i);
        cum_square[0][0] = cum_square[0][0] + gsl_matrix_get(samples, 0, i) * gsl_matrix_get(samples, 0, i);
        cum_square[0][1] = cum_square[0][1] + gsl_matrix_get(samples, 0, i) * gsl_matrix_get(samples, 1, i);
        cum_square[1][1] = cum_square[1][1] + gsl_matrix_get(samples, 1, i) * gsl_matrix_get(samples, 1, i);
    }
    //gsl_matrix_get_row(gsl_vector * v, , size_t i)

    cum_mean[0] = cum_mean[0] / nrolls;
    cum_mean[1] = cum_mean[0] / nrolls;

    value = cum_square[0][0];
    cum_square[0][0] = value / nrolls - cum_mean[0] * cum_mean[0];
    value = cum_square[1][1];
    cum_square[1][1] = value / nrolls - cum_mean[1] * cum_mean[1];
    value = cum_square[0][1];
    cum_square[0][1] = value / nrolls - cum_mean[0] * cum_mean[1];


    cum_square[1][0] = cum_square[0][1];

    std::cout << "means:" << std::endl;
    std::cout << cum_mean[0] << std::endl;
    std::cout << cum_mean[1] << std::endl;
    std::cout << "Cov:" << std::endl;
    std::cout << cum_square[0][0] << std::endl;
    std::cout << cum_square[0][1] << std::endl;
    std::cout << cum_square[1][0] << std::endl;
    std::cout << cum_square[1][1] << std::endl;

    return 0;
}

/*
means:
-0.000214197
-2.14197e-10
Cov:
3.99827
0.998343
0.998343
3.98981

*/