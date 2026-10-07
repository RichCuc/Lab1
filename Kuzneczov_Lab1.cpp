#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <limits>
#include <cstdlib>

using namespace std;

struct Pipe
{
    string name;
    double length;
    int diameter;
    bool repair;
};

struct CompressorStation
{
    string name;
    int workshopCount;
    int workshopsInOperation;
    int stationClass;
};

int readInt(string prompt, string error, int minValue, int maxValue)
{
    int value;
    cout << prompt;

    while (!(cin >> value) || value < minValue || value > maxValue || cin.peek() != '\n')
    {
        if (cin.eof()) exit(0);
        cout << error;
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cin.ignore(10000, '\n');
    return value;
}

double readLength()
{
    double value;
    cout << "Enter pipe length (km): ";

    while (!(cin >> value) || !isfinite(value) || value <= 0 || cin.peek() != '\n')
    {
        if (cin.eof()) exit(0);
        cout << "Wrong value. Enter length again: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cin.ignore(10000, '\n');
    return value;
}

Pipe inputPipe()
{
    Pipe pipe;

    cout << "Enter pipe name: ";
    getline(cin, pipe.name);
    while (pipe.name.find_first_not_of(" \t") == string::npos)
    {
        cout << "Wrong value. Enter pipe name again: ";
        getline(cin, pipe.name);
    }

    pipe.length = readLength();
    pipe.diameter = readInt("Enter pipe diameter (mm): ",
                            "Wrong value. Enter diameter again: ", 1, numeric_limits<int>::max());
    pipe.repair = readInt("Is pipe under repair? (1-y/0-n): ",
                          "Wrong value. Enter 1 or 0: ", 0, 1);
    return pipe;
}

CompressorStation inputStation()
{
    CompressorStation station;

    cout << "Enter station name: ";
    getline(cin, station.name);
    while (station.name.find_first_not_of(" \t") == string::npos)
    {
        cout << "Wrong value. Enter station name again: ";
        getline(cin, station.name);
    }

    station.workshopCount = readInt("Enter number of workshops: ",
                                    "Wrong value. Enter number of workshops again: ",
                                    0, numeric_limits<int>::max());
    station.workshopsInOperation = readInt(
        "Enter number of workshops in operation: ",
        "Wrong value. Enter number of workshops in operation again: ",
        0, station.workshopCount);
    station.stationClass = readInt("Enter station class: ",
                                   "Wrong value. Enter station class again: ",
                                   0, numeric_limits<int>::max());
    return station;
}

void printPipe(Pipe pipe)
{
    cout << "--- Pipe ---" << endl;
    cout << "Name: " << pipe.name << endl;
    cout << "Length: " << pipe.length << " km" << endl;
    cout << "Diameter: " << pipe.diameter << " mm" << endl;
    cout << "Repair: " << (pipe.repair ? "Yes" : "No") << endl;
}

void editPipe(Pipe &pipe)
{
    pipe.repair = !pipe.repair;
}

void printCompressorStation(CompressorStation station)
{
    cout << "--- Compressor Station ---" << endl;
    cout << "Name: " << station.name << endl;
    cout << "Workshops: " << station.workshopCount << endl;
    cout << "Workshops in operation: " << station.workshopsInOperation << endl;
    cout << "Station class: " << station.stationClass << endl;
}

void editStation(CompressorStation &station)
{
    int choice = readInt("1. Start workshop\n2. Stop workshop\nEnter choice: ",
                         "Wrong number. Input new choice: ", 1, 2);
    if (choice == 1)
    {
        if (station.workshopsInOperation < station.workshopCount)
        {
            station.workshopsInOperation++;
            cout << "Workshop started" << endl;
        }
        else
        {
            cout << "All workshops are already working" << endl;
        }
    }
    else
    {
        if (station.workshopsInOperation > 0)
        {
            station.workshopsInOperation--;
            cout << "Workshop stopped" << endl;
        }
        else
        {
            cout << "No working workshops" << endl;
        }
    }
}

void savePipe(ofstream &file, Pipe pipe)
{
    file << pipe.name << endl;
    file << pipe.length << endl;
    file << pipe.diameter << endl;
    file << pipe.repair << endl;
}

void saveStation(ofstream &file, CompressorStation station)
{
    file << station.name << endl;
    file << station.workshopCount << endl;
    file << station.workshopsInOperation << endl;
    file << station.stationClass << endl;
}

void saveData(Pipe pipe, CompressorStation station, bool pipeExists, bool stationExists)
{
    ofstream file("data.txt");
    if (!file)
    {
        cout << "File error" << endl;
        return;
    }
    file << "PIPE_STATION" << endl;
    file << pipeExists << " " << stationExists << endl;
    if (pipeExists) savePipe(file, pipe);
    if (stationExists) saveStation(file, station);
    cout << "Data saved" << endl;
}

bool loadData(Pipe &pipe, CompressorStation &station, bool &pipeExists, bool &stationExists)
{
    ifstream file("data.txt");
    if (!file)
    {
        cout << "File not found" << endl;
        return false;
    }

    Pipe loadedPipe;
    CompressorStation loadedStation;
    bool loadedPipeExists = false;
    bool loadedStationExists = false;
    string firstLine;

    getline(file, firstLine);
    if (firstLine == "PIPE_STATION")
    {
        int pipeFlag;
        int stationFlag;
        if (!(file >> pipeFlag >> stationFlag) ||
            (pipeFlag != 0 && pipeFlag != 1) ||
            (stationFlag != 0 && stationFlag != 1) ||
            (pipeFlag == 0 && stationFlag == 0))
        {
            cout << "File data error" << endl;
            return false;
        }
        loadedPipeExists = pipeFlag == 1;
        loadedStationExists = stationFlag == 1;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else
    {
        loadedPipeExists = true;
        loadedStationExists = true;
        file.clear();
        file.seekg(0);
    }

    if (loadedPipeExists)
    {
        getline(file, loadedPipe.name);
        file >> loadedPipe.length >> loadedPipe.diameter >> loadedPipe.repair;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (loadedStationExists)
    {
        getline(file, loadedStation.name);
        file >> loadedStation.workshopCount >> loadedStation.workshopsInOperation
             >> loadedStation.stationClass;
    }

    if (!file ||
        (loadedPipeExists &&
         (loadedPipe.name.empty() || !isfinite(loadedPipe.length) ||
          loadedPipe.length <= 0 || loadedPipe.diameter <= 0)) ||
        (loadedStationExists &&
         (loadedStation.name.empty() || loadedStation.workshopCount < 0 ||
          loadedStation.workshopsInOperation < 0 ||
          loadedStation.workshopsInOperation > loadedStation.workshopCount ||
          loadedStation.stationClass < 0)))
    {
        cout << "File data error" << endl;
        return false;
    }
    pipe = loadedPipe;
    station = loadedStation;
    pipeExists = loadedPipeExists;
    stationExists = loadedStationExists;
    cout << "Data loaded" << endl;
    return true;
}

int main()
{
    Pipe pipe;
    CompressorStation station;
    bool pipeExists = false;
    bool stationExists = false;
    int command = -1;

    while (command != 0)
    {
        cout << endl;
        command = readInt(
            "1. Add pipe\n2. Add compressor station\n3. View all objects\n"
            "4. Edit pipe status\n5. Edit compressor station\n6. Save\n"
            "7. Load\n0. Exit\nEnter command: ",
            "Wrong command. Enter number from 0 to 7: ", 0, 7);

        switch (command)
        {
            case 1:
                pipe = inputPipe();
                pipeExists = true;
                break;
            case 2:
                station = inputStation();
                stationExists = true;
                break;
            case 3:
                if (pipeExists) printPipe(pipe);
                else cout << "Pipe not added" << endl;
                cout << endl;
                if (stationExists) printCompressorStation(station);
                else cout << "Station not added" << endl;
                break;
            case 4:
                if (pipeExists)
                {
                    editPipe(pipe);
                    cout << "Pipe repair status changed" << endl;
                }
                else cout << "Pipe not added" << endl;
                break;
            case 5:
                if (stationExists) editStation(station);
                else cout << "Station not added" << endl;
                break;
            case 6:
                if (pipeExists || stationExists)
                    saveData(pipe, station, pipeExists, stationExists);
                else cout << "Add pipe or station first" << endl;
                break;
            case 7:
                loadData(pipe, station, pipeExists, stationExists);
                break;
            case 0:
                cout << "Exit" << endl;
                break;
        }
    }
    return 0;
}
