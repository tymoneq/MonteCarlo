#include "include/MonteCarlo.h"
#include <iostream>

using namespace std;

int main()
{
    struct TestCase
    {
        double S, K, r, sigma, T;
    } cases[] = {
        {100, 100, 0.05, 0.2, 1.0},  // Test 1
        {120, 100, 0.05, 0.2, 1.0},  // Test 2
        {80, 100, 0.05, 0.2, 1.0},   // Test 3
        {100, 100, 0.05, 0.2, 0.25}, // Test 4
        {100, 100, 0.05, 0.01, 1.0}  // Test 5
    };

    for (int i = 0; i < 5; i++)
    {
        double call = blackScholes(CALL, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma);
        double put = blackScholes(PUT, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma);
        std::cout << "Test Case " << (i + 1) << ": "
                  << "Call = " << call << ", Put = " << put << std::endl;
    }

    return 0;
}