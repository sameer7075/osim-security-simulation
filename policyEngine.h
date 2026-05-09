#pragma once

#include "user.h"
#include "task.h"


#define ACTION_SEND_MESSAGE_INFO "SendMessageInfo"
#define ACTION_SEND_MESSAGE_PRIVATE "SendMessagePrivate"
#define ACTION_SEND_MESSAGE_ALERT "SendMessageAlert"
#define ACTION_ASSIGN_TASK "AssignTask"
#define ACTION_SEND_NOTIFICATION_WARNING "SendNotificationWarning"
#define ACTION_SEND_NOTIFICATION_EMERGENCY "SendNotificationEmergency"
#define ACTION_GENERATE_PERFORMANCE_REPORT "GeneratePerformanceReport"
#define ACTION_VIEW_AUDIT_LOG "ViewAuditLog"
#define ACTION_DELEGATE_TASK "DelegateTask"
#define ACTION_CHANGE_STATUS "ChangeStatus"
#define ACTION_CREATE_TASK "CreateTask"
#define ACTION_DELETE_TASK "DeleteTask"
#define ACTION_VIEW_ALL_TASKS "ViewAllTasks"
#define ACTION_VIEW_ASSIGNED_TASKS "ViewAssignedTasks"
#define ACTION_VIEW_DELEGATED_TASKS "ViewDelegatedTasks"

class PolicyEngine {
public:
    
    static bool isActionAllowed(const User* user, const char* action);
    
    static bool isActionAllowed(const User* user, const char* action, const User* targetUser);
    
    static bool isActionAllowed(const User* user, const char* action, const Task* task); 
    
    static bool canChangeStatus(const User* user, const Task* task, int newStatus);

    static bool isMessageAllowed(const User* sender, const User* receiver, const string& type);

    static bool isBroadcastInfoAllowed(const User* sender, const string& targetRole);

    static bool isTaskCompletionAllowed(const User* user, const Task* task);

    static bool canSendNotification(const User* user, const string& type);

    static bool canRemoveUser(const User* user, const User* targetUser);

    static bool canViewUsers(const User* user);

    static bool canCreateUser(const User* user);

    static bool canViewAuditLog(const User* user);

    static bool canViewPerformanceReport(const User* viewer, const User* targetUser);

    static bool canDeleteUsers(const User* user);

private:
    
    static int roleToClearance(const string role);
};