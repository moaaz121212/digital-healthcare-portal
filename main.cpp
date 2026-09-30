#include <iostream>
#include <string>
#include <vector>

using namespace std;

// --- BASE CLASSES --- //

class MedicalRecord {
protected:
    string history;
    string prescriptions;
public:
    virtual void displayRecord() = 0;
    virtual ~MedicalRecord() {}
};

class User {
protected:
    string id;
    string password;
public:
    User(string uid, string pass) : id(uid), password(pass) {}
    string getId() { return id; }
    virtual bool authenticate(string pass) { return password == pass; }
    virtual void changePassword(string newPass) { password = newPass; }
    virtual ~User() {}
};

class PortalSystem {
public:
    virtual void run() = 0;
    virtual ~PortalSystem() {}
};

// --- DERIVED CLASSES --- //

class PatientRecord : public MedicalRecord {
public:
    int appointments[3][3]; // 2D Schedule representing time slots

    PatientRecord() {
        history = "No history available.";
        prescriptions = "No prescriptions.";
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                appointments[i][j] = 0; // 0 means available, 1 means booked
    }

    void updateHistory(string newHistory) { history = newHistory; }
    void updatePrescriptions(string newRx) { prescriptions = newRx; }
    string getPrescriptions() const { return prescriptions; }
    
    void displayRecord() override {
        cout << "\n--- Medical History ---\n" << history << "\n";
        cout << "--- Prescriptions ---\n" << prescriptions << "\n";
    }

    void displaySchedule() {
        cout << "\n--- 2D Appointment Layout (0=Free, 1=Booked) ---\n";
        cout << "      Morning Afternoon Evening\n";
        string days[] = {"Mon", "Tue", "Wed"};
        for(int i=0; i<3; i++) {
            cout << days[i] << "    ";
            for(int j=0; j<3; j++) {
                cout << appointments[i][j] << "         ";
            }
            cout << "\n";
        }
    }

    void bookVisit(int day, int time) {
        if (day >= 0 && day < 3 && time >= 0 && time < 3 && appointments[day][time] == 0) {
            appointments[day][time] = 1;
            cout << "Visit booked successfully.\n";
        } else {
            cout << "Slot unavailable or invalid.\n";
        }
    }
};

class Patient : public User {
private:
    PatientRecord record;
public:
    Patient(string uid, string pass) : User(uid, pass) {}
    
    PatientRecord& getRecord() { return record; }

    void patientMenu() {
        int choice;
        do {
            cout << "\n--- Patient Menu ---\n";
            cout << "1. Medical Account (View History & Book Visits)\n";
            cout << "2. Inquiry Account (View Prescriptions Only)\n";
            cout << "3. Change Password\n";
            cout << "4. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            if (choice == 1) {
                record.displayRecord();
                record.displaySchedule();
                cout << "Enter day (0-2) and time (0-2) to book a visit (or -1 to cancel): ";
                int d, t;
                cin >> d;
                if (d != -1) {
                    cin >> t;
                    record.bookVisit(d, t);
                }
            } else if (choice == 2) {
                cout << "\n--- Inquiry Account ---\n";
                cout << "Prescriptions: " << record.getPrescriptions() << "\n";
            } else if (choice == 3) {
                string newPass;
                cout << "Enter new password: ";
                cin >> newPass;
                changePassword(newPass);
                cout << "Password updated.\n";
            }
        } while (choice != 4);
    }
};

class DigitalPortal : public PortalSystem {
private:
    vector<Patient> patients;
    const string adminPass = "admin123";
    const int MAX_RECORDS = 100;

    void doctorMode() {
        string pass;
        cout << "Enter Admin Password: ";
        cin >> pass;
        if (pass != adminPass) {
            cout << "Access Denied.\n";
            return;
        }

        int choice;
        do {
            cout << "\n--- Doctor Menu ---\n";
            cout << "1. Create Patient Profile\n";
            cout << "2. Delete Patient Profile\n";
            cout << "3. Update Patient Medical Record\n";
            cout << "4. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            if (choice == 1) {
                if (patients.size() >= MAX_RECORDS) {
                    cout << "Maximum capacity reached (100 records).\n";
                    continue;
                }
                string id, pswd;
                cout << "Enter New Patient ID: ";
                cin >> id;
                cout << "Enter Initial Password: ";
                cin >> pswd;
                patients.push_back(Patient(id, pswd));
                cout << "Patient created.\n";
            } else if (choice == 2) {
                string id;
                cout << "Enter Patient ID to delete: ";
                cin >> id;
                for (auto it = patients.begin(); it != patients.end(); ++it) {
                    if (it->getId() == id) {
                        patients.erase(it);
                        cout << "Patient deleted.\n";
                        break;
                    }
                }
            } else if (choice == 3) {
                string id;
                cout << "Enter Patient ID to update: ";
                cin >> id;
                for (auto &p : patients) {
                    if (p.getId() == id) {
                        string data;
                        cin.ignore();
                        cout << "Enter new diagnostic history: ";
                        getline(cin, data);
                        p.getRecord().updateHistory(data);
                        cout << "Enter new prescriptions: ";
                        getline(cin, data);
                        p.getRecord().updatePrescriptions(data);
                        cout << "Record updated.\n";
                        break;
                    }
                }
            }
        } while (choice != 4);
    }

    void patientMode() {
        string id, pass;
        cout << "Enter Patient ID: ";
        cin >> id;
        
        Patient* currentPatient = nullptr;
        for (auto &p : patients) {
            if (p.getId() == id) {
                currentPatient = &p;
                break;
            }
        }

        if (!currentPatient) {
            cout << "Patient not found.\n";
            return;
        }

        int attempts = 0;
        while (attempts < 3) {
            cout << "Enter Password: ";
            cin >> pass;
            if (currentPatient->authenticate(pass)) {
                currentPatient->patientMenu();
                return;
            } else {
                attempts++;
                cout << "Incorrect password. Attempts left: " << (3 - attempts) << "\n";
            }
        }
        cout << "Security Alert: 3 failed attempts. System terminating.\n";
        exit(0);
    }

public:
    void run() override {
        int choice;
        while (true) {
            cout << "\n=== Digital Healthcare Patient Portal ===\n";
            cout << "1. DOCTOR Mode\n";
            cout << "2. PATIENT Mode\n";
            cout << "3. EXIT\n";
            cout << "Select Mode: ";
            cin >> choice;

            if (choice == 1) {
                doctorMode();
            } else if (choice == 2) {
                patientMode();
            } else if (choice == 3) {
                cout << "Exiting system. Goodbye!\n";
                break;
            } else {
                cout << "Invalid selection.\n";
            }
        }
    }
};

// --- MAIN FUNCTION --- //

int main() {
    DigitalPortal portal;
    portal.run();
    return 0;
}
