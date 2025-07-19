#ifndef MONTE_CARLO
#define MONTE_CARLO

#include <string>
using namespace std;

enum OptionType
{
    PUT,
    CALL
};

double normalCDF(double x);
double blackScholes(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
                    const double &riskFreeRate, const double &volatility);

#endif