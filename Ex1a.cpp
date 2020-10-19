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

    double cum_mean = 0;
    double cum_square = 0;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    //std::normal_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < nrolls; ++i) {
        double number = distribution(generator);
        cum_mean += number;
        cum_square += number * number;


    }
    cum_mean = cum_mean / nrolls;
    cum_square = cum_square / nrolls - cum_mean * cum_mean;

    std::cout << "uniform_distribution (0.0,1.0):" << std::endl;
    std::cout << std::fixed; std::cout.precision(3);
    std::cout << "mean:" << std::endl;
    std::cout << cum_mean << std::endl;
    std::cout << "variance:" << std::endl;
    std::cout << cum_square << std::endl;

    return 0;
}


/*
Output:
normal_distribution (0.0,1.0):
mean:
0.001
variance:
0.999

uniform_distribution (0.0,1.0):
mean:
0.500
variance:
0.083

*/