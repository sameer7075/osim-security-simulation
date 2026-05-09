#include "messageManager.h"
#include "policyEngine.h"
#include "audit.h"

MessageManager* MessageManager::instance = nullptr;

MessageManager::MessageManager() {
    capacity = 10;
    size = 0;
    messages = new Message*[capacity];
    loadFromFile();
    nextId = 0;
    for (int i = 0; i < size; ++i) {
        if (messages[i]->getId() > nextId) {
            nextId = messages[i]->getId();  
        }
    }
    nextId++;
}

MessageManager::~MessageManager() {
    saveToFile();
    for (int i = 0; i < size; ++i) {
        delete messages[i]; 
    }
    delete[] messages; 
}

MessageManager* MessageManager::getInstance() {
    if (instance == nullptr) {  
        instance = new MessageManager();
    }
    return instance; 
}

void MessageManager::resize(int newCapacity) {
    Message** newMessages = new Message*[newCapacity];
    for (int i = 0; i < size; ++i) {
        newMessages[i] = messages[i];
    }
    delete[] messages;
    messages = newMessages;
    capacity = newCapacity;
}

void MessageManager::addMessage(Message* message) { 
    if (size == capacity) {
        resize(capacity * 2);
    }
    messages[size++] = message;
    saveToFile();
}


void MessageManager::loadFromFile() {
    ifstream o1("inbox.txt");
    if (!o1) {
        cerr << "Could not open inbox.txt for reading.\n";
        return;
    }
    string idStr, sender, receiver, type, timestampStr, content;

    while (getline(o1, idStr, '|') &&
           getline(o1, sender, '|') &&
           getline(o1, receiver, '|') &&
           getline(o1, type, '|') &&
           getline(o1, content, '|') &&
           getline(o1, timestampStr)){   
        
        int id = stoi(idStr);
        time_t timestamp = stol(timestampStr);
        if(type == "PRIVATE"){
            content = decryptMessage(content);
        }        

        Message* msg = new Message(id, sender, receiver, content, type, timestamp);

        addMessage(msg);
    }

    o1.close();
}

void MessageManager::saveToFile() {
    ofstream out("inbox.txt", ios::trunc);
    if (out) {
        for (int i = 0; i < size; ++i) {
            string tempContent = "";
            if(messages[i]->getType() == "PRIVATE"){
                tempContent = encryptMessage(messages[i]->getContent());
            }
            else{
                tempContent = messages[i]->getContent();
            }
            out << messages[i]->getId() << "|"
                << messages[i]->getSender() << "|"
                << messages[i]->getReceiver() << "|"
                << messages[i]->getType() << "|"
                << tempContent << "|"
                << messages[i]->getTimestamp() << "\n";
        }
        out.close();
    }
}


void MessageManager::sendMessage(const User* sender, const User* receiver, const string& type) {    
    if(PolicyEngine::isMessageAllowed(sender, receiver, type)) {
        string content;
        cout << "Enter message content: ";
        getline(cin, content);
        Message* msg = new Message(nextId++, sender->getName(), receiver->getName(), content, type);
        if(msg->getType() == "PRIVATE") {
            sender->getPerformanceReview()->logMessage("PRIVATE");
            addMessage(msg);
            Audit::audit(sender->getName(), sender->getRole(), "Sent a private message", "Success");
            cout << "\nMessage sent successfully!\n";
        }
        else {
            addMessage(msg);
            sender->getPerformanceReview()->logMessage("ALERT");
            Audit::audit(sender->getName(), sender->getRole(), "Sent an alert message", "Success");
            cout << "\nMessage sent successfully!\n";
        }
    } 
    else {
        cout << "\nYou are not authorized to send this message.\n";
        Audit::audit(sender->getName(), sender->getRole(), "Attempted to send a message", "Failed");
    }
}

void MessageManager::broadcastInfo(const User* sender, const string& targetRole, const string& content) {
    bool oneSend = false;
    if(PolicyEngine::isBroadcastInfoAllowed(sender, targetRole)) {   
        string content;
        cout << "Enter message content: ";
        cin.ignore();
        getline(cin, content);   
        for (int i = 0; i < UserManager::getInstance()->getUserCount(); ++i) {
            User* user = UserManager::getInstance()->getAllUsers()[i];
            if ((user->getRole() == targetRole || targetRole == "ALL") && user->getName() != sender->getName()) {
                Message* msg = new Message(nextId++, sender->getName(), user->getName(), content, "INFO"); 
                addMessage(msg);
                sender->getPerformanceReview()->logMessage("INFO");
                Audit::audit(sender->getName(), sender->getRole(), "Broadcasted an info message", "Success");
                oneSend = true;
            }
        }
        if(oneSend){
            cout << "\nInfo message broadcasted successfully!\n";
        }    
        else{
            cout << "\nNo users found with the specified role.\n";
            Audit::audit(sender->getName(), sender->getRole(), "Attempted to broadcast an info message", "Failed");
        } 
    }
    else {
        cout << "\nYou are not authorized to broadcast this info message!\n";
        Audit::audit(sender->getName(), sender->getRole(), "Attempted to broadcast an info message", "Failed");
    }
}

void MessageManager::printInbox(const User* currentUser) {
    int count = 1;
    cout << "\n--- INBOX ---\n";
    for (int i = 0; i < size; ++i) {
        if (messages[i]->getReceiver() == currentUser->getName()) {
            time_t temp = messages[i]->getTimestamp();
            cout << "\nMessage " << count++ << ":\n";
            cout << "------------------------\n";
            cout << "ID: " << messages[i]->getId() << "\n";
            cout << "From: " << messages[i]->getSender() << "\n";
            cout << "To: " << messages[i]->getReceiver() << "\n";
            cout << "Type: " << messages[i]->getType() << "\n";
            cout << "Timestamp: " << ctime(&temp);
            cout << "Content: " << messages[i]->getContent() << "\n";
            cout << "------------------------\n";
        }
    }
}

void MessageManager::printOutbox(const User* currentUser) {
    int count = 1;
    cout << "\n--- OUTBOX ---\n";
    for (int i = 0; i < size; ++i) {
        if (messages[i]->getSender() == currentUser->getName()) {
            time_t temp = messages[i]->getTimestamp();
            cout << "\nMessage " << count++ << ":\n";
            cout << "------------------------\n";
            cout << "ID: " << messages[i]->getId() << "\n";
            cout << "From: " << messages[i]->getSender() << "\n";
            cout << "To: " << messages[i]->getReceiver() << "\n";
            cout << "Type: " << messages[i]->getType() << "\n";
            cout << "Timestamp: " << ctime(&temp);
            cout << "Content: " << messages[i]->getContent() << "\n";
            cout << "------------------------\n";
        }
    }
}

void MessageManager::deleteMessageById(const User* user, int messageId) {
    bool found = false;
    for (int i = 0; i < size; ++i) {
        if (messages[i]->getId() == messageId && messages[i]->getSender() == user->getName()) {
            found = true;

            delete messages[i];

            for (int j = i; j < size - 1; ++j) {
                messages[j] = messages[j + 1];
            }

            --size;
            cout << "Message with ID " << messageId << " deleted successfully.\n";
            break;
            Audit::audit(user->getName(), user->getRole(), "Deleted a message", "Success");
        }
    }
    if (!found) {
        cout << "Message not found!\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to delete a message", "Failed");
    }
    saveToFile();
}

