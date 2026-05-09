#include "perfomanceReview.h"

PerformanceReview::PerformanceReview(string n, string r)
    : name(n), role(r), tasksCompleted(0), tasksOverdue(0),
      messagesSent(0), alertMessages(0), privateMessages(0),
      infoMessages(0), avgTaskCompletionTime(0.0), totalCompletionTime(0) {
        loadFromFile();
}

void PerformanceReview::logTaskCompletion(const Task* task) {
    tasksCompleted++;
    totalCompletionTime += (time(nullptr) - task->getCreatedAt());
    avgTaskCompletionTime = static_cast<double>(totalCompletionTime) / tasksCompleted;
    saveToFile();
}

void PerformanceReview::logOverdueTask() {
    tasksOverdue++;
}

void PerformanceReview::logMessage(const string& type) {
    messagesSent++;
    if (type == "ALERT") alertMessages++;
    else if (type == "PRIVATE") privateMessages++;
    else if (type == "INFO") infoMessages++;
    saveToFile();
}

ostream& operator<<(ostream& out, const PerformanceReview& pr) {
    int score = pr.tasksCompleted * 10 - pr.tasksOverdue * 5;
    if (pr.avgTaskCompletionTime > 0) {
        score -= static_cast<int>(pr.avgTaskCompletionTime / 60); 
    }
    score += pr.messagesSent; 
    if(score > 100)score = 100;
    else if(score < 0) score = 0;
    out << "\nPerformance Report for " << pr.name << " (" << pr.role << ")\n";
    out << "-------------------------------------------------\n";
    out << "Tasks Completed        : " << pr.tasksCompleted << "\n";
    out << "Tasks Overdue          : " << pr.tasksOverdue << "\n";
    out << "Average Completion Time: " << pr.avgTaskCompletionTime << " seconds\n";
    out << "Messages Sent          : " << pr.messagesSent << "\n";
    out << "  - ALERT              : " << pr.alertMessages << "\n";
    out << "  - PRIVATE            : " << pr.privateMessages << "\n";
    out << "  - INFO               : " << pr.infoMessages << "\n";
    out << "Performance Score      : " << score << " / 100\n";
    out << "-------------------------------------------------\n";
    return out;
}

void PerformanceReview::saveToFile() {
    ifstream infile("performanceLog.txt");
    ofstream temp("temp.txt");

    string fileName, fileRole;
    bool found = false;
    int tCompleted, tOverdue, msgSent, alertMsg, privateMsg, infoMsg;
    double avgTime;
    long long totalTime;

    while (infile >> fileName >> fileRole >> tCompleted >> tOverdue >> msgSent >> alertMsg >> privateMsg >> infoMsg >> avgTime >> totalTime) {
        
        if (name == fileName && role == fileRole)  {            
            temp << name << " "
                 << role << " "
                 << tasksCompleted << " "
                 << tasksOverdue << " "
                 << messagesSent << " "
                 << alertMessages << " "
                 << privateMessages << " "
                 << infoMessages << " "
                 << avgTaskCompletionTime << " "
                 << totalCompletionTime << "\n";
            found = true;
        } 
        else {            
            temp << fileName << " "
                 << fileRole << " "
                 << tCompleted << " "
                 << tOverdue << " "
                 << msgSent << " "
                 << alertMsg << " "
                 << privateMsg << " "
                 << infoMsg << " "
                 << avgTime << " "
                 << totalTime << "\n";
        }
    }

    infile.close();

    if (!found) {        
        temp << name << " "
             << role << " "
             << tasksCompleted << " "
             << tasksOverdue << " "
             << messagesSent << " "
             << alertMessages << " "
             << privateMessages << " "
             << infoMessages << " "
             << avgTaskCompletionTime << " "
             << totalCompletionTime << "\n";
    }

    temp.close();
    remove("performanceLog.txt");
    rename("temp.txt", "performanceLog.txt");
}

void PerformanceReview::loadFromFile() {
    ifstream infile("performanceLog.txt");
    int tCompleted, tOverdue, msgSent, alertMsg, privateMsg, infoMsg;
    double avgTime;
    long long totalTime;

    string fileName, fileRole;
    while (infile >> fileName >> fileRole >> tCompleted >> tOverdue >> msgSent >> alertMsg >> privateMsg >> infoMsg >> avgTime >> totalTime) {

        if (name == fileName && role == fileRole) {
            tasksCompleted = tCompleted;
            tasksOverdue = tOverdue;
            messagesSent = msgSent;
            alertMessages = alertMsg;
            privateMessages = privateMsg;
            infoMessages = infoMsg;
            avgTaskCompletionTime = avgTime;
            totalCompletionTime = totalTime;
            break;
        }
    }

    infile.close();
}


