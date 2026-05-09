#include "pch.h"
#include "policyEngine.h"
#include "task.h"
#include "user.h"

int PolicyEngine::roleToClearance(const string role) {
    if (role == "Junior") return 1;
    if (role == "Employee") return 2;
    if (role == "Manager") return 3;
    if (role == "Director") return 4;
    if (role == "Executive") return 5;
    return 0; 
}

bool PolicyEngine::isActionAllowed(const User* user, const char* action) {
    int userClearance = roleToClearance(user->getRole());

    if(string(action) == ACTION_CREATE_TASK) {
        return userClearance > 1; 
    }

    if(string(action) == ACTION_VIEW_ALL_TASKS) {
        return userClearance >= 4; 
    }

    return false; 
}

bool PolicyEngine::isActionAllowed(const User* user, const char* action, const User* targetUser) {
    int userClearance = roleToClearance(user->getRole());
    int targetClearance = roleToClearance(targetUser->getRole());

    if (string(action) == ACTION_DELEGATE_TASK) {
        return targetClearance >= userClearance; 
    }

    if (string(action) == ACTION_ASSIGN_TASK) {
        return targetClearance <= userClearance; 
    }

    return isActionAllowed(user, action); 
}

bool PolicyEngine::isActionAllowed(const User* user, const char* action, const Task* task) {
    int userClearance = roleToClearance(user->getRole());

    if (string(action) == ACTION_DELEGATE_TASK) {
        if (user->getName() == task->getAssignee()) {
            return true; 
        }
        return true; 
    }

    if (string(action) == ACTION_ASSIGN_TASK) {        
        if (userClearance <= 2) {
            return false; 
        }
        
        if (userClearance == 3) {
            if (user->getName() == task->getCreator()) {
                return true; 
            }
            return false; 
        }
        
        if (userClearance >= 4) {
            return true; 
        }
    }

    if(string(action) == ACTION_DELETE_TASK){
        int currentStatus = task->getStatus();
        
        if (user->getName() == task->getCreator())
            return true;
        
        if (userClearance >= 4)
            return true;

        return false;
    }

    return isActionAllowed(user, action); 
}

bool PolicyEngine::canChangeStatus(const User* user, const Task* task, int newStatus) {
    int clearance = roleToClearance(user->getRole());
    int currentStatus = task->getStatus();
    
    if (newStatus == 4) 
        return false;
    
    if (user->getName() == task->getCreator())
        return true;
    
    if (user->getName() == task->getAssignee()) {        
        if (currentStatus == 1 && newStatus == 2)
            return true;        
    }

    if (task->isExpired() && clearance >= 4)
        return true;

    return false;
}

bool PolicyEngine::isMessageAllowed(const User* sender, const User* receiver, const string& type) {
    string senderRole = sender->getRole();  
    string receiverRole = receiver->getRole();

    if (type == "PRIVATE") {
        return true; 
    }

    if (type == "ALERT") {
        return senderRole == "Director" || senderRole == "Executive" ;
    }

    if (type == "INFO") {
        return true; 
    }

    return false;  
}

bool PolicyEngine::isBroadcastInfoAllowed(const User* sender, const string& targetRole) {
    string senderRole = sender->getRole();
    int senderClearance = roleToClearance(senderRole);
    int targetClearance = roleToClearance(targetRole);
    if (senderClearance >= targetClearance) {            
        if (senderRole == "Director" || senderRole == "Executive" || senderRole == "Manager") {
            return true; 
        }
    }
    return false; 
}

bool PolicyEngine::isTaskCompletionAllowed(const User* user, const Task* task){
    return ((task->getStatus() == 1 || task->getStatus() == 2) && task->getAssignee() == user->getName()) ;
}

bool PolicyEngine::canSendNotification(const User* user, const string& type) {
    string userRole = user->getRole();
    if (type == "WARNING" || type == "INFO") {
        return userRole == "Manager" || userRole == "Director" || userRole == "Executive";
    }
    if (type == "EMERGENCY") {
        return userRole == "Director" || userRole == "Executive";
    }
    return false; 
}

bool PolicyEngine::canRemoveUser(const User* user, const User* targetUser) {
    string removerRole = user->getRole();
    string targetRole = targetUser->getRole();

    if (removerRole != "Executive" && removerRole != "Director") {
        return false;
    }

    if (targetRole == "Director") {
        return false;
    }

    return true;
}

bool PolicyEngine::canViewUsers(const User* user) {
    string userRole = user->getRole();
    if (userRole == "Executive" || userRole == "Director") {
        return true; 
    }
    return false; 
}

bool PolicyEngine::canCreateUser(const User* currentUser) {
    string role = currentUser->getRole();
    return role == "Director" || role == "Executive";
}

bool PolicyEngine::canViewAuditLog(const User* user) {
    string role = user->getRole();
    return role == "Director" || role == "Executive";
}

bool PolicyEngine::canViewPerformanceReport(const User* viewer, const User* targetUser) {
    if (viewer->getName() == targetUser->getName())
        return true;

    int viewerClearence = viewer->getclearancelevel();
    int targetClearence = targetUser->getclearancelevel();

    if (viewerClearence > targetClearence && viewerClearence > 2) {
        return true;
    }

    return false;
}

bool PolicyEngine::canDeleteUsers(const User* user){
    return user->getRole() == "Executive" || user->getRole() == "Director";
}
