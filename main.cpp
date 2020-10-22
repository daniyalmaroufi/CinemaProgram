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
const int HALF_HOUR_COLUMNS = 5;
const int TOTAL_COLUMNS = 175;

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
    cout << "               00:00" << endl;
}

vector<map<string, string>> GetMovieScheduleOfDay(
    const vector<map<string, string>>& Movies, string MovieName, string Day) {
    vector<map<string, string>> Schedule;
    for (auto Movie : Movies) {
        if (Movie["MovieName"] == MovieName && Movie["Day"] == Day) {
            Schedule.push_back(Movie);
        }
    }
    return Schedule;
}

bool CompareCinemaNames(const map<string, string>& FirstMovie,
                        const map<string, string>& SecondMovie) {
    return FirstMovie.find("CinemaName")->second <
           SecondMovie.find("CinemaName")->second;
}

bool ComparePrices(const map<string, string>& FirstMovie,
                   const map<string, string>& SecondMovie) {
    return FirstMovie.find("Price")->second < SecondMovie.find("Price")->second;
}

bool CompareTimes(const map<string, string>& FirstMovie,
                  const map<string, string>& SecondMovie) {
    return FirstMovie.find("StartingTime")->second <
           SecondMovie.find("StartingTime")->second;
}

bool HaveInterference(const map<string, string>& FirstMovie,
                      const map<string, string>& SecondMovie) {
    if (FirstMovie.find("StartingTime")->second <
            SecondMovie.find("StartingTime")->second &&
        FirstMovie.find("FinishingTime")->second >
            SecondMovie.find("StartingTime")->second)
        return true;
    if (FirstMovie.find("StartingTime")->second ==
            SecondMovie.find("StartingTime")->second &&
        FirstMovie.find("FinishingTime")->second ==
            SecondMovie.find("FinishingTime")->second)
        return true;
    return false;
}

vector<map<string, string>> SortScheduleOfDay(
    vector<map<string, string>> Schedule) {
    // Sort with CinemaName
    sort(Schedule.begin(), Schedule.end(), CompareCinemaNames);
    // Sort with Price
    sort(Schedule.begin(), Schedule.end(), ComparePrices);
    // Sort with Starting Time
    sort(Schedule.begin(), Schedule.end(), CompareTimes);

    vector<map<string, string>> ChosenSchedule;
    ChosenSchedule.push_back(Schedule[0]);

    for (int i = 1; i < Schedule.size(); i++) {
        if (!HaveInterference(ChosenSchedule.back(), Schedule[i]))
            ChosenSchedule.push_back(Schedule[i]);
    }
    return ChosenSchedule;
}

float TimeToNumber(const string& Time) {
    stringstream SStream(Time);
    string Hour, Minute;
    getline(SStream, Hour, ':');
    getline(SStream, Minute, ':');
    float M = stoi(Minute);
    return stoi(Hour) * 2 + M / 30;
}

int StartingTimeToColumn(const string& Time) {
    return DAY_TITLE_MAX_LENGTH + 1 +
           (TimeToNumber(Time) - TimeToNumber("08:00")) * HALF_HOUR_COLUMNS;
}

int BlockLength(const string& StartingTime, const string& FinishingTime) {
    return (TimeToNumber(FinishingTime) - TimeToNumber(StartingTime)) * 5 - 1;
}

void PrintCharacters(int Length, char Character) {
    for (int i = 0; i < Length; i++) cout << Character;
}

void PrintSingleDayBox(const vector<map<string, string>>& Schedule) {
    int Column = 1;
    for (auto Movie : Schedule) {
        int TileLength =
            BlockLength(Movie["StartingTime"], Movie["FinishingTime"]);
        int StartingColumn = StartingTimeToColumn(Movie["StartingTime"]);

        PrintCharacters(StartingColumn - Column, ' ');

        if (StartingColumn + 1 != Column) cout << "+";

        PrintCharacters(TileLength, '-');

        cout << "+";
        Column = StartingColumn + TileLength + 2;
    }
    PrintCharacters(TOTAL_COLUMNS - Column + 1, ' ');
    cout << endl;
}

void PrintDay(const string& Day, const vector<map<string, string>>& Schedule) {
    cout << Day;
    PrintCharacters(DAY_TITLE_MAX_LENGTH - Day.length(), ' ');
    int Column = DAY_TITLE_MAX_LENGTH + 1;
    for (auto Movie : Schedule) {
        int TileLength =
            BlockLength(Movie["StartingTime"], Movie["FinishingTime"]);
        int StartingColumn = StartingTimeToColumn(Movie["StartingTime"]);

        PrintCharacters(StartingColumn - Column, ' ');

        if (StartingColumn + 1 != Column) cout << "|";

        cout << Movie["CinemaName"];

        PrintCharacters(TileLength - Movie["CinemaName"].length(), ' ');

        cout << "|";
        Column = StartingColumn + TileLength + 2;
    }
    PrintCharacters(TOTAL_COLUMNS - Column + 1, ' ');
    cout << endl;
}

void PrintMovieSchedule(const vector<map<string, string>>& Movies,
                        string MovieName) {
    PrintHours();
    // for (auto Day : DAYS) PrintDay(Day);
    vector<map<string, string>> DaySchedule;
    for (auto Day : DAYS) {
        DaySchedule =
            SortScheduleOfDay(GetMovieScheduleOfDay(Movies, MovieName, Day));
        PrintSingleDayBox(DaySchedule);
        PrintDay(Day, DaySchedule);
    }
    PrintSingleDayBox(DaySchedule);
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