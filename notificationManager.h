#pragma once
#include "pch.h"
#include "notification.h"

class User;

class NotificationManager
{
private:
    notification** n;
    int capacity;
    int currentnotis;
    int nextid = 0;
    NotificationManager();
    static NotificationManager* instance;
public:
    static NotificationManager* getInstance();
    
    void addNoti(notification* noti);
    void resize(int newCapacity);
    notification* getnotification(int searchID);
    void createNoti(const User* user);
    void removenoti(const User* user);
    void loadFromFile();
    void saveToFile();
    bool notificationExists(const int& sID);
    void showallnotis();
    ~NotificationManager();
};
