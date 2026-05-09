#pragma once
#include "user.h"
#include "audit.h"
#include "messageManager.h"
#include "notificationManager.h"
#include "perfomanceReview.h"
#include "policyEngine.h"
#include "taskManager.h"
#include "timeManager.h"
#include "userManager.h"
#include "utilities.h"
#include "anomaly.h"

void mainMenu(User* );
void taskManagement(const User* currentUser);
void messageManagement(const User* currentUser);
void notificationManagement(const User* currentUser);
void reviewManagement(const User* currentUser);
void auditManagement(const User* currentUser);
void userManagement(const User* currentUser);
void accountSettings(User* currentUser);