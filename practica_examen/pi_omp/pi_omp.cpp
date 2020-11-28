#include <iostream>
#include <omp.h>
using namespace std;

double calculatePI(int n)
{
    double factor = 1.0;
    double sum = 0.0;
    double pi = 0.0;
    int k;
    #pragma omp parallel for default(none) private(k,factor) shared(n) reduction(+:sum) 
    for (k = 0; k < n; ++k)
    {
        if(k%2==0)
        {
            factor =  1.0;
        }else
        {
            factor = -1.0;
        }
        sum += factor / (2 * k + 1);

    }
    pi = 4.0 * sum;
    return pi;
}

int main(int argc, char *argv[])
{
    int iterations = 0;
    if (argc >= 2 && argc < 3) 
    {
        iterations = (size_t)strtoul(argv[1], NULL, 10);
    }
    else 
    {
        printf("Error, invalid number of parameters\n");
        return 1;
    }
    double pi = calculatePI(iterations);
    printf("pi is approximately %.16f\n", pi);
    return 0;
}