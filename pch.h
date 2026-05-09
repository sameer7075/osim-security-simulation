#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

const string TASK_STATUS_STRINGS[] = {"Created", "Assigned", "InProgress", "Completed", "Expired"};

const string TASK_PRIORITY_STRINGS[] = {"Low", "Medium", "High"};

const string ROLE_STRINGS[] = {"Junior", "Employee", "Manager", "Director", "Executive"};

const string MESSAGE_TYPE_STRINGS[] = {"PRIVATE", "ALERT", "INFO"};

const string NOTIFICATION_TYPE_STRINGS[] = {"INFO", "WARNING", "EMERGENCY"};

const string banner = 
                    "                                                    ███████     █████████  █████ ██████   ██████\n"
                    "                                                  ███░░░░░███  ███░░░░░███░░███ ░░██████ ██████ \n"
                    "                                                 ███     ░░███░███    ░░░  ░███  ░███░█████░███ \n"
                    "                                                ░███      ░███░░█████████  ░███  ░███░░███ ░███ \n"
                    "                                                ░███      ░███ ░░░░░░░░███ ░███  ░███ ░░░  ░███ \n"
                    "                                                ░░███     ███  ███    ░███ ░███  ░███      ░███ \n"
                    "                                                 ░░░███████░  ░░█████████  █████ █████     █████\n"
                    "                                                   ░░░░░░░     ░░░░░░░░░  ░░░░░ ░░░░░     ░░░░░ \n";
 