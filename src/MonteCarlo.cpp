#include "../include/MonteCarlo.h"
#include <string>
#include <cmath>

using namespace std;

double normalCDF(double x)
{
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

double blackScholes(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
                    const double &riskFreeRate, const double &volatility)
{

    double d1 = (log(currentStockPrice / strikePrice) + (riskFreeRate + 0.5 * pow(volatility, 2))) * timeToMaturity / (volatility * sqrt(timeToMaturity));
    double d2 = d1 - (volatility * sqrt(timeToMaturity));

    double price = 0.0;

    if (type == CALL)
        price = currentStockPrice * normalCDF(d1) - strikePrice * exp(-riskFreeRate * timeToMaturity) * normalCDF(d2);

    else if (type == PUT)
        price = strikePrice * exp(-riskFreeRate * timeToMaturity) * normalCDF(-d2) - currentStockPrice * normalCDF(-d1);

    return price;
}