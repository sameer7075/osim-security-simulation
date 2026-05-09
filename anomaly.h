#pragma once
#include "pch.h"
#include "audit.h"

class Anomaly {
public:
    long timestamp;
    string username;
    string reason;

    Anomaly();
    Anomaly(long ts, const string& user, const string& r);
};

class AnomalyDetector {
private:
    Audit** entries;  
    int entryCount;
    int entryCapacity;

    Anomaly** anomalies;  
    int anomalyCount;
    int anomalyCapacity;

    
    void resizeEntryArray();
    void addEntry(Audit* newEntry);

    
    void resizeAnomalyArray();
    void addAnomaly(Anomaly* newAnomaly);

public:
    AnomalyDetector();
    ~AnomalyDetector();

    void loadAuditLog();
    void detectAnomalies();
    void writeAnomalies();
    void viewAnomalyReport();
};


