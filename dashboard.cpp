#include "dashboard.h"

void mainMenu(User* currentUser){
    int c1;
    cout << "\nWelcome " << currentUser->getName() << " (" << currentUser->getRole() << ")\n";
    do{
        cout << endl << endl << banner;
        NotificationManager::getInstance()->showallnotis();
        cout << "\n---------- Main Menu ----------\n";        
        cout << "1. Task Management\n";
        cout << "2. Message Management\n";
        cout << "3. Notification Management\n";
        cout << "4. Performance Review\n";
        cout << "5. Audit Management\n";
        cout << "6. User Account Management\n";
        cout << "7. Account Settings\n";
        cout << "8. Logout\n";
        cout << "Enter your choice: ";
        cin >> c1;
        switch(c1) {
            case 1:
                taskManagement(currentUser);
                break;
            case 2:
                messageManagement(currentUser);
                break;
            case 3:
                notificationManagement(currentUser);
                break;
            case 4:
                reviewManagement(currentUser);
                break;
            case 5:
                auditManagement(currentUser);
                break;
            case 6:
                userManagement(currentUser);
                break;
            case 7:
                accountSettings(currentUser);
                break;
            case 8:
                cout << "Logging Out...\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 8);
}

void taskManagement(const User* currentUser) {
    int c1;
    do{
        cout << "\n----- Task Management -----\n";
        cout << "1. Create Task\n";
        cout << "2. View All Tasks\n";
        cout << "3. View Assigned Tasks\n";
        cout << "4. View Created Tasks\n";
        cout << "5. Update Task Status\n";
        cout << "6. Delegate Task\n";
        cout << "7. Assign Task\n";
        cout << "8. Complete Task\n";
        cout << "9. Delete Task\n";
        cout << "10. Filter Tasks\n";
        cout << "11. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1:
                TaskManager::getInstance()->createTask(currentUser);
                break;
            case 2:
                TaskManager::getInstance()->printAllTasks(currentUser);
                break;
            case 3:
                TaskManager::getInstance()->printAssignedTasks(currentUser);
                break;
            case 4:
                TaskManager::getInstance()->printCreatedTasks(currentUser);
                break;
            case 5:
                TaskManager::getInstance()->updateTaskStatus(currentUser);
                break;
            case 6:
                TaskManager::getInstance()->delegateTask(currentUser);
                break;
            case 7:
                TaskManager::getInstance()->assignTask(currentUser);
                break;
            case 8:
                TaskManager::getInstance()->completeTask(currentUser);
                break;
            case 9:
                TaskManager::getInstance()->removeTask(currentUser);
                break;
            case 10:
                TaskManager::getInstance()->filterTasks(currentUser);
                break;
            case 11:
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 11);
}

void messageManagement(const User* currentUser){
    int c1;
    do{
        cout << "\n----- Message Management -----\n";
        cout << "1. Send Message\n";
        cout << "2. View Inbox\n";
        cout << "3. View Outbox\n";
        cout << "4. Delete Message\n";
        cout << "5. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1:{
                cout << "Enter message type (1: Private, 2: Alert, 3: Info): ";
                int typeChoice;
                cin >> typeChoice;
                while(typeChoice < 1 || typeChoice > 3) {
                    cout << "Invalid choice! Enter message type (1: Private, 2: Alert, 3: Info): ";
                    cin >> typeChoice;
                }
                string type = MESSAGE_TYPE_STRINGS[typeChoice - 1];
                if(typeChoice == 3){
                    int targetRole;
                    cout << "Enter target role (1: Junior, 2: Employee, 3: Manager, 4: Director, 5: Executive, -1: ALL): ";
                    cin >> targetRole;
                    while((targetRole < 1 || targetRole > 5) && targetRole != -1) {
                        cout << "Invalid choice! Enter target role (1: Junior, 2: Employee, 3: Manager, 4: Director, 5: Executive): ";
                        cin >> targetRole;
                    }
                    string targetRoleStr = (targetRole == -1 ) ? "ALL" : ROLE_STRINGS[targetRole - 1];
                    MessageManager::getInstance()->broadcastInfo(currentUser, targetRoleStr, type);
                    break;
                }
                string recipientName;
                cout << "Enter recipient's name: ";
                cin.ignore();
                getline(cin, recipientName);
                User* recipient = UserManager::getInstance()->getUser(recipientName);
                if (!recipient) {
                    cout << "\nRecipient not found!\n";
                    break;
                }
                if (recipient->getName() == currentUser->getName()) {
                    cout << "\nYou cannot send a message to yourself!\n";
                    break;
                }
                MessageManager::getInstance()->sendMessage(currentUser, recipient, type);
                break;
            }
            case 2:
                MessageManager::getInstance()->printInbox(currentUser);
                break;
            case 3:
                MessageManager::getInstance()->printOutbox(currentUser);
                break;
            case 4:{
                int messageId;
                cout << "Enter message ID to delete: ";
                cin >> messageId;
                MessageManager::getInstance()->deleteMessageById(currentUser, messageId);
                break;
            }
            case 5:
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 5);
}

void notificationManagement(const User* currentUser){
    int c1;
    do{
        cout << "\n----- Notification Management -----\n";
        cout << "1. Send Notification\n";
        cout << "2. View Notifications\n";
        cout << "3. Delete Notification\n";
        cout << "4. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1:
                NotificationManager::getInstance()->createNoti(currentUser);
                break;
            case 2:
                NotificationManager::getInstance()->showallnotis();
                break;
            case 3:
                NotificationManager::getInstance()->removenoti(currentUser);
                break;
            case 4:
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 4);
}

void reviewManagement(const User* currentUser){
    int c1;
    do{
        cout << "\n----- Performance Review -----\n";
        cout << "1. Generate your Performance Report\n";
        cout << "2. Generate Report for Another User\n";
        cout << "3. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1:{
                cout << *currentUser->getPerformanceReview();
                break;
            }                
                
            case 2:{
                string searchName;
                cout << "Enter username to generate report: ";
                cin.ignore();
                getline(cin, searchName);
                User* user = UserManager::getInstance()->getUser(searchName);
                if (!user) {
                    cout << "\nUser not found!\n";
                    break;
                }
                else{
                    if(PolicyEngine::canViewPerformanceReport(currentUser, user)){
                        cout << *user->getPerformanceReview();
                    }
                    else{
                        cout << "\nYou don't have permission to view this report!\n";
                        Audit::audit(currentUser->getName(), currentUser->getRole(), "Attempted to view a performance report", "Failed");
                    }
                    
                }
                break;
            }  
            case 3:
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
        }
    }while(c1 != 3);
}

void auditManagement(const User* currentUser){
    int c1;
    AnomalyDetector anomalyDetector;
    do{
        cout << "\n----- Audit Managament -----\n";
        cout << "1. View Audit Log\n";
        cout << "2. View Last Anomaly Report\n";
        cout << "3. Store new Anomaly Report for current Month\n";
        cout << "4. Go Back\n";
        cout << "Enter you choice: ";
        cin >> c1;

        switch (c1){
        case 1:
            Audit::viewLog(currentUser);
            break;
        
        case 2: 
            if(PolicyEngine::canViewAuditLog(currentUser)){
                anomalyDetector.viewAnomalyReport();
            }
            else{
                cout << "\nYou don't have permission to view the Anomaly Report!\n";
            }
            break;
        case 3:
            if(PolicyEngine::canViewAuditLog(currentUser)){
                anomalyDetector.loadAuditLog();
                anomalyDetector.detectAnomalies();
                anomalyDetector.writeAnomalies();
                cout << "\nAnomaly Report Generated and stored sucessfully!\n";
            }
            else{
                cout << "\nYou don't have permission to generate the Anomaly Report!\n";
            }
            break;
        case 4:
            break;        
        default:
            break;
        }

    }while(c1 != 4);
}

void userManagement(const User* currentUser){
    int c1;
    do{
        cout << "\n----- User Management -----\n";
        cout << "1. View All Users\n";
        cout << "2. Create New User\n";
        cout << "3. Delete User\n";
        cout << "4. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;

        switch(c1){
            case 1:
                UserManager::getInstance()->displayAllUsers(currentUser);
                break;            
            case 2: 
                UserManager::getInstance()->createUser(currentUser);
                break;            
            case 3:
                UserManager::getInstance()->removeEmp(currentUser);
                break;            
            case 4:
                break;            
            default: 
                cout << "\nInvalid choice. Please try again!\n"; 
                break;
        }
    }while(c1 != 4);
}

void accountSettings(User* currentUser){
    int c1;
    do{
        cout << "\n----- Account Settings -----\n";
        cout << "1. Change Password\n";
        cout << "2. View Account Details\n";
        cout << "3. Enable Multi-Factor Authentication\n";
        cout << "4. Disable Multi-Factor Authentication\n";
        cout << "5. Go Back\n";
        cout << "Enter your choice: ";
        cin >> c1;
        
        switch(c1) {
            case 1:
                UserManager::getInstance()->changePassword(currentUser);
                break;
            case 2: 
                cout << endl;
                currentUser->show();
                break;
            case 3:
                UserManager::getInstance()->enableOtp(currentUser);
                break;
            case 4:
                UserManager::getInstance()->disableOtp(currentUser);
                break;
            default:
                cout << "\nInvalid choice. Please try again!\n"; 
                break;
        }
    }while(c1 != 5);
}
