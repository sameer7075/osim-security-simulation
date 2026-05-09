#pragma once

#include "task.h"
#include "user.h"
#include "userManager.h"

class TaskManager {
private:
    Task** tasks;    
    int capacity;    
    int size;
    int currentId;
   
    TaskManager();
    static TaskManager* instance;
public:
    static TaskManager* getInstance();
    

    ~TaskManager();

    void loadFromFile();
    void resize(int newCapacity);
    void addTask(Task* task);  
    Task* getTask(int id);   
    void removeTask(const User* user);
    void printAllTasks(const User* user);
    void printAssignedTasks(const User* user);
    void printCreatedTasks(const User* user);
    void saveToFile();
    int generateUniqueId() ;
    void createTask(const User* user);
    void filterTasks(const User* user);
    void updateTaskStatus(const User* user);
    bool delegateTask(const User* user);
    bool assignTask(const User* user);
    void recursiveExpireTasks(int index = 0);
    void expireTasks(int index = 0);
    void completeTask(const User* user);
    Task** getTasksSortedByPriority();

};
