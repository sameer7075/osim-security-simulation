#include "pch.h"
#include "user.h"
#include "audit.h"

User::User(string u, string p, int l) : username(u), password(p), clearancelevel(l) {
    perfomancereview = nullptr;
}

void User::show() const {
    cout << username << " (clearancelevel: " << clearancelevel << ")\n";
}
void User::changePassword(){
    string oldPassword, newPassword, confirmPassword;
    cout << "Enter old password to verify your identity: ";
    cin >> oldPassword;
    if(generateHash(oldPassword) == password){
        cout << "Enter new Password: ";
        cin >> newPassword;
        cout << "Confirm new Password: ";
        cin >> confirmPassword;
        if(newPassword == confirmPassword){
            password = generateHash(newPassword);
        }
        cout << "\nPassword changed successfully!\n";
        Audit::audit(getName(), getRole(), "Changed password", "Success");
    }
    else{
        cout << "\nOld password is incorrect!\n";
        Audit::audit(getName(), getRole(), "Changed password", "Failed");
    }
}
User::~User() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}

int User::getclearancelevel() const { return clearancelevel; }
string User::getName() const { return username; }
string User::getHashedPassword() const { return password; }
PerformanceReview* User::getPerformanceReview() const { return perfomancereview; }

Junior::Junior(string u, string p) : User(u, p, 1) {
    perfomancereview = new PerformanceReview(username, ROLE_STRINGS[clearancelevel-1]);
}
string Junior::getRole() const { return "Junior"; }
Junior::~Junior() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}

Employee::Employee(string u, string p) : User(u, p, 2) {
    perfomancereview = new PerformanceReview(username, ROLE_STRINGS[clearancelevel-1]);
}
string Employee::getRole() const { return "Employee"; }
Employee::~Employee() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}

Manager::Manager(string u, string p) : User(u, p, 3) {
    perfomancereview = new PerformanceReview(username, ROLE_STRINGS[clearancelevel-1]);
}
string Manager::getRole() const { return "Manager"; }
Manager::~Manager() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}

Director::Director(string u, string p) : User(u, p, 4) {
    perfomancereview = new PerformanceReview(username, ROLE_STRINGS[clearancelevel-1]);
}
string Director::getRole() const { return "Director"; }
Director::~Director() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}

Executive::Executive(string u, string p) : User(u, p, 5) {
    perfomancereview = new PerformanceReview(username, ROLE_STRINGS[clearancelevel-1]);
}
string Executive::getRole() const { return "Executive"; }
Executive::~Executive() {
    if (perfomancereview) {
        delete perfomancereview; perfomancereview = nullptr;
    }
}
