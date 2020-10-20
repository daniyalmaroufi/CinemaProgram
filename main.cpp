#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string SCHEDULE_FILE_NAME = "./schedule.csv";
const int DAY_TITLE_MAX_LENGTH = 10;
const vector<string> DAYS = {"Saturday",  "Sunday",   "Monday", "Tuesday",
                             "Wednesday", "Thursday", "Friday"};

vector<string> ReadHeader(string Line) {
    vector<string> Header;
    string ColName;
    stringstream SStream(Line);
    while (getline(SStream, ColName, ',')) {
        Header.push_back(ColName);
    }
    return Header;
}

map<string, string> ReadMovie(vector<string> Header, string Line) {
    string RecordValue;
    map<string, string> TempMovie;
    stringstream SStream(Line);
    for (auto ColName : Header) {
        getline(SStream, RecordValue, ',');
        TempMovie[ColName] = RecordValue;
    }
    return TempMovie;
}

vector<map<string, string>> ReadCSVFile(string FileName) {
    vector<map<string, string>> Movies;
    ifstream File;
    File.open(FileName);
    string Line;
    getline(File, Line);
    vector<string> Header = ReadHeader(Line);
    while (getline(File, Line)) Movies.push_back(ReadMovie(Header, Line));
    File.close();
    return Movies;
}

vector<string> GetMovieNames(const vector<map<string, string>>& Movies) {
    vector<string> MovieNames;
    for (auto Movie : Movies) MovieNames.push_back(Movie["MovieName"]);
    return MovieNames;
}

void PrintMovies(const vector<map<string, string>>& Movies) {
    vector<string> MovieNames = GetMovieNames(Movies);
    sort(MovieNames.begin(), MovieNames.end());
    // remove duplicated values
    MovieNames.erase(unique(MovieNames.begin(), MovieNames.end()),
                     MovieNames.end());
    for (auto MovieName : MovieNames) cout << MovieName << endl;
}

string RemoveCommand(string UserInput, const string& Command) {
    int index = 0;
    index = UserInput.find(Command, index);
    UserInput.replace(index, Command.length(), "");
    return UserInput;
}

string GetMovieNameFromInput(string UserInput) {
    return RemoveCommand(UserInput, "GET SCHEDULE ");
}

bool MovieExists(const vector<map<string, string>>& Movies, string MovieName) {
    for (auto Movie : Movies)
        if (Movie["MovieName"] == MovieName) return true;
    return false;
}

void PrintHours() {
    cout << "          08:00";
    cout << "               10:00";
    cout << "               12:00";
    cout << "               14:00";
    cout << "               16:00";
    cout << "               18:00";
    cout << "               20:00";
    cout << "               22:00";
    cout << "               00:00";
}

vector<map<string, string>> GetMovieSchedule(
    const vector<map<string, string>>& Movies, string MovieName) {
    vector<map<string, string>> Schedule;
    for (auto Movie : Movies) {
        if (Movie["MovieName"] == MovieName) {
            Schedule.push_back({{"Day", Movie["Day"]},
                                {"StartingTime", Movie["StartingTime"]},
                                {"FinishingTime", Movie["FinishingTime"]},
                                {"CinemaName", Movie["CinemaName"]}});
        }
    }
    return Schedule;
}

void PrintDay(string Day) {
    cout << endl << Day;
    for (int i = 0; i < DAY_TITLE_MAX_LENGTH - Day.length(); i++) cout << " ";
}

void PrintMovieSchedule(const vector<map<string, string>>& Movies,
                        string MovieName) {
    PrintHours();
    // for (auto Day : DAYS) PrintDay(Day);
    cout << MovieName << endl;
    for (auto Movie : GetMovieSchedule(Movies, MovieName)) {
        cout << Movie["Day"] << "\t" << Movie["StartingTime"] << " to "
             << Movie["FinishingTime"] << " at " << Movie["CinemaName"] << endl;
    }
}

void HandleUserInput(const vector<map<string, string>>& Movies) {
    string UserInput;
    getline(cin, UserInput);
    if (UserInput == "GET ALL MOVIES") {
        PrintMovies(Movies);
    }
    if (UserInput.find("GET SCHEDULE ") != string::npos) {
        string MovieName = GetMovieNameFromInput(UserInput);
        if (MovieExists(Movies, MovieName)) {
            PrintMovieSchedule(Movies, MovieName);
        }
    }
}

int main(int argc, char* argv[]) {
    string FileName = SCHEDULE_FILE_NAME;
    if (argc > 1) FileName = argv[1];

    vector<map<string, string>> Movies = ReadCSVFile(FileName);
    while (true) HandleUserInput(Movies);

    return 0;
}