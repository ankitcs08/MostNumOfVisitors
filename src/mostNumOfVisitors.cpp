#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief Represents a time with hour and minute components.
 */
class Time {

public:
    int hour;   /**< Hour component of the time */
    int minute; /**< Minute component of the time */
	/**
     * @brief Construct a new Time object.
     * 
     * @param h The hour component.
     * @param m The minute component.
     */
    Time(int h, int m) : hour(h), minute(m) {}


    /**
     * @brief Overloaded equality operator to compare two Time objects.
     * 
     * @param other The Time object to compare with.
     * @return true if the two Time objects are equal, false otherwise.
     */
	bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute;
    }

    /**
     * @brief Overloaded greater than operator to compare two Time objects.
     * 
     * @param other The Time object to compare with.
     * @return true if the first Time object is greater than the second time object, false otherwise.
     */
    bool operator>(const Time& other) const {
        if (hour == other.hour) {
            return minute > other.minute;
        }
        return hour > other.hour;
    }

    /**
     * @brief Overloaded less than operator to compare two Time objects.
     * 
     * @param other The Time object to compare with.
     * @return true if the first Time object is less than the second time object, false otherwise.
     */
    bool operator<(const Time& other) const {
        if (hour == other.hour) {
            return minute < other.minute;
        }
        return hour < other.hour;
    }

    /**
     * @brief Overloaded greater or equal operator to compare two Time objects.
     * 
     * @param other The Time object to compare with.
     * @return true if the first Time object is greater or equal to the second time object, false otherwise.
     */
    bool operator>=(const Time& other) const {
        return *this == other || *this > other;
    }

    /**
     * @brief Overloaded less or equal operator to compare two Time objects.
     * 
     * @param other The Time object to compare with.
     * @return true if the first Time object is less or equal to the second time object, false otherwise.
     */
    bool operator<=(const Time& other) const {
        return *this == other || *this < other;
    }

};

/**
 * @brief returns an integer denoting the input time object in minutes.
 */
int getTimeInMinute(Time t)
{
    return ((t.hour * 60) + t.minute);
}

/**
 * @brief returns a Time object against the input integer denoting the time in minutes.
 */
Time getTime(int minutes)
{
    Time time((int)minutes/60, minutes%60);
    return time;
}

/**
 * @brief returns a Time object against the input string provided in hh:mm format.
 */
Time parseTime(const string& timeStr) {
    Time t(0,0);
    sscanf(timeStr.c_str(), "%d:%d", &t.hour, &t.minute);
    return t;
}

/**
 * @brief returns an string denoting the time formulated in hh:mm format against the input time object.
 */
string formatTime(const Time& t) {
    char buffer[6];
    sprintf(buffer, "%02d:%02d", t.hour, t.minute);
    return buffer;
}

/**
 * @brief Finds the time period with the most number of visitors.
 * 
 * This function iterates through the visitor times stored in a map and
 * determines the time period during which the maximum number of visitors
 * were present.
 * 
 * @param visitorTimes, The map containing the count of visitors at each time.
 * @return a string representing the start and end times of the
 *         time period along with the count of the most visitors. (hh:mm-hh:mm;count)
 */
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

/**
 * @brief Finds the time period with the most number of visitors.
 * 
 * @param inputFile, The path to the input file containing visitor data.
 */
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

/**
 * @brief The entry point of the program.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int The exit status of the program.
 */
int main(int argc, char* argv[]) {
	
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " input_file_absolute_path" << endl;
        return 1; // returning error
    }

    string inputFile = argv[1];

    countVisitors(inputFile);

    return 0; // returning success
}
