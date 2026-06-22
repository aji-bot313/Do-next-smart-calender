
# Table of Contents

* [📖 Overview](https://www.google.com/search?q=%23-overview)
* [✨ Features](https://www.google.com/search?q=%23-features)
* [🧠 Programming Concepts](https://www.google.com/search?q=%23-programming-concepts)
* [⚙️ System Flow](https://www.google.com/search?q=%23-system-flow)
* [📂 Project Structure](https://www.google.com/search?q=%23-project-structure)
* [🚀 Installation](https://www.google.com/search?q=%23-installation)
* [🖥️ Usage](https://www.google.com/search?q=%23-usage)
* [🔮 Future Improvements](https://www.google.com/search?q=%23-future-improvements)
* [👥 Contributors](https://www.google.com/search?q=%23-contributors)
* [📄 License](https://www.google.com/search?q=%23-license)

## 📖 Overview

**DONEXT Task Management System** is a dynamic, terminal-based personal productivity application developed using C++.

The name **DONEXT** represents the application's core philosophy: always knowing exactly what you need to do next. The application helps users organize their daily agendas by automatically ranking tasks based on urgency, importance, and impending deadlines.

This system provides a highly interactive command-line interface featuring arrow-key navigation, dynamic ANSI color formatting, and real-time deadline calculations that allow users to:

* Register and authenticate secure personal accounts
* Create detailed tasks with strict date and time constraints
* Automatically sort agendas based on proximity to deadlines
* Receive "toast" pop-up notifications for urgent, impending tasks
* Modify, complete, and filter tasks by priority or status
* Safely store data persistently across sessions using file I/O

This project was developed for educational purposes to implement software engineering fundamentals and demonstrate programming concepts such as functions, arrays, structures, pointers, control statements, looping, string manipulation, time math (`<ctime>`), and persistent file handling in C++.

## ✨ Features

**🔑 Authentication System**

* Sign Up
* Sign In
* Credential validation
* Login attempt limitation (Max 3 attempts)
* Secure, user-specific data isolation

**📋 Task Management (CRUD)**

* Add new tasks with precise timestamps (YYYY-MM-DD HH:MM)
* Rewrite titles and descriptions
* Update task importance/priority
* Toggle completion status
* Delete outdated or incorrect records

**⏱️ Time & Deadline Engine**

* Real-time system clock integration
* Automatic "Time Left" countdown calculations (e.g., "3d 4h left")
* "Overdue" status tracking
* Automatic urgency-based sorting

**🔔 Notification System**

* DONEXT Reminder pop-ups on login
* Dynamic urgency thresholds (12 hours for High/Medium priority, 6 hours for Low)

**🔎 Search & Filter Tools**

* Keyword-based title search (case-insensitive)
* Filter by Category/Priority (High, Medium, Low)
* Filter by Status (Completed vs. Pending)

**🖥️ Interactive UI/UX**

* Seamless arrow-key menu navigation
* Dynamic ASCII art headers
* ANSI-colored status text and error handling
* Safe exit/cancel routines via keyword interrupts

## 🧠 Programming Concepts

| Feature | Basic Programming | Looping | Statement Control | Array | Function | Struct | Pointer | File I/O |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Authentication System | ✓ | ✓ | ✓ | ✓ | ✓ | - | - | ✓ |
| Arrow-Key UI Engine | ✓ | ✓ | ✓ | ✓ | ✓ | - | - | - |
| Task Management | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Search & Filtering | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | - | - |
| Notification Engine | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | - | - |
| Time Calculation | ✓ | - | ✓ | - | ✓ | ✓ | - | - |
| Main System | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | - | ✓ |

## ⚙️ System Flow

```text
Start
 │
 ├── Sign Up (Creates Credentials in users.txt)
 │
 ├── Sign In (Authenticates Credentials)
 │
 └── DONEXT Main Menu (Loads user_tasks.txt)
      │
      ├── Urgent Task Notifications (Triggered on login)
      │
      ├── Add New Task
      ├── View Sorted Agenda
      ├── Modify Existing Properties
      ├── Remove Records
      ├── Search Task
      ├── Filter Tasks
      └── Exit / Logout

```

## 📂 Project Structure

```text
DONEXT/
│
├── main.cpp                 # Main application source code
├── users.txt                # Secure database for user credentials (auto-generated)
├── [username]_tasks.txt     # User-specific persistent task databases (auto-generated)
└── README.md                # Project documentation

```

**Single-file architecture with internal functional grouping:**

* Libraries & Macros (ANSI Colors, Constants)
* Global Data Structures (`Task` struct, global state variables)
* Utility & Navigation Helpers (`MapsMenu`, `clearInputBuffer`, `checkExit`)
* Search & Filter Algorithms
* Authentication Modules (`login`, `registration`, `saveUser`)
* Core UI Handlers (`displayHeader`, `showToast`)
* Time Math Helpers (`calculateTimeLeft`, `sortTasksByUrgency`)
* Core CRUD Functions (`addTask`, `displayTasks`, `modifyTask`, `deleteTask`)
* File I/O Modules (`saveTasks`, `loadTasks`)
* Application Loop (`appMenu`, `main`)

## 🚀 Installation

**1. Clone Repository**

```bash
git clone https://github.com/yourusername/donext.git
cd donext

```

**2. Compile**

```bash
g++ main.cpp -o donext

```

**3. Run**

*Windows:*

```bash
donext.exe

```

*Linux / Mac:*

```bash
./donext

```

*(Note: Because this application utilizes `<conio.h>` for the `_getch()` arrow-key navigation, compiling on non-Windows environments may require a specialized compiler or swapping to a cross-platform console library like `ncurses`.)*

## 🖥️ Usage

1. **Launch the application** via your terminal.
2. **Create an account** by navigating to "Register New Account".
3. **Login into the system** using your new credentials.
4. Review any **Urgent Notifications** that pop up on the screen.
5. Use the **Up/Down Arrow Keys** and **Enter** to navigate the interactive menu.
6. **Add tasks**, set rigid deadlines, and assign priority levels.
7. Return to the Main Menu to view your **Sorted Agenda**.
8. Safely **Exit / Logout** to ensure all tasks are saved to your personal `.txt` file.

## 🔮 Future Improvements

* Migration from `.txt` files to a structured database (SQL/SQLite)
* Cross-platform compatibility removal of `<conio.h>` dependency
* Recurring task functionality (Daily/Weekly/Monthly)
* Calendar grid visualizer
* Export agenda to CSV functionality
* Sub-tasks and checklists within main tasks

## 👥 Contributors

| Name | Role |
| --- | --- |
| AIJIAZI HOUSAIYIN | GROUP LEADER & Developer |
| BAYU KHULDUN ARDIANSYAH | Developer |
| MUHAMMAD FEBRYAN SYACH FAHLEVI | Developer |
| JOVANKA KIARA REVALIN SEPTYAVANYA | Developer |
| AZZAHRA SHAFIRA RACHMAN | Developer |
| RIFKA SALSABILA FUTRI | Developer |
| I GUSTI AGUNG AYU PRISILIA | Developer |

## 📄 License

This project was developed for educational purposes only.
