#include <iostream>
#include <string>

using namespace std;

struct Pipe
{
    string name;
    double length = 0;
    int diameter = 0;
    bool underRepair = false;
};

struct CompressorStation
{
    string name;
    int totalWorkshops = 0;
    int workingWorkshops = 0;
    int stationClass = 0;
};

int main()
{
    return 0;
}