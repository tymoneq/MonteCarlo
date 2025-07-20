#ifndef MONTE_CARLO
#define MONTE_CARLO

#include <string>
#include <random>
using namespace std;

enum OptionType
{
    PUT,
    CALL
};

class OptionPricing
{
public:
    OptionPricing();
    ~OptionPricing();
    double genZ();
    double normalCDF(double x);
    double blackScholes(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
                        const double &riskFreeRate, const double &volatility);

    double monteCarlo(const OptionType type, const double &currentStockPrice, const double &strikePrice, const double &timeToMaturity,
                      const double &riskFreeRate, const double &volatility, const int N);

private:
    random_device rd;
    mt19937 gen;
    uniform_real_distribution<double> d;
};
#endif