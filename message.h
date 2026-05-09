#pragma once

#include "pch.h"

class Message {
private:
    int id;
    string sender;
    string receiver;
    string content;
    string type;  // "INFO", "PRIVATE", or "ALERT"
    time_t timestamp;

public:
    Message(int id, const string& sender, const string& reciver, const string& content, const string& type, const time_t& timestamp = time(nullptr));

    int getId() const;
    string getSender() const;
    string getReceiver() const;
    string getContent() const;
    string getType() const;
    time_t getTimestamp() const;

    string encryptContent() const;
    string decryptContent() const;

};