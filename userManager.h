#pragma once
#include "pch.h"

class User;

class UserManager {
private:
    User** emp;
    int capacity;
    int currentemp;
    UserManager();
    static UserManager* instance;

public:

    static UserManager* getInstance();
    
    void addUser(User* e);
    void resize(int newCapacity);
    User* getUser(string username);
    int getUserCount();
    User** getAllUsers();
    void removeEmp(const User* currentUser);
    void loadFromFile();
    bool authenticate(const string username, const string password, const string role);
    void saveToFile();
    bool userExists(const string& username);
    void displayAllUsers(const User* user);
    void createUser(const User* currentUser);
    void changePassword(User* currentUser);
    bool checkOtpEnabled(const User* currentUser);
    void enableOtp(const User* currentUser);
    void disableOtp(const User* currentUser);
    void addMFA(const User* currentUser);
    void removeOtpPreference(const string& username) ;
    ~UserManager();
};
