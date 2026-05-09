#pragma once

#include "message.h"
#include "user.h"
#include "userManager.h"

class MessageManager {
private:
    Message** messages;
    int size;
    int capacity;
    int nextId;

    MessageManager();
    static MessageManager* instance;

public:
    static MessageManager* getInstance();    

    ~MessageManager();

    void resize(int newCapacity);
    void loadFromFile();
    void saveToFile();
    void addMessage(Message* message);

    void sendMessage(const User* sender, const User* recipient, const string& type);

    void broadcastInfo(const User* sender, const string& targetRole, const string& content);

    void printInbox(const User* currentUser);

    void printOutbox(const User* currentUser);

    void deleteMessageById(const User* user, int messageId) ;
};
