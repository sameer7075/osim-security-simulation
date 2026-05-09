#include "pch.h"
#include "taskManager.h"
#include "policyEngine.h"
#include "task.h"
#include "user.h"
#include "timeManager.h"
#include "userManager.h"
#include "audit.h"

TaskManager* TaskManager::instance = nullptr;

TaskManager::TaskManager() {
    capacity = 10; 
    size = 0;         
    tasks = new Task*[capacity];  
    loadFromFile();
    currentId = 0;
    for (int i = 0; i < size; ++i) {
        if (tasks[i]->getId() > currentId) {
            currentId = tasks[i]->getId();  
        }
    }
    expireTasks();saveToFile();
}

TaskManager* TaskManager::getInstance() {
    if (instance == nullptr) {  
        instance = new TaskManager();
    }
    return instance; 
}

TaskManager::~TaskManager() {
    saveToFile();
    for (int i = 0; i < size; ++i) {
        delete tasks[i]; 
    }
    delete[] tasks; 
}

void TaskManager::loadFromFile() {
    ifstream o1("tasks.txt");
    if (!o1) {
        cerr << "Could not open tasks.txt for reading.\n";
        return;
    }

    while (o1) {
        string idStr, titleStr, statusStr, createdStr, updatedStr, ttlStr, creatorStr, assigneeStr, delegatedByStr, priorityStr;

        if (!(getline(o1, idStr, '|') &&
              getline(o1, titleStr, '|') &&
              getline(o1, statusStr, '|') &&
              getline(o1, createdStr, '|') &&
              getline(o1, updatedStr, '|') &&
              getline(o1, ttlStr, '|') &&
              getline(o1, creatorStr, '|') &&
              getline(o1, assigneeStr, '|') &&
              getline(o1, delegatedByStr, '|') &&
              getline(o1, priorityStr, '\n'))) {
            break; 
        }

        int id = stoi(idStr);
        int status = stoi(statusStr);
        time_t created = stol(createdStr);
        time_t updated = stol(updatedStr);
        int ttl = stoi(ttlStr);
        int priority = stoi(priorityStr);

        Task* t = new Task(id, titleStr, ttl, creatorStr, assigneeStr, priority);
        t->setStatus(status);
        t->setCreatedAt(created);
        t->setDelegatedBy(delegatedByStr);
        t->setUpdatedAt(updated);
        addTask(t);
    }

    o1.close();
}

void TaskManager::resize(int newCapacity) {
    Task** newTasks = new Task*[newCapacity];  
    for (int i = 0; i < size; ++i) {
        newTasks[i] = tasks[i];  
    }
    delete[] tasks;  
    tasks = newTasks;  
    capacity = newCapacity;  
}

void TaskManager::addTask(Task* task) {
    if (size == capacity) {
        resize(capacity * 2);  
    }
    tasks[size++] = task;  
    saveToFile();
}

Task* TaskManager::getTask(int id) {
    expireTasks();saveToFile();
    for (int i = 0; i < size; ++i) {
        if (tasks[i]->getId() == id) {
            return tasks[i];  
        }
    }
    return nullptr;  
}

void TaskManager::removeTask(const User* user) {
    int taskId;
    cout << "Enter task ID to delete: ";
    cin >> taskId;
    Task* task = getTask(taskId);
    if(PolicyEngine::isActionAllowed(user, ACTION_DELETE_TASK, task)){        
        for (int i = 0; i < size; ++i) {
            if (tasks[i]->getId() == taskId) {
                delete tasks[i];  
                for (int j = i; j < size - 1; ++j) {
                    tasks[j] = tasks[j + 1];  
                }
                size--;  
                break;
            }
        }
        saveToFile();
        Audit::audit(user->getName(), user->getRole(), "Deleted a task", "Success");
        cout << "\nTask with ID " << taskId << " deleted successfully!\n";
    }
    else {
        cout << "\nYou are not authorized to delete this task!\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to delete a task", "Failed");
    }
}

int TaskManager::generateUniqueId() {
    return ++currentId;  
}

void TaskManager::createTask(const User* user) {
    if(PolicyEngine::isActionAllowed(user, ACTION_CREATE_TASK)) {
        string title, assigneeName;
        int ttl, priority;
        cout << "Enter task title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter assignee's name (leave blank if not applicable): ";
        getline(cin, assigneeName);
        cout << "Enter TTL (in seconds): ";
        cin >> ttl;
        while(ttl < 0){
            cout << "Invalid TTL!\n";
            cout << "Enter TTL (in seconds): ";
            cin >> ttl;
        }
        cout << "Enter priority (0: Low, 1: Medium, 2: High): ";
        cin >> priority;
        int id = generateUniqueId();
        User* assignee = UserManager::getInstance()->getUser(assigneeName);
        if (!assignee && assigneeName != "") {
            cout << "\nAssignee not found. Task will be created without an assignee.\n";
            assigneeName = "";
        }
        if((assignee && PolicyEngine::isActionAllowed(user, ACTION_ASSIGN_TASK, assignee)) || assigneeName == ""){
            Task* newTask = new Task(id, title, ttl, user->getName(), assigneeName, priority);
            if(assigneeName == "") {
                newTask->setStatus(0);  
            } 
            else {
                newTask->setStatus(1);  
            }
            addTask(newTask);
            saveToFile();
            Audit::audit(user->getName(), user->getRole(), "Created a task", "Success");
            cout << "\nTask created successfully!\n";
        }
        else {
            cout << "\nYou are not authorized to Assign this task.\n";
            Audit::audit(user->getName(), user->getRole(), "Attempted to assign a task", "Failed");
        }        
    }
    else {
        cout << "\nYou are not authorized to create a task.\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to create a task", "Failed");
    }
    saveToFile();
}

void TaskManager::filterTasks(const User* user) {
    int choice;
    string input;

    do{
        expireTasks();saveToFile();
        cout << "\n--- Filter Tasks ---\n";
        cout << "1. Filter by ID\n";
        cout << "2. Filter by Status\n";
        cout << "3. Filter by Creator\n";
        cout << "4. Filter by Assignee\n";
        cout << "5. Filter by TTL\n";
        cout << "6. Filter by Priority\n"; 
        cout << "7. Show Expired Tasks\n";
        cout << "8. Sort By Priority\n";
        cout << "9. Go Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: {
                expireTasks();saveToFile();
                int id;
                cout << "Enter task ID: ";
                cin >> id;
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getId() == id) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 2: {
                expireTasks();saveToFile();
                int status;
                cout << "Enter status (0: Created, 1: Assigned, 2: In Progress, 3: Completed, 4: Expired): ";
                cin >> status;
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getStatus() == status) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 3: {
                expireTasks();saveToFile();
                cout << "Enter creator's name: ";
                getline(cin, input);
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getCreator() == input) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 4: {
                expireTasks();saveToFile();
                cout << "Enter assignee's name: ";
                getline(cin, input);
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getAssignee() == input) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 5: {
                expireTasks();saveToFile();
                int ttl;
                cout << "Enter TTL (in seconds): ";
                cin >> ttl;
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getTTL() == ttl) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }

            case 6:{ 
                expireTasks();saveToFile();
                int priority;
                cout << "Enter priority (0: Low, 1: Medium, 2: High): ";
                cin >> priority;
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->getPriority() == priority) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 7: {
                expireTasks();saveToFile();
                for (int i = 0; i < size; ++i) {
                    if (tasks[i]->isExpired()) {
                        cout << "\nTask " << i + 1 << ":\n";
                        cout << "------------------------------------\n";
                        tasks[i]->printDetails(); 
                        cout << "------------------------------------\n";
                    }
                }
                break;
            }
            case 8:{
                expireTasks();saveToFile();
                Task** sortedTasks = getTasksSortedByPriority();
                for (int i = 0; i < size; ++i) {                
                    cout << "\nTask " << i + 1 << ":\n";
                    cout << "------------------------------------\n";
                    sortedTasks[i]->printDetails(); 
                    cout << "------------------------------------\n";                    
                }
            }        
            case 9:
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }while(choice != 9);
}

void TaskManager::printAllTasks(const User* user) {
    expireTasks();saveToFile();
    if (size == 0) {
        cout << "\nNo tasks available.\n";
        return;
    }
    if(PolicyEngine::isActionAllowed(user, ACTION_VIEW_ALL_TASKS)) {
        for (int i = 0; i < size; ++i) {
            cout << "\nTask " << i + 1 << ":\n";
            cout << "------------------------------------\n";
            tasks[i]->printDetails();  
            cout << "------------------------------------\n";
        }
    }
    else {
        cout << "\nYou are not authorized to view all tasks.\n";
    }
}

void TaskManager::printAssignedTasks(const User* user) {
    expireTasks();saveToFile();
    bool flag;
    Task** soretdTasks = getTasksSortedByPriority();
    for (int i = 0; i < size; ++i) {
        if(soretdTasks[i]->getAssignee() == user->getName()) {
            flag = true;
            cout << "\nTask " << i + 1 << ":\n";
            cout << "------------------------------------\n";
            soretdTasks[i]->printDetails();  
            cout << "------------------------------------\n";
        } 
    }
    if(!flag){
        cout << "\nNo Assigned Tasks Found!\n";
    }
}

void TaskManager::printCreatedTasks(const User* user) {
    expireTasks();saveToFile();
    bool flag = false;
    Task** soretdTasks = getTasksSortedByPriority();
    for (int i = 0; i < size; ++i) {
        if(soretdTasks[i]->getCreator() == user->getName()) {
            flag = true;
            cout << "\nTask " << i + 1 << ":\n";
            cout << "------------------------------------\n";
            soretdTasks[i]->printDetails();  
            cout << "------------------------------------\n";
        } 
    }
    if(!flag){
        cout << "\nNo Created Tasks Found!\n";
    }
}

void TaskManager::saveToFile() {
    ofstream o1("tasks.txt", ios::trunc);
    if(o1){
        for (int i = 0; i < size; ++i) {
            tasks[i]->saveToFile(o1);
        }
        o1.close();
    }    
}

void TaskManager::updateTaskStatus(const User* user) {
    expireTasks();saveToFile();
    int taskId, newStatus;
    cout << "Enter task ID: ";
    cin >> taskId;
    cout << "Enter new status (0: Created, 1: Assigned, 2: In Progress, 3: Completed, 4: Expired): ";
    cin >> newStatus;
    while(newStatus < 0 || newStatus > 4) {
        cout << "Invalid task ID!";
        cout << "Enter new status (0: Created, 1: Assigned, 2: In Progress, 3: Completed, 4: Expired): ";
        cin >> taskId;
    }
    Task* task = getTask(taskId);
    if (!task) {
        cout << "\nTask not found!\n";
        return;
    }

    if(PolicyEngine::canChangeStatus(user, task, newStatus)) {
        if (task->isExpired()) {
            cout << "Warning: Task is expired!\n";
            cout << "Creation Date would be reset.\n";
            cout << "Do you want to update the status? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (choice != 'y' && choice != 'Y') {
                cout << "\nStatus update cancelled.\n";
                return;
            }
            task->setCreatedAt(TimeManager::getCurrentTime());
            task->setStatus(newStatus);
            cout << "\nExpired task reopened!\n";
        } 
        else {       
            task->setStatus(newStatus);
        }        
    }
    else {
        cout << "\nYou are not authorized to do this action!\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to change task status", "Failed");
        return;
    }
    saveToFile();
    Audit::audit(user->getName(), user->getRole(), "Changed task status", "Success");
    cout << "\nTask status updated successfully!\n";
}

void TaskManager::completeTask(const User* user) {
    expireTasks();saveToFile();
    int taskId;
    cout << "Enter task ID to complete: ";
    cin >> taskId;
    Task* task = getTask(taskId);
    if (!task) return;
    if (PolicyEngine::isTaskCompletionAllowed(user, task)) {
        task->setStatus(3);
        user->getPerformanceReview()->logTaskCompletion(task);
        cout << "\nTask completed successfully!\n";
        Audit::audit(user->getName(), user->getRole(), "Completed a task", "Success");
        saveToFile();
    }    
    else {
        cout << "\nYou are not authorized to complete this task.\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to complete a task", "Failed");
        return;
    }     
}

bool TaskManager::delegateTask(const User* user) {
    expireTasks();saveToFile();
    int taskId;
    cout << "Enter task ID to delegate: ";  
    cin >> taskId;
    cout << "Enter assignee's name: ";
    cin.ignore();
    string assigneeName;
    getline(cin, assigneeName);
    User* assignee = UserManager::getInstance()->getUser(assigneeName);
    Task* task = getTask(taskId);
    if (task) {  
        if(task->isExpired()) {
            saveToFile();
            cout << "\nTask is expired. Cannot delegate.\n";
            Audit::audit(user->getName(), user->getRole(), "Attempted to delegate an expired task", "Failed");
            return false;
        }

        if (task->getStatus() == 3) { 
            cout << "\nCannot delegate a completed task.\n";
            Audit::audit(user->getName(), user->getRole(), "Attempted to delegate a completed task", "Failed");
            return false;
        }

        if((PolicyEngine::isActionAllowed(user, ACTION_DELEGATE_TASK, task))){
            if (PolicyEngine::isActionAllowed(user, ACTION_DELEGATE_TASK, assignee)) {
                task->setAssignee(assignee->getName());
                task->setStatus(1); 
                task->setDelegatedBy(user->getName());
                saveToFile();
                cout << "\nTask successfully delegated to " << assignee->getName() << ".\n";
                Audit::audit(user->getName(), user->getRole(), "Delegated a task", "Success");
                return true;
            }
            else{
                cout << "\nDelegation not allowed: Delegatee must have higher clearance.\n";
                Audit::audit(user->getName(), user->getRole(), "Attempted to delegate a task", "Failed");
            }
        }
        else{
            cout << "\nYou are not authorized to Delegate this task.\n";
            Audit::audit(user->getName(), user->getRole(), "Attempted to delegate a task", "Failed");
        }        
        saveToFile();
    }
    else{
        cout << "\nTask not found!\n";
    }
    return false;  
}

bool TaskManager::assignTask(const User* user) {
    expireTasks();saveToFile(); 
    int taskId;
    cout << "Enter task ID to delegate: ";  
    cin >> taskId;
    cout << "Enter assignee's name: ";
    cin.ignore();
    string assigneeName;
    getline(cin, assigneeName);
    User* assignee = UserManager::getInstance()->getUser(assigneeName);
    Task* task = getTask(taskId);

    if (!task) {
        cout << "\nTask not found!\n";
        return false;
    }

    if (task->getStatus() == 4) { 
        cout << "\nCannot assign an expired task.\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to assign an expired task", "Failed");
        return false;
    }

    if (task->getStatus() == 3) { 
        cout << "\nCannot assign a completed task.\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to assign a completed task", "Failed");
        return false;
    }

    if(PolicyEngine::isActionAllowed(user, ACTION_ASSIGN_TASK, task)){        
        if(PolicyEngine::isActionAllowed(user, ACTION_ASSIGN_TASK, assignee)) {
            task->setAssignee(assignee->getName());
            task->setStatus(1); 
            task->setUpdatedAt(TimeManager::getCurrentTime());
            saveToFile();
            cout << "\nTask successfully assigned to " << assignee->getName() << ".\n";
            Audit::audit(user->getName(), user->getRole(), "Assigned a task", "Success");
            return true;
        }
        else{
            cout << "\nAssignment not allowed: Assignor must have higher clearance.\n";
            Audit::audit(user->getName(), user->getRole(), "Attempted to assign a task", "Failed");
        }
    }
    else{
        cout << "\nYou are not authorized to Assign this task.\n";
        Audit::audit(user->getName(), user->getRole(), "Attempted to assign a task", "Failed");
    }
    return false;
}

void TaskManager::recursiveExpireTasks(int index) {
    if (index >= size) {
        return;
    }

    if (tasks[index]->isExpired() && tasks[index]->getStatus() != 4) {
        // idr overdue ki logic dalni
        tasks[index]->setStatus(4); 
        string assigneeName = tasks[index]->getAssignee();
        UserManager* userManager = UserManager::getInstance();
        if (userManager) {
            User* user = userManager->getUser(assigneeName); 
            if (user && user->getPerformanceReview()) {
                user->getPerformanceReview()->logOverdueTask();
            }
        }
    }

    recursiveExpireTasks(index + 1);
}

void TaskManager::expireTasks(int index) {
    recursiveExpireTasks(index);
    saveToFile();
}

Task** TaskManager::getTasksSortedByPriority() {
    if (size == 0) return nullptr;

    Task** sortedTasks = new Task*[size];
    for (int i = 0; i < size; ++i) {
        sortedTasks[i] = tasks[i];
    }

    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (sortedTasks[j]->getPriority() < sortedTasks[j + 1]->getPriority()) {
                Task* temp = sortedTasks[j];
                sortedTasks[j] = sortedTasks[j + 1];
                sortedTasks[j + 1] = temp;
            }
        }
    }

    return sortedTasks;
}



