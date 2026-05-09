#include "anomaly.h"


Anomaly::Anomaly(){}

Anomaly::Anomaly(long ts, const string& user, const string& r){
    timestamp = ts;
    username = user;
    reason = r;
}

AnomalyDetector::AnomalyDetector() {
    entries = nullptr;
    entryCount = 0;
    entryCapacity = 5;

    anomalies = nullptr;
    anomalyCount = 0;
    anomalyCapacity = 5;

    
    entries = new Audit*[entryCapacity];
    anomalies = new Anomaly*[anomalyCapacity];
}

AnomalyDetector::~AnomalyDetector() {
    for (int i = 0; i < entryCount; ++i) {
        delete entries[i];  
    }
    delete[] entries;  

    for (int i = 0; i < anomalyCount; ++i) {
        delete anomalies[i];  
    }
    delete[] anomalies;  
}

void AnomalyDetector::resizeEntryArray() {
    int newCapacity = entryCapacity * 2;
    Audit** temp = new Audit*[newCapacity];

    for (int i = 0; i < entryCount; ++i)
        temp[i] = entries[i];

    delete[] entries;
    entries = temp;
    entryCapacity = newCapacity;
}

void AnomalyDetector::addEntry(Audit* newEntry) {
    if (entryCount == entryCapacity) {
        resizeEntryArray();
    }
    entries[entryCount++] = newEntry;
}

void AnomalyDetector::resizeAnomalyArray() {
    int newCapacity = anomalyCapacity * 2;
    Anomaly** temp = new Anomaly*[newCapacity];

    for (int i = 0; i < anomalyCount; ++i)
        temp[i] = anomalies[i];

    delete[] anomalies;
    anomalies = temp;
    anomalyCapacity = newCapacity;
}

void AnomalyDetector::addAnomaly(Anomaly* newAnomaly) {
    if (anomalyCount == anomalyCapacity) {
        resizeAnomalyArray();
    }
    anomalies[anomalyCount++] = newAnomaly;
}

void AnomalyDetector::loadAuditLog() {
    ifstream file("audit.txt");

    string tsStr, user, role, action, status;
    time_t now = time(0);
    tm* now_tm = localtime(&now);
    int currentMonth = now_tm->tm_mon;
    int currentYear = now_tm->tm_year;
    
    while (getline(file, tsStr, '|') &&
            getline(file, user, '|') &&
            getline(file, role, '|') &&
            getline(file, action, '|') &&
            getline(file, status)) {       

        time_t ts = stol(tsStr);
        tm* ts_tm = localtime(&ts);

        if (ts_tm->tm_mon == currentMonth && ts_tm->tm_year == currentYear) {
            Audit* log = new Audit(user, role, action, status);
            log->setTimestamp(ts);
            addEntry(log);
        }
    }
    file.close();
}

void AnomalyDetector::detectAnomalies() {
    for (int i = 0; i < entryCount; ++i) {
        Audit* e = entries[i];
        if (e->getAction() == "Authenticated" && e->getStatus() == "Failed") {
            int fails = 1;
            bool skip = false;
            for (int j = i + 1; j < entryCount && !skip; ++j) {
                if (entries[j]->getUsername() == e->getUsername() &&
                    entries[j]->getAction() == "Authenticated" &&
                    entries[j]->getStatus() == "Failed" &&
                    entries[j]->getTimestamp() - e->getTimestamp() <= 300) {
                    ++fails;
                }
            }
            if (fails >= 3) {
                string reason = "Multiple failed logins in short time";
                skip = true;
                addAnomaly(new Anomaly(e->getTimestamp(), e->getUsername(), reason));
            }
        }
        
        if (e->getAction() == "Authenticated" && e->getStatus() == "Success") {
            time_t t = e->getTimestamp();
            int hours = (t / 3600) % 24;  
            if (hours >= 2 && hours <= 5) {
                string reason = "Login at unusual hour";
                addAnomaly(new Anomaly(e->getTimestamp(), e->getUsername(), reason));
            }
        }
        
        if (e->getAction() == "Changed password") {
            for (int j = i + 1; j < entryCount; ++j) {
                Audit* next = entries[j];
                if (next->getUsername() == e->getUsername()) {
                    if (next->getAction() == "Authenticated") {
                        if (next->getStatus() == "Failed") {
                            string reason = "Failed login after password change";
                            addAnomaly(new Anomaly(next->getTimestamp(), next->getUsername(), reason));
                        }
                        break; 
                    }
                }
            }
        }

        if (e->getAction() == "Changed password" && e->getStatus() == "Failed") {
                string reason = "Failed to change Password";
                addAnomaly(new Anomaly(e->getTimestamp(), e->getUsername(), reason));            
        }
    }
}

void AnomalyDetector::writeAnomalies() {
    ofstream out("anomalyLog.txt");
    for (int i = 0; i < anomalyCount; ++i) {
        out << anomalies[i]->timestamp << "|"
            << anomalies[i]->username << "|"
            << anomalies[i]->reason << "\n";
    }
    out.close();
}

void AnomalyDetector::viewAnomalyReport() {
    ifstream in("anomalyLog.txt");

    string tsStr, user, reason;
    cout << "\n---- Anomaly Report ----\n";
    cout << left << setw(35) << "Timestamp" << setw(20) << "Username" << "Reason\n\n";

    while (getline(in, tsStr, '|') &&
           getline(in, user, '|') &&
           getline(in, reason)) {
        time_t ts = stol(tsStr);
        char* timeStr = ctime(&ts);
        timeStr[strlen(timeStr) - 1] = '\0'; 

        cout << left << setw(35) << timeStr << setw(20) << user << reason << "\n";
    }
    in.close();
}

