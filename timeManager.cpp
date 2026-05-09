#include "pch.h"
#include "timeManager.h"

time_t TimeManager::getCurrentTime() {
    return time(nullptr); 
}

bool TimeManager::isExpired(time_t createdAt, int ttlSeconds) {
    time_t now = getCurrentTime();
    return (now - createdAt) >= ttlSeconds;
}

int TimeManager::getTimeDifferenceSeconds(time_t t1, time_t t2) {
    return static_cast<int>(difftime(t1, t2));
}
