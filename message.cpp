#include "message.h"

Message::Message(int id, const string& sender, const string& receiver, const string& content, const string& type, const time_t& timestamp)
                : id(id), sender(sender), receiver(receiver), content(content), type(type), timestamp(timestamp) {
   
}

int Message::getId() const { return id; }
string Message::getSender() const { return sender; }
string Message::getReceiver() const { return receiver; }
string Message::getContent() const { return content; }
string Message::getType() const { return type; }
time_t Message::getTimestamp() const { return timestamp; }

string Message::encryptContent() const {
    if (type != "PRIVATE") return content;
    string encrypted = content;
    for (int i = 0; encrypted[i] != '\0'; ++i) {        
        if (encrypted[i] != '|') {
            encrypted[i] = encrypted[i] + 3; 
            
            if (encrypted[i] == '|') {
                encrypted[i] = encrypted[i] + 1; 
            }
        }
    }
    return encrypted;
}

string Message::decryptContent() const {
    if (type != "PRIVATE") return content;
    string decrypted = content;
    for (int i = 0; decrypted[i] != '\0'; ++i) {        
        if (decrypted[i] != '|') {
            decrypted[i] = decrypted[i] - 3; 
            
            if (decrypted[i] == '|') {
                decrypted[i] = decrypted[i] - 1; 
            }
        }
    }
    return decrypted;
}



