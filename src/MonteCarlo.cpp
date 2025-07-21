#include "../include/MonteCarlo.h"
#include <string>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

OptionPricing::OptionPricing() : gen(rd()), d(0.0, 1.0)
{
}

double OptionPricing::normalCDF(double x)
{
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

double OptionPricing::genZ()
{

    double U1 = this->d(this->gen);
    double U2 = this->d(this->gen);

    return sqrt(-2.0 * log(U1)) * cos(2 * M_PI * U2);
}

double OptionPricing::blackScholes(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
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

double OptionPricing::monteCarlo(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
                                 const double &riskFreeRate, const double &volatility, const int N, const string &outputFile)
{

    double meanPayoff = 0.0;

    ofstream outFile(outputFile);

    for (int i = 0; i < N; i++)
    {
        double Si = currentStockPrice * exp((riskFreeRate - 0.5 * pow(volatility, 2)) * timeToMaturity + volatility * sqrt(timeToMaturity) * genZ());

        if (type == CALL)
            Si = max(Si - strikePrice, 0.0);

        else if (type == PUT)
            Si = max(strikePrice - Si, 0.0);

        meanPayoff += Si;
        outFile << Si << "\n";
    }

    meanPayoff /= N;
    meanPayoff *= exp(-riskFreeRate * timeToMaturity);

    outFile.close();

    return meanPayoff;
}

OptionPricing::~OptionPricing() {}