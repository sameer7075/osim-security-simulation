#include "pch.h"
#include "notification.h"

notification::notification(int pid, string u,  string t, string m, time_t tm) {
    id = pid;
    username = u;
    type = t;
    message = m;
    time = tm;
}

int notification::getid() { return id; }
time_t notification::gettime() { return time; }
string notification::getusername() { return username; }
string notification::gettype() { return type; }
string notification::getmessage() { return message ; }
