#pragma once

class TimeManager {
public:
    static time_t getCurrentTime();

    static bool isExpired(time_t createdAt, int ttlSeconds);

    static int getTimeDifferenceSeconds(time_t t1, time_t t2);
};

