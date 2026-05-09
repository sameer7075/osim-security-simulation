#include "utilities.h"

string getTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt)-1] = '\0';
    return string(dt);
}

string generateHash(const string& input) {
    long hash = 0;  
    for (int i = 0; i < input.length(); i++) {
        char c = input[i];
        hash = (hash * 31) + c;  
    }
    return to_string(hash);  
}

char encryptChar(char messageChar, char keywordChar) {
    if (messageChar < 33 || messageChar > 126) {
        return messageChar;
    }

    int position = keywordChar - 33;
    char encrypted = (((messageChar - 33 + position) % 95) + 33);


    if (encrypted == '|') {
        return '~';  
    }

    return encrypted;
}

char decryptChar(char encryptedChar, char keywordChar) {
    
    if (encryptedChar == '~') {
        encryptedChar = '|';
    }

    if (encryptedChar < 33 || encryptedChar > 126) {
        return encryptedChar;
    }

    int position = keywordChar - 33;
    return (((encryptedChar - 33 - position + 95) % 95) + 33);
}

string encryptMessage(const string& message, const string& keyword) {
    string encryptedMessage;
    int mlength = message.length();
    int klength = keyword.length();

    for (int c = 0; c < mlength; ++c) {
        encryptedMessage += encryptChar(message[c], keyword[c % klength]);
    }
    return encryptedMessage;
}

string decryptMessage(const string& encryptedMessage, const string& keyword) {
    string decryptedMessage;
    int emlength = encryptedMessage.length();
    int klength = keyword.length();

    for (int c = 0; c < emlength; ++c) {
        decryptedMessage += decryptChar(encryptedMessage[c], keyword[c % klength]);
    }
    return decryptedMessage;
}



void addNotification(const string& username, const string& role,const string& type, const string& message) {
    ofstream fout("notification.txt", ios::app);
    if (!fout.is_open()) {
        cout << "Could not open notification.txt\n";
        return;
    }

    fout << username << "|" << role <<"|"<< type << "|" << message << "\n";
    fout.close();
}

void removeNotification(const string& username, const string& role,const string& type, const string& message) {
    ifstream fin("notification.txt");
    ofstream fout("temp.txt");

    string line;
    bool found = false;
    while (getline(fin, line)) {
        if (line != username + "|" + role +"|"+type +"|" + message) {
            fout << line << "\n";
        } else {
            found = true;
        }
    }

    fin.close();
    fout.close();

    remove("notification.txt");
    rename("temp.txt", "notification.txt");

    if (found)
        cout << "Notification removed successfully.\n";
    else
        cout << "Notification not found.\n";
}

bool isValidUsername(const string& username) {
    if (username.length() < 3) return false;

    for (char ch : username) {
        if (ch == ' ') return false;  
    }

    return true;
}



