#include "include/MonteCarlo.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct dataFormat
{
    OptionType optionType;
    // stockPrice, strikePrice, timeToMaturity, riskFreeRate, volatility;
    vector<double> stockData;
    string optionName;
};

vector<dataFormat> readData(char *fileName);

int main(int argc, char *argv[])
{
    // argv contains file name
    vector<dataFormat> optionData = readData(argv[1]);

    // OptionPricing optionPricing;

    // for (int i = 0; i < 5; i++)
    // {
    //     double call = optionPricing.blackScholes(CALL, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma);
    //     double put = optionPricing.blackScholes(PUT, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma);

    //     double monteCarloCall = optionPricing.monteCarlo(CALL, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma, 10'000'000);
    //     double monteCarloPut = optionPricing.monteCarlo(PUT, cases[i].S, cases[i].K, cases[i].T, cases[i].r, cases[i].sigma, 10'000'000);

    //     std::cout << "Test Case " << (i + 1) << ": "
    //               << "Call = " << call << ", Put = " << put << std::endl;

    //     cout << "monteCarloCall: " << monteCarloCall << " " << "monteCarloPut: " << monteCarloPut << "\n";
    // }

    return 0;
}

vector<dataFormat> readData(char *fileName)
{
    ifstream file(fileName);

    vector<dataFormat> data;
    const char pattern = ' ';
    string row;
    while (getline(file, row))
    {
        dataFormat dataRow;
        // split data
        vector<string> result;
        string tmp;
        for (int i = 0; i < row.size(); i++)
        {
            if (row[i] != pattern)
                tmp += row[i];

            else
            {
                result.emplace_back(tmp);
                tmp.clear();
            }
        }
        if (tmp.size() > 0)
        {
            result.emplace_back(tmp);
            tmp.clear();
        }
        // formating input

        if (result[0] == "CALL")
            dataRow.optionType = CALL;
        else
            dataRow.optionType = PUT;

        dataRow.optionName = result.back();

        for (int i = 1; i < result.size() - 1; i++)
        {
            double num = 0.0;
            int power = 1;
            string tmp = result[i];

            for (int j = tmp.size() - 1; j >= 0; j--)
            {
                if (tmp[j] == '.')
                {
                    num /= power;
                    power = 1;
                }
                else
                {
                    num += (tmp[j] - '0') * power;
                    power *= 10;
                }
            }

            dataRow.stockData.emplace_back(num);
        }

        data.emplace_back(dataRow);
    }

    file.close();

    return data;
}