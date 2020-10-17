#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string SCHEDULE_FILE_NAME = "./schedule.csv";

struct Movie {
    string CinemaName;
    string MovieName;
    string Day;
    string StartingTime;
    string FinishingTime;
    int Price;
    int Hall;
};

vector<string> ReadHeader(string Line) {
    vector<string> Header;
    string ColName;
    stringstream SStream(Line);
    while (getline(SStream, ColName, ',')) {
        Header.push_back(ColName);
    }
    return Header;
}

Movie ReadMovie(vector<string> Header, string Line) {
    string RecordValue;
    Movie TempMovie;
    stringstream SStream(Line);
    for (auto ColName : Header) {
        getline(SStream, RecordValue, ',');
        if (ColName == "CinemaName") TempMovie.CinemaName = RecordValue;
        if (ColName == "MovieName") TempMovie.MovieName = RecordValue;
        if (ColName == "Day") TempMovie.Day = RecordValue;
        if (ColName == "StartingTime") TempMovie.StartingTime = RecordValue;
        if (ColName == "FinishingTime") TempMovie.FinishingTime = RecordValue;
        if (ColName == "Price") TempMovie.Price = stoi(RecordValue);
        if (ColName == "Hall") TempMovie.Hall = stoi(RecordValue);
    }
    return TempMovie;
}

vector<Movie> ReadCSVFile(string FileName) {
    vector<Movie> Movies;
    ifstream File;
    File.open(FileName);
    string Line;
    getline(File, Line);
    vector<string> Header = ReadHeader(Line);
    while (getline(File, Line)) {
        Movies.push_back(ReadMovie(Header, Line));
    }
    File.close();
    return Movies;
}

vector<string> GetMovieNames(const vector<Movie>& Movies) {
    vector<string> MovieNames;
    for (auto Movie : Movies) MovieNames.push_back(Movie.MovieName);
    return MovieNames;
}

void PrintMovies(const vector<Movie>& Movies) {
    vector<string> MovieNames = GetMovieNames(Movies);
    sort(MovieNames.begin(), MovieNames.end());
    // remove duplicated values
    MovieNames.erase(unique(MovieNames.begin(), MovieNames.end()),
    MovieNames.end());
    for (auto MovieName : MovieNames) cout << MovieName << endl;
}

int main(int argc, char* argv[]) {
    string FileName = SCHEDULE_FILE_NAME;
    if (argc > 1) FileName = argv[1];

    vector<Movie> Movies = ReadCSVFile(FileName);
    PrintMovies(Movies);

    return 0;
}