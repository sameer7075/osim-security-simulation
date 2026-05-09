#pragma once
#include "pch.h"
#include "utilities.h"

class notification {
    private:
    int id;
    string username;
    string type;
    string message;
    time_t time;

    public:
    notification(int pid, string u, string t, string m, time_t tm);

    int getid();
    string getusername();
    string gettype();
    string getmessage();
    time_t gettime();
};
