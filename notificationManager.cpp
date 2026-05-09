#include "notificationManager.h"
#include "policyEngine.h"
#include "audit.h"

NotificationManager* NotificationManager::instance = nullptr;

NotificationManager::NotificationManager(){
    capacity = 10;
    currentnotis = 0;
    n = new notification*[capacity];
    loadFromFile();
    if(currentnotis > 0) {
        nextid = n[currentnotis - 1]->getid() + 1;
    }
    else {
        nextid = 0;
    }
}

NotificationManager* NotificationManager::getInstance() {
    if(instance == nullptr){
        instance = new NotificationManager;
    }
    return instance;
}

void NotificationManager::addNoti(notification* noti) {
    if (currentnotis == capacity) {
        resize(capacity * 2);
    }
    n[currentnotis++] = noti;
}

void NotificationManager::resize(int newCapacity) {
    notification** newusers = new notification*[newCapacity];
    for (int i = 0; i < currentnotis; ++i) {
        newusers[i] = n[i];
    }
    delete[] n;
    n = newusers;
    capacity = newCapacity;
}

notification* NotificationManager::getnotification(int searchID) {
    for (int i = 0; i < currentnotis; ++i) {
        if (n[i]->getid() == searchID) {
            return n[i];
        }
    }
    return nullptr;
}

void NotificationManager::createNoti(const User* user) {
    int typeInt;
    cout << "Enter notification type (1: Info, 2: Warning, 3: Emergency): ";
    cin >> typeInt;
    while(typeInt < 1 || typeInt > 3) {
        cout << "Invalid choice! Enter notification type (1: Info, 2: Warning, 3: Emergency): ";
        cin >> typeInt;
    }
    string type = NOTIFICATION_TYPE_STRINGS[typeInt - 1];
    if(PolicyEngine::canSendNotification(user, type)) {
        string message;
        cout << "Enter notification message: "; 
        cin.ignore();
        getline(cin, message);
        time_t now = time(0);
        notification* t = new notification(nextid++, user->getName(), type, message, now);
        addNoti(t);
        saveToFile();
        cout << "\nNotification sent successfully!\n";
        Audit::audit(user->getName(), user->getRole(), "Sent a notification", "Success");
    }
    else{
        cout << "\nYou don't have permission to send this notification.\n" << endl;
        Audit::audit(user->getName(), user->getRole(), "Attempted to send a notification", "Failed");
    }
}

void NotificationManager::removenoti(const User* user) {
    bool found = false;
    int delID;
    cout << "Enter notification ID to delete: ";
    cin >> delID;
    for (int i = 0; i < currentnotis; ++i) {
        if (n[i]->getid() == delID) {
            found = true;
            if(n[i]->getusername() != user->getName() && user->getRole() != "Executive" && user->getRole() != "Director") {
                cout << "\nYou don't have permission to delete this notification!\n" << endl;
                Audit::audit(user->getName(), user->getRole(), "Attempted to delete a notification", "Failed");
                return;
            }
            delete n[i];
            for (int j = i; j < currentnotis - 1; ++j) {
                n[j] = n[j + 1];
            }
            currentnotis--;
        }
    }
    if(!found){
        cout << "\nNotification not found!\n";
        return;
    }
    saveToFile();
    cout << "\nNotification deleted successfully!\n" ;
    Audit::audit(user->getName(), user->getRole(), "Deleted a notification", "Success");
}

void NotificationManager::loadFromFile() {
    ifstream fin("notification.txt");
    if (!fin) {
        cerr << "Could not open notification.txt for reading.\n";
        return;
    }
    string fileID, fileUsername, type, message, fileTime;
    while (fin) {
        if (!(getline(fin, fileID, '|') &&
              getline(fin, fileUsername, '|') &&
              getline(fin, type, '|') &&
              getline(fin, message, '|') &&
              getline(fin, fileTime) )) {
            break;
        }
        int id = stoi(fileID);
        time_t time = stol(fileTime);
        notification* t = new notification(id, fileUsername, type, message, time);
        addNoti(t);
    }
    fin.close();
}

void NotificationManager::saveToFile() {
    ofstream o1("notification.txt", ios::trunc);
    if (o1) {
        for (int i = 0; i < currentnotis; ++i) {
            o1 << n[i]->getid() << "|"
               << n[i]->getusername() << "|"
               << n[i]->gettype() << "|"
               << n[i]->getmessage() << "|"
               << n[i]->gettime() << "\n";
        }
        o1.close();
    }
}

bool NotificationManager::notificationExists(const int& sID) {
    ifstream fin("notifications.txt");
    string fileID, fileUsername, type, message, fileTime;
    while (fin) {
        if (!(getline(fin, fileID, '|') &&
              getline(fin, fileUsername, '|') &&
              getline(fin, type, '|') &&
              getline(fin, message) &&
              getline(fin, fileTime, '|') )) {
            break;
        }
        int fID = stoi(fileID);

        if (fID == sID) {
            return true;
        }
    }

    return false;
}

void NotificationManager::showallnotis() {
    cout << "\n--- All Notifications ---\n";
    for (int i = 0; i < currentnotis; ++i) {
        time_t temp = n[i]->gettime();
        cout << "[" << n[i]->gettype() << "] " << "ID: " << n[i]->getid() << "\n"
             << n[i]->getmessage() << " (by " << n[i]->getusername() << ")\n"
             << "Time: " << ctime(&temp)
             << "--------------------------\n";
    }
}

NotificationManager::~NotificationManager() {
    for (int i = 0; i < currentnotis; i++) {
        delete n[i];
    }
    delete[] n;
    n = NULL;
}
