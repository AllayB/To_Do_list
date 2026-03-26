#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>  
#include <ctime>

using namespace std;

struct Task {
    string title;
    int priority;
    string description;
    tm datetime;
};

time_t toTimeT(tm t) {
    return mktime(&t);
}


tm createDateTime() {
    tm t = {};
    cout << "Enter year: ";
    cin >> t.tm_year;
    cout << "Month: ";
    cin >> t.tm_mon;
    cout << "Day: ";
    cin >> t.tm_mday;
    cout << "Hour: ";
    cin >> t.tm_hour;
    cout << "Minute: ";
    cin >> t.tm_min;

    t.tm_year -= 1900;
    t.tm_mon -= 1;

    return t;
}

void printTask(const Task& t, int index) {
    cout << index << ". " << t.title
        << " | Priority: " << t.priority
        << " | Date: "
        << t.datetime.tm_mday << "."
        << t.datetime.tm_mon + 1 << "."
        << t.datetime.tm_year + 1900
        << " " << t.datetime.tm_hour << ":"
        << t.datetime.tm_min << endl;
    cout << "Description: " << t.description << endl;
}


void saveToFile(vector<Task>& tasks) {
    ofstream file("tasks.txt");

    for (auto& t : tasks) {
        file << t.title << "|"
            << t.priority << "|"
            << t.description << "|"
            << t.datetime.tm_year << " "
            << t.datetime.tm_mon << " "
            << t.datetime.tm_mday << " "
            << t.datetime.tm_hour << " "
            << t.datetime.tm_min << endl;
    }
}


void loadFromFile(vector<Task>& tasks) {
    ifstream file("tasks.txt");

    if (!file.is_open()) return;

    Task t;
    while (getline(file, t.title, '|')) {
        file >> t.priority;
        file.ignore();

        getline(file, t.description, '|');

        file >> t.datetime.tm_year
            >> t.datetime.tm_mon
            >> t.datetime.tm_mday
            >> t.datetime.tm_hour
            >> t.datetime.tm_min;
        file.ignore();

        tasks.push_back(t);
    }
}

void addTask(vector<Task>& tasks) {
    Task t;
    cin.ignore();

    cout << "Title: ";
    getline(cin, t.title);

    cout << "Priority (1-5): ";
    cin >> t.priority;
    cin.ignore();

    cout << "Description: ";
    getline(cin, t.description);

    t.datetime = createDateTime();

    tasks.push_back(t);
    saveToFile(tasks);
}

void deleteTask(vector<Task>& tasks) {
    int index;
    cout << "Enter task index to delete: ";
    cin >> index;

    if (index >= 0 && index < tasks.size()) {
        tasks.erase(tasks.begin() + index);
        saveToFile(tasks);
    }
}

void editTask(vector<Task>& tasks) {
    int index;
    cout << "Enter task index: ";
    cin >> index;
    cin.ignore();

    if (index >= 0 && index < tasks.size()) {
        cout << "New title: ";
        getline(cin, tasks[index].title);

        cout << "New priority: ";
        cin >> tasks[index].priority;
        cin.ignore();

        cout << "New description: ";
        getline(cin, tasks[index].description);

        tasks[index].datetime = createDateTime();

        saveToFile(tasks);
    }
}

void searchTasks(vector<Task>& tasks) {
    string keyword;
    cin.ignore();

    cout << "Enter keyword: ";
    getline(cin, keyword);

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].title.find(keyword) != string::npos ||
            tasks[i].description.find(keyword) != string::npos) {
            printTask(tasks[i], i);
        }
    }
}

bool compareByPriority(Task a, Task b) {
    return a.priority < b.priority;
}

bool compareByDate(Task a, Task b) {
    return mktime(&a.datetime) < mktime(&b.datetime);
}

void showTasks(vector<Task>& tasks) {
    int choice;
    cout << "1. Sort by priority\n2. Sort by date\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        sort(tasks.begin(), tasks.end(), compareByPriority);
    }
    else {
        sort(tasks.begin(), tasks.end(), compareByDate);
    }

    for (int i = 0; i < tasks.size(); i++) {
        printTask(tasks[i], i);
    }
}

int main() {
    vector<Task> tasks;
    loadFromFile(tasks);

    int choice;

    while (true) {
        cout << "\n\033[032m 1. Add Task\n\033[31m 2. Delete Task\n\033[33m 3. Edit Task\n\033[0m 4. Search\n\033[36m 5. Show Tasks\n\033[0m \033[1m0. Exit\n \033[0mChoice: ";
        cin >> choice;

        switch (choice) {
        case 1: addTask(tasks); break;
        case 2: deleteTask(tasks); break;
        case 3: editTask(tasks); break;
        case 4: searchTasks(tasks); break;
        case 5: showTasks(tasks); break;
        case 0: return 0;
        }
    }
}