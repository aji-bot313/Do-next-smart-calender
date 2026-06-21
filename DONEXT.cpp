#include <iostream>
#include <fstream>
#include <cstring> 
#include <ctime>
#include <string>
#include <iomanip> 
#include <conio.h> 

using namespace std;

#define MAX_USERNAME 50
#define MAX_PASSWORD 50

const int MAX_TASKS = 100;
const int TERMINAL_WIDTH = 75; 

// ANSI Escape Codes for Colors
#define RESET "\033[0m"
#define GREEN "\033[1;92m"
#define YELLOW "\033[38;2;227;180;34m"
#define RED "\033[1;91m"
#define CYAN "\033[1;96m"
#define GRAY "\033[0;90m"
#define WHITE "\033[1;97m"
#define MAGENTA "\033[1;95m"
#define BLUE "\033[1;94m"

struct Task {
    int id;
    string title;
    string description;
    string category;
    time_t deadlineTimestamp; 
    string dueDateStr;        
    int importance;           
    bool completed;
};

// Global Variables
string currentUser = "";
Task taskList[MAX_TASKS];  
int taskCount = 0;
int nextTaskId = 1;

// Function Prototypes
string getUserFileName(); 
void showAdminMenu();
void registration();
void login();
bool usernameExists(const char username[]);
void saveUser(const char username[], const char password[]);
bool verifyUser(const char username[], const char password[]);
void clearInputBuffer();
void clearScreen();
void pauseScreen();
void ifEmpty();

void addTask();
void modifyTask();
void deleteTask();
void displayTasks();
int findTaskIndexById(int taskId);
void saveTasks();
void loadTasks();
void appMenu();
string getTaskFileName();

void showToast();
void displayHeader(string menuTitle);
string calculateTimeLeft(time_t deadline);
void sortTasksByUrgency();

bool titleMatches(const string& title, const string& keyword);
void searchByTitle();
void filterByCategory();
void filterByStatus();
void searchTask();
void filterTasks();

// Helper function to check if the user entered "exit" (case-insensitive)
bool checkExit(const string& userInput) {
    string lowercaseInput = userInput;
    for (char &c : lowercaseInput) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return (lowercaseInput == "exit");
}

// Generic Interactive Menu Arrow-Key Navigator Helper
int navigateMenu(string title, const string options[], int count) {
    int selected = 0;
    while (true) {
        displayHeader(title);
        
        for (int i = 0; i < count; i++) {
            string optText;
            if (i == selected) {
                optText = ">> " + options[i] + " <<";
            } else {
                optText = options[i];
            }
            
            int padding = (TERMINAL_WIDTH - optText.length()) / 2;
            if (padding < 0) padding = 0;
            
            cout << string(padding, ' ');
            if (i == selected) {
                cout << BLUE << optText << RESET << "\n"; 
            } else {
                cout << optText << "\n";
            }
        }
        cout << string(TERMINAL_WIDTH, '=') << "\n";
        
        string hint = "(Use UP/DOWN arrows to navigate, ENTER to select)";
        int hintPadding = (TERMINAL_WIDTH - hint.length()) / 2;
        if (hintPadding < 0) hintPadding = 0;
        cout << string(hintPadding, ' ') << hint << "\n";

        int ch = _getch();
        if (ch == 0 || ch == 224) { // Handle arrow key sequences
            ch = _getch();
            if (ch == 72) { // Up Arrow
                selected = (selected - 1 + count) % count;
            } else if (ch == 80) { // Down Arrow
                selected = (selected + 1) % count;
            }
        } else if (ch == 13) { // Enter Key
            return selected;
        }
    }
}

// UTILITY & CORE HELPER FUNCTIONS 
void clearInputBuffer() {
    cin.clear(); 
    while (cin.get() != '\n'); 
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    cout << "\nPess ENTER to return to main menu...";
    cin.get();
}

void ifEmpty() {
    cout << "\n" << RED << "[Error] Input cannot be empty!" << RESET << "\n";
    cout << "Press Enter to try again...";
    cin.get(); 
}

// SEARCH AND FILTER FUNCTIONS
bool titleMatches(const string& title, const string& keyword){
    string copytitle = title;
    string copykeyword = keyword;
    for (char &c : copytitle){
        c = tolower(static_cast<unsigned char>(c));
    }
    for (char &c : copykeyword){
        c = tolower(static_cast<unsigned char>(c));
    }

    size_t posisi = copytitle.find(copykeyword);
    return posisi != string::npos;
}

void searchByTitle(){
    string keyword;
    int foundcount = 0;
    cout << "=======================================" << endl;
    cout << "Search by Title..." << endl;
    cout << "Enter keyword: ";
    getline(cin, keyword);
    cout << "=======================================" << endl;

    // Interrupt operation and return if 'exit' keyword is triggered
    if (checkExit(keyword)) {
        cout << YELLOW << "\n[Notice] Search operation cancelled. Returning to menu...\n" << RESET;
        return;
    }

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
    pauseScreen();
}

void filterByCategory(){
    int foundcount = 0;
    string categoryt;
    cout << "=======================================" << endl;
    cout << "Search Category Priority (High/Medium/Low): " << endl;
    cout << "Enter Category: ";
    getline(cin, categoryt);
    cout << "=======================================" << endl;

    for (char &c : categoryt) c = tolower(static_cast<unsigned char>(c));
    for (int i = 0; i < taskCount; i++){
        string pStr = (taskList[i].importance == 1 ? "high" : (taskList[i].importance == 2 ? "medium" : "low"));
        string pNum = to_string(taskList[i].importance); 

        if (pStr.find(categoryt) != string::npos || pNum == categoryt){
            cout << "ID      : " << taskList[i].id << endl;
            cout << "Title   : " << taskList[i].title << endl;
            
            cout << "Priority: ";
            if (taskList[i].importance == 1)      cout << RED << "High" << RESET << endl;
            else if (taskList[i].importance == 2) cout << YELLOW << "Medium" << RESET << endl;
            else                                  cout << GREEN << "Low" << RESET << endl;
            
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
    pauseScreen();
}

void filterByStatus(){
    string options[] = {"Pending Tasks", "Completed Tasks"};
    int selection = navigateMenu("FILTER BY STATUS", options, 2);

    bool targetstatus = (selection == 1);
    int foundcount = 0;
    
    displayHeader("FILTER BY STATUS RESULTS");

    for (int i = 0; i < taskCount; i++){
        if (taskList[i].completed == targetstatus){
            cout << "ID      : " << taskList[i].id << endl;
            cout << "Title   : " << taskList[i].title << endl;
            cout << "Status  : " << (taskList[i].completed ? "Completed" : "Pending") << endl;
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
    pauseScreen();
}

void searchTask(){
    string options[] = {"Search by Title", "Return to Main Menu"};
    while (true){
        int choice = navigateMenu("FIND TASK MENU", options, 2);
        if (choice == 0) searchByTitle();
        else if (choice == 1) break;
    }
}

void filterTasks(){
    string options[] = {"By Category (Priority)", "By Status", "Return to Main Menu"};
    while (true){
        int choice = navigateMenu("FILTER TASKS", options, 3);
        if (choice == 0) filterByCategory();
        else if (choice == 1) filterByStatus();
        else if (choice == 2) break;
    }
}

// MAIN AUTHENTICATION MENU
void showAdminMenu() {
    string options[] = {"Register New Account", "Login to Account", "Exit Application"};
    while (true) {
        int choice = navigateMenu("LOGIN & REGISTRATION SYSTEM", options, 3);
        if (choice == 0) registration();
        else if (choice == 1) login();
        else if (choice == 2) {
            cout << "\nThank you for using the system!\n";
            break;
        }
    }
}

string getUserFileName(){
    return "users.txt";
}

void registration() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char confirmPassword[MAX_PASSWORD];
    
    cout << "\n--- USER REGISTRATION ---" << endl;
    cout << "Username: ";
    cin.getline(username, MAX_USERNAME);
    
    if (strlen(username) == 0) {
        cout << RED << "[Error] Username cannot be empty!" << RESET << endl;
        return;
    }
    if (usernameExists(username)) {
        cout << RED << "[Error] Username already exists! Please choose another one." << RESET << endl;
        return;
    }
    
    cout << "Password: ";
    cin.getline(password, MAX_PASSWORD);
    
    if (strlen(password) == 0) {
        cout << RED << "[Error] Password cannot be empty!" << RESET << endl;
        return;
    }
    
    cout << "Confirm Password: ";
    cin.getline(confirmPassword, MAX_PASSWORD);
    
    if (strcmp(password, confirmPassword) != 0) {
        cout << RED << "[Error] Passwords do not match!" << RESET << endl;
        return;
    }
    
    saveUser(username, password);
    cout << GREEN << "[Success] Registration successful! You can now login." << RESET << endl;
    pauseScreen();
}

void login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int attempts = 0;

    cout << "\n--- USER LOGIN ---" << endl;

    while (attempts < 3) {
        cout << "Username: ";
        cin.getline(username, MAX_USERNAME);

        cout << "Password: ";
        cin.getline(password, MAX_PASSWORD);

        if (verifyUser(username, password)) {
            currentUser = username;
            loadTasks();
            cout << "\n" << GREEN << "[Success] Login successful! Welcome, " << username << "!" << RESET << "\n";
            pauseScreen();
            showToast();
            appMenu();
            return;
        }
        attempts++;
        cout << RED << "[Error] Invalid username or password! (" << 3 - attempts << " attempts left)\n" << RESET << endl;
    }

    cout << RED << "[Error] Too many failed attempts.\n" << RESET;
    pauseScreen();
}

bool usernameExists(const char username[]) {
    ifstream file(getUserFileName());
    if (!file.is_open()) return false; 
    
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

void saveUser(const char username[], const char password[]) {
    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cout << RED << "[Error] Could not open user database!" << RESET << endl;
        return;
    }
    file << username << ":" << password << endl;
    file.close();
}

bool verifyUser(const char username[], const char password[]) {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cout << RED << "[Error] No users registered yet!" << RESET << endl;
        return false;
    }
    
    char storedUsername[MAX_USERNAME];
    char storedPassword[MAX_PASSWORD];
    
    while (file.getline(storedUsername, MAX_USERNAME, ':')) {
        file.getline(storedPassword, MAX_PASSWORD);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void displayHeader(string menuTitle) {
    clearScreen();
    time_t now = time(0);

    cout << RED"                ____  ____              "<<GREEN"_   _________  ________\n";
    cout << RED"               / __ \\/ __ \\            "<<GREEN"/ | / / ____/ |/ /_  __/\n";
    cout << RED"              / / / / / / /  "<<YELLOW"______   "<<GREEN"/  |/ / __/  |   / / /\n";
    cout << RED"             / /_/ / /_/ /  "<<YELLOW"/_____/  "<<GREEN"/ /|  / /___ /   | / /\n";
    cout << RED"            /_____/\\____/           "<<GREEN"/_/ |_/_____//_/|_|/_/\n";
    cout << RESET << endl ;

    if (!currentUser.empty()) {
        bool bannerPrinted = false;
        for (int i = 0; i < taskCount; i++) {
            if (!taskList[i].completed) {
                double diffSeconds = difftime(taskList[i].deadlineTimestamp, now);
                if (diffSeconds > 0 && diffSeconds < (12 * 3600)) {
                    long long totalMin = diffSeconds / 60;
                    long long hours = totalMin / 60;
                    long long mins = totalMin % 60;
                    
                    cout << RED << "[URGENT] \"" << taskList[i].title << "\" DUE IN ";
                    if (hours > 0) cout << hours << " hours " << mins << " minutes";
                    else cout << mins << " minutes";
                    cout << RESET << "\n";
                    bannerPrinted = true;
                }
            }
        }
    }
    
    tm *ltm = localtime(&now);
    char dateBuf[20];
    strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", ltm);
    string dateStr = "Today: " + string(dateBuf);
    
    int spacesNeeded = TERMINAL_WIDTH - menuTitle.length() - dateStr.length();
    if (spacesNeeded < 1) spacesNeeded = 1;
    
    cout << string(TERMINAL_WIDTH, '=') << RESET << "\n";
    cout << menuTitle << string(spacesNeeded, ' ') << dateStr << RESET << "\n";
    cout << string(TERMINAL_WIDTH, '=') << RESET << "\n";

    // Place the context hint right below the line at the top-right corner
    if (menuTitle.find("ADD") != string::npos || 
        menuTitle.find("EDIT") != string::npos || 
        menuTitle.find("SEARCH") != string::npos) {
        
        string exitHint = RED "[Type 'exit' to cancel]" RESET;
        // 21 represents the true raw length of "[Type 'exit' to cancel]" excluding color escape characters
        int hintSpaces = TERMINAL_WIDTH - 21; 
        if (hintSpaces < 0) hintSpaces = 0;
        
        cout << string(hintSpaces, ' ') << exitHint << "\n";
    }
}

string calculateTimeLeft(time_t deadline) {
    time_t now = time(0);
    if (deadline <= now) return "PASSED / DUE";
    
    long long diffSeconds = difftime(deadline, now);
    long long days = diffSeconds / (24 * 3600);
    diffSeconds %= (24 * 3600);
    long long hours = diffSeconds / 3600;
    
    if (days > 0) return to_string(days) + "d " + to_string(hours) + "h left";
    return to_string(hours) + "h left";
}

int findTaskIndexById(int taskId) {
    for (int i = 0; i < taskCount; i++) {
        if (taskList[i].id == taskId) return i;
    }
    return -1;
}

void sortTasksByUrgency() {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            bool swapNeeded = false;
            
            if (taskList[j].completed && !taskList[j + 1].completed) {
                swapNeeded = true; 
            } 
            else if (!taskList[j].completed && taskList[j + 1].completed) {
                swapNeeded = false; 
            } 
            else {
                if (taskList[j].deadlineTimestamp > taskList[j + 1].deadlineTimestamp) {
                    swapNeeded = true; 
                } 
                else if (taskList[j].deadlineTimestamp < taskList[j + 1].deadlineTimestamp) {
                    swapNeeded = false;
                } 
                else {
                    if (taskList[j].importance > taskList[j + 1].importance) {
                        swapNeeded = true; 
                    }
                }
            }
            if (swapNeeded) {
                Task temp = taskList[j];
                taskList[j] = taskList[j + 1];
                taskList[j + 1] = temp;
            }
        }
    }
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        cout << "\n" << RED << "[Error] Planner database registry allocation full!" << RESET << "\n";
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
    
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : ";
        getline(cin, newTask.title);
        if (newTask.title.empty()) ifEmpty();
        // Break out and return to appMenu if user types 'exit'
        else if (checkExit(newTask.title)) {
            cout << YELLOW << "\n[Notice] Secure registry formulation abandoned. Returning...\n" << RESET;
            return;
        }
        else break;
    }
    
    cout << "Enter Task's Description : ";
    getline(cin, newTask.description);
    // Break out and return to appMenu if user types 'exit'
    if (checkExit(newTask.description)) {
        cout << YELLOW << "\n[Notice] Secure registry formulation abandoned. Returning...\n" << RESET;
        return;
    }

    int year, month, day, hour, minute;
    string errorMsg = "";
    
    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }

        cout << "    YYYY                 : ";
        if (!(cin >> year)) {
            clearInputBuffer();
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        clearInputBuffer();

        if (year >= currentYear) break;
        else errorMsg = "[Error] Invalid year! Year must be " + to_string(currentYear) + " or above.";
    }

    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }

        cout << "    MM (1-12)            : ";
        if (!(cin >> month)) {
            clearInputBuffer();
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        clearInputBuffer();

        if (month >= 1 && month <= 12 && !(year == currentYear && month < currentMonth)) break;
        else errorMsg = "[Error] Invalid month! Cannot be in the past or outside 1-12.";
    }

    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }

        cout << "    DD                   : ";
        if (!(cin >> day)) {
            clearInputBuffer();
            errorMsg = "[Error] Please enter a valid number!"; continue;
        }
        clearInputBuffer();

        int maxDays = 31;
        if (month == 4 || month == 6 || month == 9 || month == 11) maxDays = 30;
        else if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            maxDays = isLeap ? 29 : 28;
        }

        if (day >= 1 && day <= maxDays && !(year == currentYear && month == currentMonth && day < currentDay)) break;
        else errorMsg = "[Error] Invalid day! Max day for this month is " + to_string(maxDays) + " and cannot be in the past.";
    }

    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        cout << "    DD                   : " << day << endl;
        if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }

        cout << "    HH (0-23)            : ";
        if (!(cin >> hour)) {
            clearInputBuffer();
            errorMsg = "[Error] Please enter a valid number!"; 
            continue;
        }
        clearInputBuffer();

        if (hour >= 0 && hour <= 23 && !(year == currentYear && month == currentMonth && day == currentDay && hour < currentHour)) 
        break;
        else errorMsg = "[Error] Invalid hour! Must be 0-23 and cannot be in the past.";
    }

    while (true) {
        displayHeader("ADD NEW TASK");
        cout << "Enter Task's Title       : " << newTask.title << endl;
        cout << "Enter Task's Description : " << newTask.description << endl;
        cout << "Task's Deadline" << endl;
        cout << "    YYYY                 : " << year << endl;
        cout << "    MM (1-12)            : " << month << endl;
        cout << "    DD                   : " << day << endl;
        cout << "    HH (0-23)            : " << hour << endl;
        if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }

        cout << "    MM (0-59)            : ";
        if (!(cin >> minute)) {
            clearInputBuffer();
            errorMsg = "[Error] Please enter a valid number!"; 
            continue;
        }
        clearInputBuffer();

        if (minute >= 0 && minute <= 59 && !(year == currentYear && month == currentMonth && day == currentDay && hour == currentHour && minute < currentMin)) 
        break;
        else errorMsg = "[Error] Invalid minute! Must be 0-59 and cannot be in the past.";
    }

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

    while (true) {
        cout << "Set Importance Level (" << RED << "High:1" << RESET << " | " << YELLOW << "Medium:2" << RESET << " | " << GREEN << "Low:3" << RESET << "): ";
        if (!(cin >> newTask.importance) || newTask.importance < 1 || newTask.importance > 3) {
            clearInputBuffer();
            cout << RED << "[Error] Value out of range selector map bounds!\n" << RESET; 
            continue;
        }
        clearInputBuffer();
        break;
    }

    newTask.id = nextTaskId++;
    newTask.completed = false;
    newTask.category = "General";
    
    // Verification Prompt
    string confirmation;
    cout << "\n[Verification] Are you sure you want to add this task? ("<< GREEN << "y" << RESET << "/" << RED << "n" << RESET << "): ";
    getline(cin, confirmation);

    // Convert input to lowercase for easier validation
    for (char &c : confirmation) c = tolower(static_cast<unsigned char>(c));

    if (confirmation == "y" || confirmation == "yay") {
        taskList[taskCount++] = newTask;
        saveTasks();
        cout << "\n" << GREEN << "[Success] New task registered securely.\n" << RESET;
    } else if (confirmation == "n" || confirmation == "nay") {
        cout << "\n" << RED << "[Cancelled] Task creation discarded.\n" << RESET;
        nextTaskId--; 
    } else {
        cout << "\n" << RED << "[Error] Invalid input. Task creation cancelled.\n" << RESET;
        nextTaskId--; 
    }
}

void showToast() {
    sortTasksByUrgency();
    time_t now = time(0);
    bool urgent = false;

    for (int i = 0; i < taskCount; i++) {
        if (!taskList[i].completed) {
            int thresholdHours = (taskList[i].importance == 1) ? 24 : ((taskList[i].importance == 2) ? 12 : 6);
            double thresholdSeconds = thresholdHours * 3600.0;
            double diffSeconds = difftime(taskList[i].deadlineTimestamp, now);
            
            if (diffSeconds <= thresholdSeconds) {
                if (!urgent) clearScreen();
                
                long long totalMin = diffSeconds / 60;
                long long hours = totalMin / 60;
                long long mins = totalMin % 60;

                bool overdue = (diffSeconds < 0);
                long long displayHours = abs(hours);
                long long displayMins = abs(mins);

                string safeTitle = (taskList[i].title.length() > 38) ? taskList[i].title.substr(0, 35) + "..." : taskList[i].title;
                string safeDesc = (taskList[i].description.length() > 38) ? taskList[i].description.substr(0, 35) + "..." : taskList[i].description;

                cout << "  .--------------------------------------------------.\n";
                cout << "  | [i] DONEXT Reminder                              |\n";
                cout << "  |--------------------------------------------------|\n";
                cout << "  | Task : " << left << setw(39) << safeTitle << "   |\n";
                cout << "  | Desc : " << left << setw(39) << safeDesc << "   |\n";
                
                cout << "  | Prio : ";
                if (taskList[i].importance == 1)      cout << RED << left << setw(39) << "High" << RESET;
                else if (taskList[i].importance == 2) cout << YELLOW << left << setw(39) << "Medium" << RESET;
                else                                  cout << GREEN << left << setw(39) << "Low" << RESET;
                cout << "   |\n";
                
                cout << "  |                                                  |\n";
                
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
        cout << string(TERMINAL_WIDTH, '=') << RESET << "\n";
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
        string statusStr = taskList[i].completed ? "Completed" : "Pending";
        string titleSummary = (taskList[i].title.length() > 12) ? taskList[i].title.substr(0, 9) + "..." : taskList[i].title;
        string timeLeftStr = calculateTimeLeft(taskList[i].deadlineTimestamp);

        cout << left << setw(4)  << taskList[i].id 
             << setw(15) << titleSummary 
             << setw(18) << taskList[i].dueDateStr 
             << setw(15) << timeLeftStr;
             
        if (taskList[i].importance == 1)      cout << RED << setw(10) << "High" << RESET;
        else if (taskList[i].importance == 2) cout << YELLOW << setw(10) << "Medium" << RESET;
        else                                  cout << GREEN << setw(10) << "Low" << RESET;

        cout << statusStr << endl;
    }
    cout << string(TERMINAL_WIDTH, '=') << RESET << "\n";
}

void modifyTask() {
    displayTasks();
    if (taskCount == 0) return;

    int idChoice;
    cout << "\nEnter the Task ID to alter parameters: ";
    if (!(cin >> idChoice)) { clearInputBuffer(); return; }
    clearInputBuffer();

    int index = findTaskIndexById(idChoice);
    if (index == -1) { cout << RED << "[Error] ID reference not found.\n" << RESET; return; }

    Task *taskPtr = &taskList[index];
    
    string options[] = {
        "Toggle Progress State (Complete/Pending)",
        "Rewrite Title and Description",
        "Edit Deadline (Date & Time)",
        "Change Importance Level",
        "Cancel & Go Back"
    };

    int subChoice = navigateMenu("MODIFY TASK: " + taskPtr->title, options, 5);
    
    if (subChoice == 4) return;
    else if (subChoice == 0) {
        taskPtr->completed = !taskPtr->completed;
        saveTasks();
        cout << "\n" << GREEN << "[Success] Status changed.\n" << RESET;
    } 
    else if (subChoice == 1) {
        string newTitle, newDesc;
        cout << "New Title: "; getline(cin, newTitle);
        // Safely interrupt if user uses 'exit' keyword
        if (checkExit(newTitle)) {
            cout << YELLOW << "\n[Notice] Operation aborted. Returning to menu...\n" << RESET;
            return;
        }
        
        cout << "New Desc : "; getline(cin, newDesc);
        if (checkExit(newDesc)) {
            cout << YELLOW << "\n[Notice] Operation aborted. Returning to menu...\n" << RESET;
            return;
        }

        taskPtr->title = newTitle;
        taskPtr->description = newDesc;
        saveTasks();
        cout << "\n" << GREEN << "[Success] Content rewritten.\n" << RESET;
    } 
    else if (subChoice == 3) {
        while (true) {
            cout << "Set New Importance Level (" << RED << "High:1" << RESET << " | " << YELLOW << "Medium:2" << RESET << " | " << GREEN << "Low:3" << RESET << "): ";
            if (!(cin >> taskPtr->importance) || taskPtr->importance < 1 || taskPtr->importance > 3) {
                clearInputBuffer();
                cout << RED << "[Error] Value out of range selector map bounds!\n" << RESET; 
                continue;
            }
            clearInputBuffer();
            break;
        }
        saveTasks();
        cout << "\n" << GREEN << "[Success] Importance level adjusted successfully.\n" << RESET;
    }
    else if (subChoice == 2) {
        int year, month, day, hour, minute;
        string errorMsg = "";
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int currentYear = 1900 + ltm->tm_year;
        int currentMonth = 1 + ltm->tm_mon;
        int currentDay = ltm->tm_mday;
        int currentHour = ltm->tm_hour;
        int currentMin = ltm->tm_min;

        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }
            cout << "    Year (YYYY)          : ";
            if (!(cin >> year)) { clearInputBuffer(); errorMsg = "[Error] Integer format only!"; 
                continue; }
            clearInputBuffer();
            if (year >= currentYear) 
                break;
            else errorMsg = "[Error] Invalid year! Year must be " + to_string(currentYear) + " or above.";
        }

        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }
            cout << "    Month (1-12)         : ";
            if (!(cin >> month)) { clearInputBuffer(); errorMsg = "[Error] Integer format only!"; 
                continue; }
            clearInputBuffer();
            if (month >= 1 && month <= 12 && !(year == currentYear && month < currentMonth)) 
                break;
            else errorMsg = "[Error] Invalid month! Cannot be in the the past or outside 1-12.";
        }

        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }
            cout << "    Day (1-31)           : ";
            if (!(cin >> day)) { clearInputBuffer(); errorMsg = "[Error] Integer format only!"; 
                continue; }
            clearInputBuffer();

            int maxDays = 31;
            if (month == 4 || month == 6 || month == 9 || month == 11) maxDays = 30;
            else if (month == 2) {
                bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                maxDays = isLeap ? 29 : 28;
            }
            if (day >= 1 && day <= maxDays && !(year == currentYear && month == currentMonth && day < currentDay)) 
                break;
            else errorMsg = "[Error] Invalid day! Max day for this month is " + to_string(maxDays) + " and cannot be in the past.";
        }

        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            cout << "    Day (1-31)           : " << day << endl;
            if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }
            cout << "    Hour (0-23)          : ";
            if (!(cin >> hour)) { clearInputBuffer(); errorMsg = "[Error] Integer format only!"; 
                continue; }
            clearInputBuffer();
            if (hour >= 0 && hour <= 23 && !(year == currentYear && month == currentMonth && day == currentDay && hour < currentHour)) 
                break;
            else errorMsg = "[Error] Invalid hour! Must be 0-23 and cannot be in the past.";
        }

        while (true) {
            displayHeader("EDIT TASK DEADLINE");
            cout << "Selected Task : " << taskPtr->title << endl << endl;
            cout << "Enter New Deadline Constraints:\n";
            cout << "    Year (YYYY)          : " << year << endl;
            cout << "    Month (1-12)         : " << month << endl;
            cout << "    Day (1-31)           : " << day << endl;
            cout << "    Hour (0-23)          : " << hour << endl;
            if (!errorMsg.empty()) { cout << RED << errorMsg << RESET << "\n"; errorMsg = ""; }
            cout << "    Minute (0-59)        : ";
            if (!(cin >> minute)) { clearInputBuffer(); errorMsg = "[Error] Integer format only!"; 
                continue; }
            clearInputBuffer();
            if (minute >= 0 && minute <= 59 && !(year == currentYear && month == currentMonth && day == currentDay && hour == currentHour && minute < currentMin)) 
                break;
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
        cout << "\n" << GREEN << "[Success] Deadline updated successfully!" << RESET << "\n";
    }
}

void deleteTask() {
    displayTasks();
    if (taskCount == 0) return;

    int idChoice;
    cout << "\nEnter Target Erasure Task ID: ";
    if (!(cin >> idChoice)) { clearInputBuffer(); return; }
    clearInputBuffer();

    int index = findTaskIndexById(idChoice);
    if (index == -1) { 
        cout << RED << "[Error] Reference not identified.\n" << RESET;
        return;
    }
    for (int i = index; i < taskCount - 1; i++) {
        taskList[i] = taskList[i + 1];
    }
    taskCount--;
    saveTasks();
    cout << GREEN << "[Success] Element deleted successfully.\n" << RESET;
}

string getTaskFileName() {
    return currentUser + "_tasks.txt";
}

void appMenu() {
    string options[] = {
        "Add New Task",
        "View Sorted Agenda",
        "Modify Existing Properties",
        "Remove Records",
        "Search Task",
        "Filter Tasks",
        "Exit / Logout"
    };

    while (true) {
        int opt = navigateMenu("DONEXT: MAIN MENU", options, 7);
        if (opt == 6) {
            currentUser = "";
            taskCount = 0;
            break; 
        }

        switch (opt) {
            case 0: addTask(); 
                break;
            case 1: displayTasks(); 
                break;
            case 2: modifyTask(); 
                break;
            case 3: deleteTask(); 
                break;
            case 4: searchTask(); 
                break;
            case 5: filterTasks(); 
                break;
        }
        cout << "\nPress Enter to return to Main Menu...";
        cin.get();
    }
}

void saveTasks() {
    ofstream file(getTaskFileName());
    if (!file.is_open()) {
        cout << RED << "[Error] Cannot save tasks!\n" << RESET;
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

int main() {
    showAdminMenu();
    return 0;
}
