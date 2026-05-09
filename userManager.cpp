#include "pch.h"
#include "userManager.h"
#include "policyEngine.h"
#include "audit.h"
#include "MFA.h"
#include "utilities.h"

UserManager* UserManager::instance = nullptr;

UserManager::UserManager() {
    capacity = 10;
    currentemp = 0;
    emp = new User*[10];
    loadFromFile();
}

void UserManager::addUser(User* e) {
    if (currentemp == capacity) {
        resize(capacity * 2);
    }
    emp[currentemp++] = e;
}

UserManager* UserManager::getInstance() {
    if (instance == nullptr) {  
        instance = new UserManager();
    }
    return instance;
}

void UserManager::resize(int newCapacity) {
    User** newusers = new User*[newCapacity];
    for (int i = 0; i < currentemp; ++i) {
        newusers[i] = emp[i];
    }
    delete[] emp;
    emp = newusers;
    capacity = newCapacity;
}

User* UserManager::getUser(string username) {
    for (int i = 0; i < currentemp; ++i) {
        if (emp[i]->getName() == username) {
            return emp[i];
        }
    }
    return nullptr;
}

int UserManager::getUserCount() {
    return currentemp;
}

User** UserManager::getAllUsers() {
    return emp;
}

void UserManager::removeEmp(const User* currentUser) {    
    if(!PolicyEngine::canDeleteUsers(currentUser)){
        cout << "\nYou don't have permission to remove users!\n";
        Audit::audit(currentUser->getName(), currentUser->getRole(), "Attempted to remove a user", "Failed");
        return;
    }
    string username;
    cout << "Enter the username of the user to remove: ";
    cin >> username;
    User* target = getUser(username);
    if(!target) {
        cout << "\nUser not found!\n" << endl;
        return;
    }
    if(PolicyEngine::canRemoveUser(currentUser, target)) {
        for (int i = 0; i < currentemp; ++i) {
            if (emp[i]->getName() == username ) {
                delete emp[i];
                for (int j = i; j < currentemp - 1; ++j) {
                    emp[j] = emp[j + 1];
                }
                currentemp--;
                break;
            }
        }
        cout << "\nUser '" << username << "' removed successfully.\n";
        saveToFile();
        removeOtpPreference(username);
        Audit::audit(currentUser->getName(), currentUser->getRole(), "Removed a user", "Success");
    }
    else{
        cout << "\nYou don't have permission to remove this user!\n";
        Audit::audit(currentUser->getName(), currentUser->getRole(), "Attempted to remove a user", "Failed");
    }    
}

void UserManager::loadFromFile() {
    ifstream fin("users.txt");
    if (!fin) {
        cerr << "\nCould not open notification.txt for reading.\n";
        return;
    }
    string fileUsername, fileRole, filePassword, filelevel;
    while (fin) {
        if (!(getline(fin, fileUsername, '|') &&
              getline(fin, filePassword, '|') &&
              getline(fin, fileRole, '|') &&
              getline(fin, filelevel))) {
            break;
        }
        if (fileRole == "Junior") {
            User* t = new Junior(fileUsername, filePassword);
            addUser(t);
        }
        else if (fileRole == "Employee") {
            User* t = new Employee(fileUsername, filePassword);
            addUser(t);
        }
        else if (fileRole == "Manager") {
            User* t = new Manager(fileUsername, filePassword);
            addUser(t);
        }
        else if (fileRole == "Executive") {
            User* t = new Executive(fileUsername, filePassword);
            addUser(t);
        }
        else if (fileRole == "Director") {
            User* t = new Director(fileUsername, filePassword);
            addUser(t);
        }
    }
    fin.close();
}

bool UserManager::authenticate(const string username, const string password, const string role) {
    bool found = false;
    ifstream fin("users.txt");
    if (!fin) {
        cerr << "\nCould not open user.txt for reading.\n";
        return false;
    }

    string n, p, r, c;
    while ((getline(fin, n, '|') && getline(fin, p, '|') && getline(fin, r, '|') && getline(fin, c))) {
        if (username == n && generateHash(password) == p && role == r) {
            if(checkOtpEnabled(getUser(username))){
                MFA* mfa = new MFA;
                string otp;
                mfa->sendCode();
                cout << "Enter OTP(OTP expires in 30 seconds): ";
                cin >> otp;
                if(!mfa->check(otp)){
                    cout << "\nInvalid OTP!\n";
                    Audit::audit(username, role, "Authenticated", "Failed");
                    fin.close();
                    delete mfa;
                    return false;
                }
                else{
                    cout << "\nOTP verified successfully!\n";
                    delete mfa;
                }
            }
            cout << "\nUser authenticated successfully!\n";
            Audit::audit(username, role, "Authenticated", "Success");
            fin.close();
            return true;
        }
    }
    cout << "\nCouldn't find user, Incorrect credentials!\n";
    fin.close();
    Audit::audit(username, role, "Authenticated", "Failed");
    return false;
}

void UserManager::saveToFile() {
    ofstream o1("users.txt", ios::trunc);
    if (o1) {
        for (int i = 0; i < currentemp; ++i) {
            o1 << emp[i]->getName() << "|"
               << emp[i]->getHashedPassword() << "|"
               << emp[i]->getRole() << "|"
               << emp[i]->getclearancelevel() << "\n";
        }
        o1.close();
    }
}

bool UserManager::userExists(const string& username) {
    ifstream file("users.txt");
    string name, dummy, fileRole;

    while (getline(file, name, '|') && getline(file, dummy, '|') && getline(file, fileRole, '|') && getline(file, dummy)) {
        if (name == username) {
            return true;
        }
    }

    return false;
}

UserManager::~UserManager() {
    for (int i = 0; i < currentemp; i++) {
        delete emp[i];
    }
    delete[] emp;
    emp = NULL;
}

void UserManager::displayAllUsers(const User* user) {
    if(PolicyEngine::canViewUsers(user)) {
        cout << "\n--- All Users ---\n";
        for (int i = 0; i < currentemp; ++i) {
            emp[i]->show();
        }
    }
    else{
        cout << "\nYou don't have permission to view all users!\n";
    }
    
}

void UserManager::createUser(const User* currentUser) {  

    if(PolicyEngine::canCreateUser(currentUser)) {
        string username, password, role;
        cin.ignore();
        cout << "Enter username: ";
        getline(cin, username);
        while(!isValidUsername(username)){
            cout << "\nInvalid Username! Username must be atleast 3 character and should not contain a space\n";
            cout << "Enter username: ";
            getline(cin, username);
        }
        while(userExists(username)){
            cout << "\nUsername already taked!\n";
            cout << "Enter username: ";
            getline(cin, username);
        }
        cout << "Enter password: ";
        getline(cin,password);
        password = generateHash(password);
        cout << "Enter role (Junior/Employee/Manager): ";
        cin >> role;

        if (role == "Junior") {   
            User* t = new Junior(username, password);
            addUser(t);
            saveToFile();
            cout << "\nUser created successfully!\n";
            addMFA(t);
            Audit::audit(currentUser->getName(), currentUser->getRole(), "Created a user", "Success");
        }
        else if (role == "Employee") {
            User* t = new Employee(username, password);
            addUser(t);
            saveToFile();
            cout << "\nUser created successfully!\n";
            addMFA(t);
            Audit::audit(currentUser->getName(), currentUser->getRole(), "Created a user", "Success");
        }
        else if (role == "Manager") {
            User* t = new Manager(username, password);
            addUser(t);
            saveToFile();
            cout << "\nUser created successfully!\n";
            addMFA(t);
            Audit::audit(currentUser->getName(), currentUser->getRole(), "Created a user", "Success");
        }        
        else {
            cout << "\nInvalid role!\n" << endl;
            cout << "Can only create Junior, Employee or Manager!\n";
            Audit::audit(currentUser->getName(), currentUser->getRole(), "Attempted to create a user", "Failed");
        }
    }
    else{
        cout << "\nYou don't have permission to create a user!\n";
        Audit::audit(currentUser->getName(), currentUser->getRole(), "Attempted to create a user", "Failed");
    }    
}

void UserManager::changePassword(User* currentUser){
    currentUser->changePassword();
    saveToFile();
    
}

bool UserManager::checkOtpEnabled(const User* currentUser){
    fstream o1;
    string fileUsername;
    int state;

    o1.open("preference.txt", ios::in);
    if(o1){
        while(o1 >> fileUsername >> state){
            if(currentUser->getName() == fileUsername && state == 1){
                o1.close();
                return true;
            }
        }
        o1.close();
    }
    return false;
}

void UserManager::enableOtp(const User* currentUser){
    if(checkOtpEnabled(currentUser)){
        cout << "\nMFA is already enabled!\n";
        return;
    }
    fstream o1, o2;
    o1.open("preference.txt", ios::in);
    o2.open("temp.txt", ios::out);
    string fileUsername;
    int fileState;
    if(o1){
        while(o1 >> fileUsername >> fileState){
            if(currentUser->getName() == fileUsername){
                o2 << fileUsername << " " << 1 << "\n";
            }
            else{
                o2 << fileUsername << " " << fileState << "\n";
            }
        }
        o1.close();
        o2.close();
    }
    remove("preference.txt");
    rename("temp.txt", "preference.txt");
    cout << "\nMFA enabled successfully!\n";
}

void UserManager::disableOtp(const User* currentUser){
    if(!checkOtpEnabled(currentUser)){
        cout << "\nMFA is already disabled!\n";
        return;
    }
    fstream o1, o2;
    o1.open("preference.txt", ios::in);
    o2.open("temp.txt", ios::out);
    string fileUsername;
    int fileState;
    if(o1){
        while(o1 >> fileUsername >> fileState){
            if(currentUser->getName() == fileUsername){
                o2 << fileUsername << " " << 0 << "\n";
            }
            else{
                o2 << fileUsername << " " << fileState << "\n";
            }
        }
        o1.close();
        o2.close();
    }
    remove("preference.txt");
    rename("temp.txt", "preference.txt");
    cout << "\nMFA disabled successfully!\n";
}

void UserManager::addMFA(const User* currentUser){
    fstream o1;
    o1.open("preference.txt", ios::out | ios::app);
    if(o1){
        o1 << currentUser->getName() << " " << 0 << "\n";
        o1.close();
    }
}

void UserManager::removeOtpPreference(const string& username) {
    ifstream inFile("preference.txt");
    ofstream outFile("temp.txt");
    string fileUsername;
    int state;

    if (inFile && outFile) {
        while (inFile >> fileUsername >> state) {
            if (fileUsername != username) {
                outFile << fileUsername << " " << state << "\n";
            }
        }
        inFile.close();
        outFile.close();
        remove("preference.txt");
        rename("temp.txt", "preference.txt");
    }
}