#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

class Time {

public:
    int hour;
    int minute;
    Time(int h, int m) : hour(h), minute(m) {}


    // Overload the equality operator ==
    bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute;
    }

    // Overload the greater than operator >
    bool operator>(const Time& other) const {
        if (hour == other.hour) {
            return minute > other.minute;
        }
        return hour > other.hour;
    }

    // Overload the less than operator <
    bool operator<(const Time& other) const {
        if (hour == other.hour) {
            return minute < other.minute;
        }
        return hour < other.hour;
    }

    // Overload the greater than or equal to operator >=
    bool operator>=(const Time& other) const {
        return *this == other || *this > other;
    }

    // Overload the less than or equal to operator <=
    bool operator<=(const Time& other) const {
        return *this == other || *this < other;
    }

};

int getTimeInMinute(Time t)
{
    return ((t.hour * 60) + t.minute);
}

Time getTime(int minutes)
{
    Time time((int)minutes/60, minutes%60);
    return time;

}

Time parseTime(const string& timeStr) {
    Time t(0,0);
    sscanf(timeStr.c_str(), "%d:%d", &t.hour, &t.minute);
    return t;
}

string formatTime(const Time& t) {
    char buffer[6];
    sprintf(buffer, "%02d:%02d", t.hour, t.minute);
    return buffer;
}

string findMaxPeriod(const unordered_map<int, int>& visitorTimes) {

    int maxVisitors = 0;

	Time start_time(99,99);
	Time end_time(0, 0);

    for (const auto& entry : visitorTimes) {
        if (entry.second > maxVisitors) {
            maxVisitors = entry.second;
        }
    }
	
    for (const auto& entry : visitorTimes) 
	{
        if (entry.second == maxVisitors) 
		{
			if(getTimeInMinute(start_time) > entry.first)
				start_time = getTime(entry.first);
			if(getTimeInMinute(end_time) < entry.first)
				end_time = getTime(entry.first);
        }
    }

    return formatTime(start_time) + "-" + formatTime(end_time) + ";" + to_string(maxVisitors);
}

void countVisitors( string inputFile) 
{
    unordered_map<int, int> visitorTimes;
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string enter, leave;
        getline(ss, enter, ',');
        getline(ss, leave, ',');

		Time enter_time = parseTime(enter);
		Time leave_time = parseTime(leave);

		Time current_time = enter_time;
		while (current_time <= leave_time)
		{
			visitorTimes[getTimeInMinute(current_time)] += 1;
			current_time.minute++;
			if (current_time.minute >= 60) 
			{
				current_time.minute = 0;
				current_time.hour++;
			}
		}
	}
    string maxPeriod = findMaxPeriod(visitorTimes);
    cout << maxPeriod;
}

int main(int argc, char* argv[]) {
	
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " input_file_absolute_path" << endl;
        return 1;
    }

    string inputFile = argv[1];

    countVisitors(inputFile);

    return 0;
}
