#include "include/MonteCarlo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#ifdef _WIN32
#include <direct.h>
#define CHDIR _chdir
#else
#include <unistd.h>
#define CHDIR chdir
#endif

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
    // if compaling with CMake
    CHDIR("../");

    const int N = 1'000'000;
    // argv contains file name
    vector<dataFormat> optionData = readData(argv[1]);

    OptionPricing optionPricing;

    map<string, int> fileNames;
    for (auto el : optionData)
    {
        // checking the name of the output file
        if (fileNames[el.optionName] > 0)
            el.optionName += to_string(fileNames[el.optionName]);

        fileNames[el.optionName]++;

        double mean = optionPricing.monteCarlo(el.optionType, el.stockData[0], el.stockData[1], el.stockData[2], el.stockData[3], el.stockData[4], N, el.optionName + ".out");

        cout << mean << "\n";
    }

    return 0;
}

vector<dataFormat> readData(char *fileName)
{
    ifstream file(fileName);
    // ifstream file("data.in");

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

        if (result[0][0] == 'C' || result[0][0] == 'c')
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