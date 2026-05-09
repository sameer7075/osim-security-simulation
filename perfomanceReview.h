#pragma once
#include "pch.h"

#include "task.h"


class User;

class PerformanceReview {
    string name;
    string role;
    int tasksCompleted;
    int tasksOverdue;
    int messagesSent;
    int alertMessages;
    int privateMessages;
    int infoMessages;
    double avgTaskCompletionTime;
    long long totalCompletionTime;

public:
    PerformanceReview(string n, string r);

    void logTaskCompletion(const Task* task);
    void logOverdueTask();
    void logMessage(const string& type);
    void saveToFile();
    void loadFromFile();

    friend ostream& operator<<(ostream& out, const PerformanceReview& pr);
};
