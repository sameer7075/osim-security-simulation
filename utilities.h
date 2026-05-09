#pragma once
#include "pch.h"

string getTime();
string generateHash(const string& input);
char encryptChar(char messageChar, char keywordChar);
char encryptChar(char messageChar, char keywordChar);
char decryptChar(char encryptedChar, char keywordChar);

string encryptMessage(const string& message, const string& keyword = "killit");
string decryptMessage(const string& encryptedMessage, const string& keyword = "killit");

void addNotification(const string& username, const string& role, const string& type, const string& message);
void removeNotification(const string& username, const string& role, const string& type, const string& message);

bool isValidUsername(const string& username);
