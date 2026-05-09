#include "pch.h"
#include "task.h"
#include "timeManager.h"

Task::Task(int id, const string& title, int ttl, const string& creator, const string& assignee, int priority) {
    this->id = id;
    this->title = title;
    this->status = 0; 
    this->createdAt = TimeManager::getCurrentTime();
    this->updatedAt = createdAt;
    this->ttl = ttl;
    this->creator = creator;
    this->assignee = assignee;
    this->delegatedBy = "";
    this->priority = priority;
}

int Task::getId() const { return id; }
string Task::getTitle() const { return title; }
int Task::getStatus() const { return status; }
time_t Task::getCreatedAt() const { return createdAt; }
time_t Task::getUpdatedAt() const { return updatedAt; }
int Task::getTTL() const { return ttl; }
string Task::getCreator() const { return creator; }
string Task::getAssignee() const { return assignee; }
string Task::getDelegatedBy() const { return delegatedBy; }
int Task::getPriority() const { return priority; }

void Task::setPriority(int newPriority) {
    if (newPriority >= 0 && newPriority <= 2) {
        priority = newPriority;
        updatedAt = TimeManager::getCurrentTime(); 
    } 
    else {
        cout << "Invalid priority value. Priority must be between 0 and 2!\n";
    }
}

void Task::setDelegatedBy(const string& newDelegatedBy) {
    delegatedBy = newDelegatedBy;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setStatus(int newStatus) {
    if (newStatus >= 0 && newStatus <= 4) {
        status = newStatus;
        updatedAt = TimeManager::getCurrentTime();  
    } 
    else {
        cout << "Invalid status value. Status must be between 0 and 4!\n";
    }
}

void Task::setAssignee(const string& newAssignee) {
    assignee = newAssignee;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setCreator(const string& newCreator) {
    creator = newCreator;
    updatedAt = TimeManager::getCurrentTime();
}

void Task::setTTL(int newTTL) {
    ttl = newTTL;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setTitle(const string& newTitle) {
    title = newTitle;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setId(int newId) {
    id = newId;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setCreatedAt(time_t newCreatedAt) {
    createdAt = newCreatedAt;
    updatedAt = TimeManager::getCurrentTime(); 
}

void Task::setUpdatedAt(time_t newUpdatedAt) {
    updatedAt = newUpdatedAt;
}

bool Task::isExpired() const {
    return TimeManager::isExpired(createdAt, ttl);
}

void Task::transitionStatus(int newStatus) {
    if (status != newStatus) {
        setStatus(newStatus);  
    }
}

void Task::printDetails() const {            
    cout << "Task ID: " << id << "\n"
         << "Title: " << title << "\n"
         << "Status: " << TASK_STATUS_STRINGS[status] << "\n"
         << "Created At: " << ctime(&createdAt)
         << "Updated At: " << ctime(&updatedAt)
         << "TTL (seconds): " << ttl << "\n"
         << "Creator: " << creator << "\n"
         << "Assignee: " << assignee << "\n"
         << "Delegated By: " << delegatedBy << "\n"
         << "Priority: " << TASK_PRIORITY_STRINGS[priority] << "\n";
}        

void Task::saveToFile(ofstream& outFile) const {
    outFile << id << "|"
            << title << "|"
            << status << "|"
            << createdAt << "|"
            << updatedAt << "|"
            << ttl << "|"
            << creator << "|"
            << assignee << "|"
            << delegatedBy << "|"
            << priority << "\n";
}

Task Task::loadFromFile(ifstream& inFile) {
    string idStr, titleStr, statusStr, createdStr, updatedStr, ttlStr, creatorStr, assigneeStr, priorityStr;

    getline(inFile, idStr, '|');
    getline(inFile, titleStr, '|');
    getline(inFile, statusStr, '|');
    getline(inFile, createdStr, '|');
    getline(inFile, updatedStr, '|');
    getline(inFile, ttlStr, '|');
    getline(inFile, creatorStr, '|');
    getline(inFile, assigneeStr, '|');
    getline(inFile, priorityStr);

    int id = stoi(idStr);
    int status = stoi(statusStr);
    time_t created = stol(createdStr);
    time_t updated = stol(updatedStr);
    int ttl = stoi(ttlStr);
    int priority = stoi(priorityStr);

    Task t(id, titleStr, ttl, creatorStr, assigneeStr);
    t.setStatus(status);
    t.setCreatedAt(created);
    t.setUpdatedAt(updated);
    return t;
}