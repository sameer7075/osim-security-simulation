#pragma once

class Task {
private:
    int id;
    string title;
    int status;        //0 = Created, 1 = Assigned, 2 = InProgress, 3 = Completed, 4 = Expired
    time_t createdAt;
    int ttl;      
    time_t updatedAt;  
    string assignee;
    string creator;
    string delegatedBy;
    int priority; // 0 = Low, 1 = Medium, 2 = High

public:
    Task(int id, const string& title, int ttl, const string& creator, const string& assignee, int priority = 0);

    int getId() const;
    string getTitle() const;
    int getStatus() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    int getTTL() const;
    string getCreator() const;
    string getAssignee() const;
    string getDelegatedBy() const;
    int getPriority() const;

    void setStatus(int newStatus);
    void setAssignee(const string& newAssignee);
    void setCreator(const string& newCreator);
    void setTTL(int newTTL);
    void setTitle(const string& newTitle);
    void setId(int newId);
    void setCreatedAt(time_t newCreatedAt);
    void setUpdatedAt(time_t newUpdatedAt);
    void setDelegatedBy(const string& newDelegatedBy);
    void setPriority(int newPriority);

    bool isExpired() const;
    void transitionStatus(int newStatus);
    void printDetails() const;
    void saveToFile(ofstream& outFile) const;
    static Task loadFromFile(ifstream& inFile);
};
