#include "pch.h"
#include "audit.h"
#include "policyEngine.h"   

Audit::Audit() {
    username = "";
    role = "";
    action = "";
    status = "";
}

Audit::Audit(const string& user, const string& role, const string& action, const string& status)
    : username(user), role(role), action(action), status(status), timestamp(time(nullptr)) {}

string Audit::getUsername() const {return username;}

string Audit::getRole() const { return role;}

string Audit::getAction() const { return action;}

string Audit::getStatus() const {return status;}

time_t Audit::getTimestamp() const {return timestamp;}

void Audit::audit(string user , string role, string action,  string status) {
    ofstream fout("audit.txt", ios::app);
    fout << time(nullptr) << "|" <<user << "|"<<role<<"|"<< action<< "|"<< status << endl;
    fout.close();
}

void Audit::writeLog() {
    ofstream fout("audit.txt", ios::app);
    fout << *this;
    fout.close();
}

ostream& operator<<(ostream& out, const Audit& a) {
    out << a.timestamp << "|" << a.username << "|" << a.role << "|" << a.action << "|" << a.status << endl;
    return out;
}

void Audit::viewLog(const User* currentUser) {
    if(PolicyEngine::canViewAuditLog(currentUser)) {
        ifstream fin("audit.txt");
        if (!fin) {
            cerr << "Could not open audit.txt for reading.\n";
            return;
        }
        cout << "\n--- Audit Log ---\n";
        cout << left
            << setw(20) << "Username"
            << setw(15) << "Role"
            << setw(50) << "Action"
            << setw(10) << "Status"
            << "Timestamp"
            << endl << endl;

        string timeStr, user, role, action, status;
        while (getline(fin, timeStr, '|') &&
            getline(fin, user, '|') &&
            getline(fin, role, '|') &&
            getline(fin, action, '|') &&
            getline(fin, status)) {

            time_t temp = stol(timeStr);

            cout << left
                << setw(20) << user
                << setw(15) << role
                << setw(50) << action
                << setw(10) << status
                << ctime(&temp); 
        }

        fin.close();
    }
    else{
        cout << "\nYou don't have permission to view the audit log!\n";
    }    
}

void Audit::setTimestamp(time_t ts){ timestamp = ts; }

