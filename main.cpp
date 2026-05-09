#include "pch.h"
#include "dashboard.h"

int main() {
    User* currentUser = nullptr;
    UserManager* userManager = UserManager::getInstance();
    MessageManager* messageManager = MessageManager::getInstance();
    NotificationManager* notificationManager = NotificationManager::getInstance();
    TaskManager* taskManager = TaskManager::getInstance();    

    int c1;
    do{
        cout << endl << endl << banner << endl;
        cout << "\n1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1: {
                cout << "\nSelect your role:\n";
                cout << "1. Junior\n";
                cout << "2. Employee\n";
                cout << "3. Manager\n";
                cout << "4. Director\n";
                cout << "5. Executive\n";
                int roleChoice;
                do{
                    cout << "Enter your choice: ";
                    cin >> roleChoice;
                }while(roleChoice < 1 || roleChoice > 5);
                string name, password;
                cout << "Enter your username: ";
                cin >> name;
                cout << "Enter your password: ";
                cin >> password;
                if(userManager->authenticate(name, password, ROLE_STRINGS[roleChoice - 1])) {
                    currentUser = userManager->getUser(name);
                    mainMenu(currentUser);
                } 
                
                break;
            }
            case 2:
                cout << "Exiting...\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 2);
    
    
    return 0;
}

