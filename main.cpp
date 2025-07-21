#include "include/MonteCarlo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <filesystem>
#include <thread>
#include <functional>

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

vector<vector<dataFormat>> readData(char *fileName, const int NumberOfThreads);

void getAns(vector<vector<dataFormat>> &data, int id, const int N);

int main(int argc, char *argv[])
{
    // if compaling with CMake
    CHDIR("../");

    const int N = 1'000'000;
    const int numberOfThreads = 6;

    // argv contains file name
    vector<vector<dataFormat>> optionData = readData(argv[1], numberOfThreads);

    // creating out directory

    filesystem::path dirPath = "out";
    if (filesystem::create_directory(dirPath))
        cout << "Directory created successfully \n";
    else
        cout << "Directory already exist \n";

    CHDIR("out/");

    // creating file names
    map<string, int> fileNames;

    for (int i = 0; i < numberOfThreads; i++)
    {
        for (auto &el : optionData[i])
        {

            fileNames[el.optionName]++;
            el.optionName += to_string(fileNames[el.optionName]);
        }
    }
    // creating multithreading
    vector<thread> threads;

    for (int i = 0; i < numberOfThreads; i++)
        threads.emplace_back(getAns, ref(optionData), i, N);

    for (auto &t : threads)
        t.join();

    return 0;
}

vector<vector<dataFormat>> readData(char *fileName, const int NumberOfThreads)
{
    ifstream file(fileName);
    // ifstream file("data.in");

    vector<vector<dataFormat>> data(NumberOfThreads);
    const char pattern = ' ';
    string row;
    int cnt = 0;
    while (getline(file, row))
    {
        dataFormat dataRow;
        // split data
        vector<string> result;
        string tmp;
        for (long unsigned int i = 0; i < row.size(); i++)
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

        for (long unsigned int i = 1; i < result.size() - 1; i++)
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

        data[cnt].emplace_back(dataRow);
        cnt++;
        cnt %= NumberOfThreads;
    }

    file.close();

    return data;
}

void getAns(vector<vector<dataFormat>> &data, int id, const int N)
{
    OptionPricing optionPricing;

    for (auto el : data[id])
    {
        optionPricing.monteCarlo(el.optionType, el.stockData[0], el.stockData[1], el.stockData[2], el.stockData[3], el.stockData[4], N, el.optionName + ".out");
    }
}