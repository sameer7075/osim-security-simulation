#include "pch.h"
#include "MFA.h"

void MFA::sendCode() {
    srand(time(0));
    code = to_string(rand() % 9000 + 1000);
    timeSent = time(0);
    cout << "OTP sent: " << code << endl;
}

bool MFA::check(string input) {
    return difftime(time(0), timeSent) < 60 && input == code;
}
