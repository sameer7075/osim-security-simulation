#pragma once
#include "pch.h"
#include "utilities.h"
#include "perfomanceReview.h"

class User {
protected:
    string username;
    string password;
    int clearancelevel;
    PerformanceReview* perfomancereview;

public:
    User(string u, string p, int l);
    void show() const;
    int getclearancelevel() const;
    string getName() const;
    string getHashedPassword() const;
    virtual string getRole() const = 0;
    PerformanceReview* getPerformanceReview() const;
    void changePassword();
    virtual ~User();
};

class Junior : public User {
public:
    Junior(string u, string p);
    string getRole() const override;
    ~Junior() override ;
};

class Employee : public User {
public:
    Employee(string u, string p);
    string getRole() const override;
    ~Employee() override ;
};

class Manager : public User {
public:
    Manager(string u, string p);
    string getRole() const override;
    ~Manager() override ;
};

class Director : public User {
public:
    Director(string u, string p);
    string getRole() const override;
    ~Director() override ;
};

class Executive : public User {
public:
    Executive(string u, string p);
    string getRole() const override;
    ~Executive() override ;
};
