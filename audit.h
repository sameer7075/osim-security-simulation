#pragma once
#include "pch.h"
#include "utilities.h"
#include "user.h"
#include "userManager.h"

class Audit {
private:
    string username;
    string role;
    string action;
    string status;
    time_t timestamp;
public:
    Audit();
    Audit(const string& user, const string& role, const string& action, const string& status);
    static void audit(string user , string role, string action,  string status);
    void writeLog();
    static void viewLog(const User* currentUser);
    friend ostream& operator<<(ostream& out, const Audit& a);
    void setTimestamp(time_t ts);
    string getUsername() const;
    string getRole() const;
    string getAction() const;
    string getStatus() const;
    time_t getTimestamp() const;  
};
