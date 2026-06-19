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