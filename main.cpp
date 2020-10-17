#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const string SCHEDULE_FILE_NAME = "schedule.csv";

struct Movie {
    string CinemaName;
    string MovieName;
    string Day;
    string StartingTime;
    string FinishingTime;
    int Price;
    int Hall;
};

vector<string> ReadHeader(string Line){
    vector<string> Header;
    string ColName;
    stringstream SStream(Line);
    while(getline(SStream,ColName,',')){
        Header.push_back(ColName);
    }
    return Header;
}

void ReadCSVFile() {
    vector<Movie> Movies;
    ifstream File;
    File.open(SCHEDULE_FILE_NAME);
    string Line;
    getline(File,Line);
    vector<string> Header=ReadHeader(Line);

    for(auto ext:Header){
        cout<<ext<<" ";
    }

    File.close();
}

int main() {
    ReadCSVFile();

    return 0;
}