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
    int id;
    string title;
    string description;
    string category;
    time_t deadlineTimestamp; // Stores exact epoch time for math calculations
    string dueDateStr;        // Friendly formatted string YYYY-MM-DD HH:MM
    int importance;           // 1 = High, 2 = Medium, 3 = Low
    bool completed;
};

//Global Variables
string currentUser = "";
string viewMode = "Detailed";
Task taskList[MAX_TASKS];  // Global Array & State Trackers
int taskCount = 0;
int nextTaskId = 1;

// Function Prototypes Registration and Login
string getUserFileName(); // Return file name
void showAdminMenu();
void registration();
void login();
bool usernameExists(const char username[]);
void saveUser(const char username[], const char password[]);
bool verifyUser(const char username[], const char password[]);
void clearInputBuffer();
void clearScreen();
void pauseScreen();

// Function Prototypes Task Management

void addTask();
void editTask();
void deleteTask();
void displayTasks();
int  findTask(int taskId);
void completeTask();
void reopenTask();
void saveTasks();
void loadTasks();
void appMenu();
string getTaskFileName();

//DEADLINE PARSING FUNCTION PROTOTYPES

time_t getCurrentTime();
bool parseDate(const string& dateInput, time_t& deadline);
time_t parseDeadline(const string& deadlineInput);
string formatDate(time_t deadline);
void showDeadlineHelp();

//PRIORITY ALGORITHM FUNCTION PROTOTYPES

int calculatePriority(int importance, bool completed);
void assignPriority();
void updatePriority();
string getPriorityLevel(int priorityScore);
void displayPriority();

//TUI NAVIGATION (Toast) FUNCTION PROTOTYPES

void showToast() ;

//SEARCH AND FILTER FUNCTION PROTOTYPES
void searchTask();
void filterTasks();
bool titleMatches(const string& title, const string& keyword);
void searchByTitle();
void filterByCategory();
bool categoryMatches(const string& category, const string& categoryName);
void filterByStatus();


bool titleMatches(const string& title, const string& keyword){
    string copytitle = title;
    string copykeyword = keyword;
    for (char &c : copytitle){
        c = tolower(static_cast<unsigned char>(c));
    }
    for (char &c : copykeyword){
        c = tolower (static_cast<unsigned char>(c));
    }

    size_t posisi = copytitle.find(copykeyword);
    return posisi != string::npos;
}

bool categoryMatches(const string& category, const string& categoryName){
    string copycat = category;
    string copykey = categoryName;

    for (char &c : copycat) c = tolower(static_cast<unsigned char>(c));
    for (char &c : copykey) c = tolower(static_cast<unsigned char>(c));

    return copycat.find(copykey) != string::npos;
}

void searchByTitle(){
    string keyword;
    int foundcount = 0;
    cout << "=======================================" << endl;
    cout << "Search by Title..." << endl;
    cout << "Enter keyword: ";
    getline(cin, keyword);
    cout << "=======================================" << endl;

    for (int i = 0; i < taskCount; i++){
        if (titleMatches(taskList[i].title, keyword)){
            cout << "ID      : " << taskList[i].id << endl;
            cout << "Title   : " << taskList[i].title << endl;
            cout << "Deadline: " << taskList[i].dueDateStr << endl;
            cout << "---------------------------------------" << endl;

            foundcount++;
        }
    }
    if (foundcount == 0){
        cout << "Search not found..." << endl;
    } else {
        cout << "Found " << foundcount << " task(s)." << endl;
        cout << "=======================================" << endl;
    }
    cout << "\nTekan ENTER untuk kembali ke menu...";
    system("pause");
}

void filterByCategory(){
    int foundcount = 0;
    string categoryt;
    cout << "=======================================" << endl;
    cout << "Search Category Priotity (High/Medium/Low): " << endl;
    cout << "Enter Category: ";
    getline(cin, categoryt);
    cout << "=======================================" << endl;

    for (char &c : categoryt) c = tolower(static_cast<unsigned char>(c));
    for (int i = 0; i < taskCount; i++){
        string pStr = (taskList[i].importance == 1 ? "high" : (taskList[i].importance == 2 ? "medium" : "low"));
        string pNum = to_string(taskList[i].importance); // "1", "2", atau "3"

        if (pStr.find(categoryt) != string::npos || pNum == categoryt){
            string priorityDisplay = (taskList[i].importance == 1 ? "High" : (taskList[i].importance == 2 ? "Medium" : "Low"));
            cout << "ID      : " << taskList[i].id << endl;
            cout << "Title   : " << taskList[i].title << endl;
            cout << "Priority: " << priorityDisplay << endl;
            cout << "---------------------------------------" << endl;
        foundcount++;
        }
    }

    if (foundcount == 0){
        cout << "Category not found..." << endl;
    } else {
        cout << "Found " << foundcount << " task(s) in this category." << endl;
    }
    cout << "=======================================" << endl;
    cout << "\nTekan ENTER untuk kembali ke menu...";
    system("pause");
}

void filterByStatus(){
    int statuschoice;
    cout << "=======================================" << endl;
    cout << "  Filter by Status: " << endl;
    cout << "  1. Pending" << endl;
    cout << "  2. Completed" << endl;
    cout << "=======================================" << endl;
    cout << "Choose option (1/2): ";

    while (!(cin >> statuschoice) || (statuschoice < 1 || statuschoice > 2)) {
        cout << "[Failed] Invalid choice! Input 1 or 2: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');

    bool targetstatus;
    if (statuschoice == 2){
        targetstatus = true;
    } else {
        targetstatus = false;
    }

    int foundcount = 0;
    cout << "=======================================" << endl;

    for (int i = 0; i < taskCount; i++){
        if (taskList[i].completed == targetstatus){
            cout << "ID      : " << taskList[i].id << endl;
            cout << "Title   : " << taskList[i].title << endl;
            cout << "Status  : " << (taskList[i].completed ? "Completed: " : "Pending") << endl;
            cout << "=======================================" << endl;
            foundcount++;
        }
    }
    if (foundcount == 0){
        cout << "No tasks found with this status..." << endl;
    } else {
        cout << "Found " << foundcount << " task(s)." << endl;
    }
    cout << "=======================================" << endl;
    cout << "\nTekan ENTER untuk kembali ke menu...";
    system("pause");
}

void searchTask(){
    int find;
    while (true){
    cout << "=======================================" << endl;
    cout << "              FIND TASK                " << endl;
    cout << "=======================================" << endl;
    cout << "1. Search by Title" << endl;
    cout << "0. Return" << endl;
    cout << "Choose Menu: ";

    if (!(cin >> find)) {
            cout << "[Failed] Input must be a number!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (find == 1) searchByTitle();
        else if (find == 0) {
            cout << "Thank you for using the system!" << endl;
            break;
        }
        else cout << "[Failed] Invalid menu! Choose between 0 and 2." << endl;
    }
}

void filterTasks(){
    int choosefilter;
    while (true){
    cout << "=======================================" << endl;
    cout << "                 Filter                " << endl;
    cout << "=======================================" << endl;
    cout << "1. By Category" << endl;
    cout << "2. By Status" << endl;
    cout << "0. Return" << endl;
    if (!(cin >> choosefilter)) {
            cout << "[Failed] Input must be a number!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        cin.ignore(1000, '\n');

        if (choosefilter == 1){
            filterByCategory();
        }
        else if (choosefilter == 2){
            filterByStatus();
        }
        else if (choosefilter == 0) {
            cout << "Thank you for using the system!" << endl;
            break;
        }
        else cout << "[Failed] Invalid menu! Choose between 0 and 2." << endl;
    }
}

// MAIN AUTHENTICATION MENU
void showAdminMenu() {
    int choice = true;
    while (true) {
        cout << "===============================" << endl;
        cout << "    LOGIN & REGISTRATION SYSTEM   " << endl;
        cout << "===============================" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose menu: ";

        if (!(cin >> choice)) {
            cout << "[Failed] Input must be a number!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) registration();
        else if (choice == 2) login();
        else if (choice == 0) {
            cout << "Thank you for using the system!" << endl;
            break;
        }
        else cout << "[Failed] Invalid menu! Choose between 0 and 2." << endl;
    }
}

string getUserFileName(){
    return "users.txt";
}

// REGISTRATION FUNCTION
void registration() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char confirmPassword[MAX_PASSWORD];
    
    cout << "\n--- USER REGISTRATION ---" << endl;
    
    // Get username
    cout << "Username: ";
    cin.ignore();
    cin.getline(username, MAX_USERNAME);
    
    // Validate username (not empty)
    if (strlen(username) == 0) {
        cout << "[Error] Username cannot be empty!" << endl;
        return;
    }
    
    // Check for duplicate username
    if (usernameExists(username)) {
        cout << "[Error] Username already exists! Please choose another one." << endl;
        return;
    }
    
    // Get password
    cout << "Password: ";
    cin.getline(password, MAX_PASSWORD);
    
    // Validate password (not empty)
    if (strlen(password) == 0) {
        cout << "[Error] Password cannot be empty!" << endl;
        return;
    }
    
    // Confirm password
    cout << "Confirm Password: ";
    cin.getline(confirmPassword, MAX_PASSWORD);
    
    // Check if passwords match
    if (strcmp(password, confirmPassword) != 0) {
        cout << "[Error] Passwords do not match!" << endl;
        return;
    }
    
    // Save user credentials
    saveUser(username, password);
    cout << "[Success] Registration successful! You can now login." << endl;
}

// LOGIN FUNCTION
void login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int attempts = 0;

    cout << "\n--- USER LOGIN ---" << endl;

    while (attempts < 3) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Username: ";
        cin.getline(username, MAX_USERNAME);

        cout << "Password: ";
        cin.getline(password, MAX_PASSWORD);

        if (verifyUser(username, password)) {
        currentUser = username;
        loadTasks();
        cout << "\n[Success] Login successful! Welcome, "<< username << "!\n";
        showToast();
        appMenu();
        return;
        }
        attempts++;
        cout << "[Error] Invalid username or password!" << endl;
    }

    cout << "[Error] Too many failed attempts.\n";
}
// CHECK IF USERNAME EXISTS IN DATABASE
bool usernameExists(const char username[]) {
    std::ifstream file(getUserFileName());
    if (!file.is_open()) {
        return false; // File doesn't exist yet
    }
    
    char storedUsername[MAX_USERNAME];
    char storedPassword[MAX_PASSWORD];
    
    while (file.getline(storedUsername, MAX_USERNAME, ':')) {
        file.getline(storedPassword, MAX_PASSWORD);
        
        if (strcmp(storedUsername, username) == 0) {
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

// SAVE USER CREDENTIALS TO FILE
void saveUser(const char username[], const char password[]) {
    std::ofstream file("users.txt", std::ios::app);
    
    if (!file.is_open()) {
        cout << "[Error] Could not open user database!" << endl;
        return;
    }
    
    // Store in format: username:password
    file << username << ":" << password << endl;
    file.close();
}

// VERIFY USER CREDENTIALS
bool verifyUser(const char username[], const char password[]) {
    std::ifstream file("users.txt");
    if (!file.is_open()) {
        cout << "[Error] No users registered yet!" << endl;
        return false;
    }
    
    char storedUsername[MAX_USERNAME];
    char storedPassword[MAX_PASSWORD];
    
    while (file.getline(storedUsername, MAX_USERNAME, ':')) {
        file.getline(storedPassword, MAX_PASSWORD);
        
        if (strcmp(storedUsername, username) == 0 && 
            strcmp(storedPassword, password) == 0) {
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

// CLEAR INPUT BUFFER
void clearInputBuffer() {
    cin.clear();
    cin.ignore(1000, '\n');
}

// UTILITY & CORE HELPER FUNCTIONS
void clearScreen() {
    cout << "\033[2J\033[H";
}

void ifEmpty() {
    cout << "\n[Error] Input cannot be empty!\n";
    cout << "Press Enter to try again...";
    cin.get(); 
}

// Formats live dates dynamically for the top right corner header layout
void displayHeader(string menuTitle) {
    clearScreen();
    time_t now = time(0);

    // DYNAMIC TOP BANNER: Scans and prints urgent reminders (< 12 Hours) at the very top
    bool bannerPrinted = false;
    for (int i = 0; i < taskCount; i++) {
        if (!taskList[i].completed) {
            double diffSeconds = difftime(taskList[i].deadlineTimestamp, now);
            // Check if deadline is in the future but less than 12 hours away
            if (diffSeconds > 0 && diffSeconds < (12 * 3600)) {
                long long totalMin = diffSeconds / 60;
                long long hours = totalMin / 60;
                long long mins = totalMin % 60;

                cout << "[URGENT] \"" << taskList[i].title << "\" DUE IN ";
                if (hours > 0) {
                    cout << hours << " hours " << mins << " minutes";
                } else {
                    cout << mins << " minutes";
                }
                cout << "\n";
                bannerPrinted = true;
            }
        }
    }
    if (bannerPrinted) {
        cout << string(TERMINAL_WIDTH, '-') << "\n";
    }
    tm *ltm = localtime(&now);
    char dateBuf[20];
    strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", ltm);
    string dateStr = "Today: " + string(dateBuf);
    
    int spacesNeeded = TERMINAL_WIDTH - menuTitle.length() - dateStr.length();
    if (spacesNeeded < 1) spacesNeeded = 1;
    
    cout << string(TERMINAL_WIDTH, '=') << "\n";
    cout << menuTitle << string(spacesNeeded, ' ') << dateStr << "\n";
    cout << string(TERMINAL_WIDTH, '=') << "\n";
}

// Computes structural differences between current clock data and deadlines
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

// Basic structural mapping search algorithm
int findTaskIndexById(int taskId) {
    for (int i = 0; i < taskCount; i++) {
        if (taskList[i].id == taskId) return i;
    }
    return -1;
}

// PRIORITY AND TIME METRIC SORTING ENGINE
void sortTasksByUrgency() {
    // Bubble Sort with multi-layered sorting rules 
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            bool swapNeeded = false;
            
            //check status completion (status of pending appear before status of completed)
            if (taskList[j].completed && !taskList[j + 1].completed) {
                swapNeeded = true; //swap so "pending" goes up
            } 
            else if (!taskList[j].completed && taskList[j + 1].completed) {
                swapNeeded = false; //right condition, pending is already above completed
            } 
            else {
                // if the status are the same (both pending or both completed)
                
                //check Due Date (closest deadline appears first)
                if (taskList[j].deadlineTimestamp > taskList[j + 1].deadlineTimestamp) {
                    swapNeeded = true; //swap, the closer due date goes up
                } 
                else if (taskList[j].deadlineTimestamp < taskList[j + 1].deadlineTimestamp) {
                    swapNeeded = false;
                } 
                else {
                    //if Due Dates are the same, check Importance (High:1, Medium:2, Low:3)
                    if (taskList[j].importance > taskList[j + 1].importance) {
                        swapNeeded = true; //swap, the most important goes up
                    }
                }
            }
            
            //swap objects if the condition is fulfilled
            if (swapNeeded) {
                Task temp = taskList[j];
                taskList[j] = taskList[j + 1];
                taskList[j + 1] = temp;
            }
        }
    }
}

// SYSTEM ACTION CONTROLLERS
void addTask() {
    if (taskCount >= MAX_TASKS) {
        cout << "\n[Error] Planner database registry allocation full!\n";
        return;
    }

    Task newTask;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;
    int currentHour = ltm->tm_hour;
    int currentMin = ltm->tm_min;
    
    // 1. INPUT TITLE
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : ";
        getline(cin, newTask.title);
        if (newTask.title.empty()) ifEmpty();
        else break;
    }
    
    // 2. INPUT DESCRIPTION
    cout << "Enter Task's Description : ";
    getline(cin, newTask.description);

    // 3. INPUT DEADLINE 
    int year, month, day, hour, minute;
    string errorMsg = "";
    
    // Loop Year
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }

        cout << "    YYYY                 : ";
        if (!(cin >> year)) {
            cin.clear(); cin.ignore(10000, '\n');
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        cin.ignore(10000, '\n');

        if (year >= currentYear) break;
        else errorMsg = "[Error] Invalid year! Year must be " + to_string(currentYear) + " or above.";
    }

    // Loop Month
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }

        cout << "    MM (1-12)            : ";
        if (!(cin >> month)) {
            cin.clear(); cin.ignore(10000, '\n');
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        cin.ignore(10000, '\n');

        if (month >= 1 && month <= 12 && !(year == currentYear && month < currentMonth)) break;
        else errorMsg = "[Error] Invalid month! Cannot be in the past or outside 1-12.";
    }

    // Loop Day
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }

        cout << "    DD                   : ";
        if (!(cin >> day)) {
            cin.clear(); cin.ignore(10000, '\n');
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        cin.ignore(10000, '\n');

        int maxDays = 31;
        if (month == 4 || month == 6 || month == 9 || month == 11) maxDays = 30;
        else if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            maxDays = isLeap ? 29 : 28;
        }

        if (day >= 1 && day <= maxDays && !(year == currentYear && month == currentMonth && day < currentDay)) break;
        else errorMsg = "[Error] Invalid day! Max day for this month is " + to_string(maxDays) + " and cannot be in the past.";
    }

    // Loop Hour
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        cout << "    DD                   : " << day << endl;
        if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }

        cout << "    HH (0-23)            : ";
        if (!(cin >> hour)) {
            cin.clear(); cin.ignore(10000, '\n');
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        cin.ignore(10000, '\n');

        if (hour >= 0 && hour <= 23 && !(year == currentYear && month == currentMonth && day == currentDay && hour < currentHour)) break;
        else errorMsg = "[Error] Invalid hour! Must be 0-23 and cannot be in the past.";
    }

    // Loop Minute
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        cout << "    DD                   : " << day << endl;
        cout << "    HH (0-23)            : " << hour << endl;
        if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }

        cout << "    MM (0-59)            : ";
        if (!(cin >> minute)) {
            cin.clear(); cin.ignore(10000, '\n');
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        cin.ignore(10000, '\n');

        if (minute >= 0 && minute <= 59 && !(year == currentYear && month == currentMonth && day == currentDay && hour == currentHour && minute < currentMin)) break;
        else errorMsg = "[Error] Invalid minute! Must be 0-59 and cannot be in the past.";
    }

    // Finalize Timestamp Assignment
    tm deadlineTm = {0};
    deadlineTm.tm_year = year - 1900;
    deadlineTm.tm_mon = month - 1;
    deadlineTm.tm_mday = day;
    deadlineTm.tm_hour = hour;
    deadlineTm.tm_min = minute;
    deadlineTm.tm_isdst = -1;
    
    newTask.deadlineTimestamp = mktime(&deadlineTm);
    
    char textBuf[30];
    strftime(textBuf, sizeof(textBuf), "%Y-%m-%d %H:%M", &deadlineTm);
    newTask.dueDateStr = string(textBuf);

    // 4. INPUT IMPORTANCE LEVEL
    while (true) {
        cout << "Set Importance Level (High:1 | Medium:2 | Low:3): ";
        if (!(cin >> newTask.importance) || newTask.importance < 1 || newTask.importance > 3) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "[Error] Value out of range selector map bounds!\n"; continue;
        }
        cin.ignore(10000, '\n');
        break;
    }

    newTask.id = nextTaskId++;
    newTask.completed = false;
    newTask.category = "General";
    
    taskList[taskCount++] = newTask;
    saveTasks();
    cout << "\n[Success] New task registered securely.\n";
}

void showToast() {
    sortTasksByUrgency();

    time_t now = time(0);
    bool urgent = false;

    // Scan for urgent tasks
    for (int i = 0; i < taskCount; i++) {
        if (!taskList[i].completed) {
            int thresholdHours = 0;
            if (taskList[i].importance == 1) thresholdHours = 24;      // High Priority
            else if (taskList[i].importance == 2) thresholdHours = 12; // Medium Priority
            else if (taskList[i].importance == 3) thresholdHours = 6;  // Low Priority

            double thresholdSeconds = thresholdHours * 3600.0;
            double diffSeconds = difftime(taskList[i].deadlineTimestamp, now);
            
            // Toast settings
            if (diffSeconds <= thresholdSeconds) {
                
                // Clear screen for the first popup only
                if (!urgent) clearScreen();
                
                long long totalMin = diffSeconds / 60;
                long long hours = totalMin / 60;
                long long mins = totalMin % 60;

                bool overdue = (diffSeconds < 0);
                long long displayHours = abs(hours);
                long long displayMins = abs(mins);

                // Format Priority String
                string priority = (taskList[i].importance == 1) ? "High" : 
                                  (taskList[i].importance == 2) ? "Medium" : "Low";

                // Format string lengths to prevent the ASCII box from breaking
                string safeTitle = (taskList[i].title.length() > 38) ? taskList[i].title.substr(0, 35) + "..." : taskList[i].title;
                string safeDesc = (taskList[i].description.length() > 38) ? taskList[i].description.substr(0, 35) + "..." : taskList[i].description;

                // Toast notification ui
                cout << "  .--------------------------------------------------.\n";
                cout << "  | [i] DONEXT Reminder                              |\n";
                cout << "  |--------------------------------------------------|\n";
                cout << "  | Task : " << left << setw(39) << safeTitle << "   |\n";
                cout << "  | Desc : " << left << setw(39) << safeDesc << "   |\n";
                cout << "  | Prio : " << left << setw(39) << priority << "   |\n";
                cout << "  |                                                  |\n";
                
                // Print time remaining/overdue
                string timePrefix = overdue ? "Overdue: -" : "Due in :  ";
                cout << "  | " << timePrefix << displayHours << "h " << displayMins << "m  ";
                int timeStrLength = timePrefix.length() + to_string(displayHours).length() + 2 + to_string(displayMins).length() + 3;
                int padding = 49 - timeStrLength;
                cout << right << setw(padding) << taskList[i].dueDateStr << "|\n";
                
                cout << "  |                                                  |\n";
                cout << "  '--------------------------------------------------'\n\n";

                urgent = true;
            }
        }
    }

    if (urgent) {
        cout << "Press Enter to dismiss notifications and open planner...";
        cin.get(); 
        clearScreen();
    }
}

void displayTasks() {
    sortTasksByUrgency();
    displayHeader("ALL TASK ENTRIES (URGENCY RANKED)");
    
    if (taskCount == 0) {
        cout << "\n           [Notice] Your planner is completely clean. No tasks found.\n\n";
        cout << string(TERMINAL_WIDTH, '=') << "\n";
        return;
    }

    cout << left << setw(4)  << "ID" 
         << setw(15) << "Title" 
         << setw(18) << "Deadline" 
         << setw(15) << "Time Left" 
         << setw(10) << "Priority" 
         << "Status" << endl;
         
    cout << string(TERMINAL_WIDTH, '-') << "\n";

    for (int i = 0; i < taskCount; i++) {
        string priorityStr = (taskList[i].importance == 1 ? "High" : (taskList[i].importance == 2 ? "Medium" : "Low"));
        string statusStr = taskList[i].completed ? "Completed" : "Pending";
        string titleSummary = (taskList[i].title.length() > 12) ? taskList[i].title.substr(0, 9) + "..." : taskList[i].title;
        string timeLeftStr = calculateTimeLeft(taskList[i].deadlineTimestamp);

        cout << left << setw(4)  << taskList[i].id 
             << setw(15) << titleSummary 
             << setw(18) << taskList[i].dueDateStr 
             << setw(15) << timeLeftStr 
             << setw(10) << priorityStr 
             << statusStr << endl;
    }
    cout << string(TERMINAL_WIDTH, '=') << "\n";
}

void modifyTask() {
    displayTasks();
    if (taskCount == 0) return;

    int idChoice;
    cout << "\nEnter the Task ID to alter parameters: ";
    if (!(cin >> idChoice)) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    int index = findTaskIndexById(idChoice);
    if (index == -1) { cout << "[Error] ID reference not found.\n"; return; }

    Task *taskPtr = &taskList[index];
    displayHeader("MODIFY TASK DATA");
    cout << "Selected: " << taskPtr->title << "\n\n";
    cout << "1. Toggle Progress State (Complete/Pending)\n";
    cout << "2. Rewrite Title and Description\n";
    cout << "3. Edit Deadline (Date & Time)\n";
    cout << "4. Change Importance Level\n";
    cout << "5. Cancel & Go Back\n";
    cout << "Choice (1-5): ";
    
    int subChoice;
    if (!(cin >> subChoice)) { 
        cin.clear(); cin.ignore(10000, '\n'); 
        return; 
    }
    cin.ignore(10000, '\n');
    
    if (subChoice == 0) {
        cout << "\n[Notice] Custom modification canceled. Returning to safety loop...\n";
        return;
    }
    else if (subChoice == 1) {
        taskPtr->completed = !taskPtr->completed;
        saveTasks();
        cout << "\n[Success] Status changed.\n";
    } 
    else if (subChoice == 2) {
        cout << "New Title: "; getline(cin, taskPtr->title);
        cout << "New Desc : "; getline(cin, taskPtr->description);
        saveTasks();
        cout << "\n[Success] Content rewritten.\n";
    } 
    else if (subChoice == 4) {
        while (true) {
            cout << "Set New Importance Level (High:1 | Medium:2 | Low:3): ";
            if (!(cin >> taskPtr->importance) || taskPtr->importance < 1 || taskPtr->importance > 3) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "[Error] Value out of range selector map bounds!\n"; continue;
            }
            cin.ignore(10000, '\n');
            break;
        }
        saveTasks();
        cout << "\n[Success] Importance level adjusted successfully.\n";
    }
    else if (subChoice == 3) {
        int year, month, day, hour, minute;
        string errorMsg = "";
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int currentYear = 1900 + ltm->tm_year;
        int currentMonth = 1 + ltm->tm_mon;
        int currentDay = ltm->tm_mday;
        int currentHour = ltm->tm_hour;
        int currentMin = ltm->tm_min;

        // Year Input Loop
        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            if (!errorMsg.empty()) { 
                cout << errorMsg << "\n"; errorMsg = ""; 
            }
            cout << "    Year (YYYY)          : ";
            if (!(cin >> year)) { 
                cin.clear(); cin.ignore(10000, '\n'); errorMsg = "[Error] Integer format only!";
                continue; 
            }
            cin.ignore(10000, '\n');
            if (year >= currentYear) 
                break;
            else errorMsg = "[Error] Invalid year! Year must be " + to_string(currentYear) + " or above.";
        }

        // Month Input Loop
        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            if (!errorMsg.empty()) { 
                cout << errorMsg << "\n"; errorMsg = ""; 
            }
            cout << "    Month (1-12)         : ";
            if (!(cin >> month)) { 
                cin.clear(); cin.ignore(10000, '\n'); errorMsg = "[Error] Integer format only!"; 
                continue; 
            }
            cin.ignore(10000, '\n');
            if (month >= 1 && month <= 12 && !(year == currentYear && month < currentMonth)) 
                break;
            else errorMsg = "[Error] Invalid month! Cannot be in the past or outside 1-12.";
        }

        // Day Input Loop
        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }
            cout << "    Day (1-31)           : ";
            if (!(cin >> day)) { cin.clear(); cin.ignore(10000, '\n'); 
                errorMsg = "[Error] Integer format only!"; 
                continue;
            }
            cin.ignore(10000, '\n');

            int maxDays = 31;
            if (month == 4 || month == 6 || month == 9 || month == 11) maxDays = 30;
            else if (month == 2) {
                bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                maxDays = isLeap ? 29 : 28;
            }

            if (day >= 1 && day <= maxDays && !(year == currentYear &&
                month == currentMonth && day < currentDay)) break;
            else errorMsg = "[Error] Invalid day! Max day for this month is " + to_string(maxDays) + " and cannot be in the past.";
        }

        // Hour Input Loop
        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            cout << "    Day (1-31)           : " << day << endl;
            if (!errorMsg.empty()) {
                cout << errorMsg << "\n"; errorMsg = "";
            }
            cout << "    Hour (0-23)          : ";
            if (!(cin >> hour)) { 
                cin.clear(); cin.ignore(10000, '\n'); errorMsg = "[Error] Integer format only!"; continue;
            }
            cin.ignore(10000, '\n');
            if (hour >= 0 && hour <= 23 && !(year == currentYear && month == currentMonth && 
                day == currentDay && hour < currentHour)) break;
            else errorMsg = "[Error] Invalid hour! Must be 0-23 and cannot be in the past.";
        }

        // Minute Input Loop
        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            cout << "    Day (1-31)           : " << day << endl;
            cout << "    Hour (0-23)          : " << hour << endl;
            if (!errorMsg.empty()) { cout << errorMsg << "\n"; errorMsg = ""; }
            cout << "    Minute (0-59)        : ";
            if (!(cin >> minute)) { cin.clear(); cin.ignore(10000, '\n'); 
                errorMsg = "[Error] Integer format only!"; continue; }
            cin.ignore(10000, '\n');
            if (minute >= 0 && minute <= 59 && !(year == currentYear && month == currentMonth && 
                day == currentDay && hour == currentHour && minute < currentMin)) break;
            else errorMsg = "[Error] Invalid minute! Must be 0-59 and cannot be in the past.";
        }

        tm deadlineTm = {0};
        deadlineTm.tm_year = year - 1900;
        deadlineTm.tm_mon = month - 1;
        deadlineTm.tm_mday = day;
        deadlineTm.tm_hour = hour;
        deadlineTm.tm_min = minute;
        deadlineTm.tm_isdst = -1;
        
        taskPtr->deadlineTimestamp = mktime(&deadlineTm);
        
        char textBuf[30];
        strftime(textBuf, sizeof(textBuf), "%Y-%m-%d %H:%M", &deadlineTm);
        taskPtr->dueDateStr = string(textBuf);
        saveTasks();
        cout << "\n[Success] Deadline updated successfully!\n";
    } else {
        cout << "[Error] Invalid sub-option choice.\n";
    }
}

void deleteTask() {
    displayTasks();
    if (taskCount == 0) return;

    int idChoice;
    cout << "\nEnter Target Erasure Task ID: ";
    if (!(cin >> idChoice)) { 
    cin.clear(); cin.ignore(10000, '\n'); 
    return; 
    }
    cin.ignore(10000, '\n');

    int index = findTaskIndexById(idChoice);
    if (index == -1) { 
    cout << "[Error] Reference not identified.\n";
    return;
    }
    for (int i = index; i < taskCount - 1; i++) {
        taskList[i] = taskList[i + 1];
    }
    taskCount--;
    saveTasks();
    cout << "[Success] Element deleted successfully.\n";
}
string getTaskFileName() {
    return currentUser + "_tasks.txt";
}

void appMenu() {
    string inputOpsi;
    while (true) {
        displayHeader("DONEXT: MAIN MENU");
        cout << "1. Add New Task\n2. View Sorted Agenda\n3. Modify Existing Properties\n4. Remove Records\n5. Search Task\n6. Filter\n0. Exit\n\nSelect option (0-6): ";
        getline(cin, inputOpsi);

        if (inputOpsi.empty() || inputOpsi.length() != 1 || (inputOpsi[0] < '0' || inputOpsi[0] > '6')) {
            cout << "\n[Error] Invalid Selection Action Code mapping range pattern!\nPress Enter to reload menu context...";
            cin.get(); continue;
        }

        int opt = inputOpsi[0] - '0';
        if (opt == 0) break;

        switch (opt) {
            case 1: addTask(); break;
            case 2: displayTasks(); break;
            case 3: modifyTask(); break;
            case 4: deleteTask(); break;
            case 5: searchTask(); break;
            case 6: filterTasks(); break;
        }
        cout << "\nPress Enter to return to Main Menu...";
        cin.get();
    }
}
void saveTasks() {
    ofstream file(getTaskFileName());

    if (!file.is_open()) {
        cout << "[Error] Cannot save tasks!\n";
        return;
    }

    file << nextTaskId << endl;
    file << taskCount << endl;

    for (int i = 0; i < taskCount; i++) {

        file << taskList[i].id << "|"
             << taskList[i].title << "|"
             << taskList[i].description << "|"
             << taskList[i].category << "|"
             << taskList[i].deadlineTimestamp << "|"
             << taskList[i].dueDateStr << "|"
             << taskList[i].importance << "|"
             << taskList[i].completed
             << endl;
    }

    file.close();
}
void loadTasks() {

    ifstream file(getTaskFileName());

    if (!file.is_open()) {
        taskCount = 0;
        nextTaskId = 1;
        return;
    }

    file >> nextTaskId;
    file >> taskCount;
    file.ignore();

    string line;

    for (int i = 0; i < taskCount; i++) {

        getline(file, line);

        stringstream ss(line);
        string temp;

        getline(ss, temp, '|');
        taskList[i].id = stoi(temp);

        getline(ss, taskList[i].title, '|');
        getline(ss, taskList[i].description, '|');
        getline(ss, taskList[i].category, '|');

        getline(ss, temp, '|');
        taskList[i].deadlineTimestamp = stoll(temp);

        getline(ss, taskList[i].dueDateStr, '|');

        getline(ss, temp, '|');
        taskList[i].importance = stoi(temp);

        getline(ss, temp, '|');
        taskList[i].completed = stoi(temp);
    }

    file.close();
}

// current time
    // task and the descpription
    // priority
    // deadline

int main() {
    showAdminMenu();
    return 0;
}