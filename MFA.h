#pragma once
#include "pch.h"

class MFA {
private:
    string code;
    time_t timeSent;

public:
    void sendCode();
    bool check(string input);
};
