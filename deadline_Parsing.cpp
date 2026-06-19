#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <limits>
#include <ctime>
#include <string>
#include <iomanip> // Essential for formatting tables and setw()

using namespace std;

#define MAX_USERNAME 50
#define MAX_PASSWORD 50

const int MAX_TASKS = 100;
const int TERMINAL_WIDTH = 75; // Width optimization for balanced column spaces

struct Task {
    time_t deadlineTimestamp; 
    string dueDateStr;        
};

string calculateTimeLeft(time_t deadline) {
    time_t now = time(0);
    if (deadline <= now) {
        return "PASSED / DUE";
    }
    
    long long diffSeconds = difftime(deadline, now);
    long long days = diffSeconds / (24 * 3600);
    diffSeconds %= (24 * 3600);
    long long hours = diffSeconds / 3600;
    
    if (days > 0) {
        return to_string(days) + "d " + to_string(hours) + "h left";
    }
    return to_string(hours) + "h left";
}

void parseAndAssignDeadline(Task &targetTask, int year, int month, int day, int hour, int minute) {
    tm deadlineTm = {0};
    deadlineTm.tm_year = year - 1900;
    deadlineTm.tm_mon = month - 1;
    deadlineTm.tm_mday = day;
    deadlineTm.tm_hour = hour;
    deadlineTm.tm_min = minute;
    deadlineTm.tm_isdst = -1;
    
    targetTask.deadlineTimestamp = mktime(&deadlineTm);
    
    char textBuf[30];
    strftime(textBuf, sizeof(textBuf), "%Y-%m-%d %H:%M", &deadlineTm);
    targetTask.dueDateStr = string(textBuf);
}