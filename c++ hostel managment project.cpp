#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

void printHeader(const string& title) {
    cout << "\n====================================" << endl;
    cout << "      " << title << endl;
    cout << "====================================\n" << endl;
}

class Student {
public:
    int id;
    string name;
    string roomNumber;
    string checkInDate;
    float feesDue;

    void input() {
        printHeader("Student Registration");
        while (true) {
            cout << "Enter Student ID: ";
            string inputID;
            cin >> inputID;
            if (inputID.find_first_not_of("0123456789") != string::npos) {
                cout << "[Error] Please enter digits only!" << endl;
            } else {
                id = stoi(inputID);
                break;
            }
        }

        cin.ignore();
        while (true) {
            cout << "Enter Student Name: ";
            getline(cin, name);
            if (name.find_first_of("0123456789") != string::npos) {
                cout << "[Error] Name cannot contain digits!" << endl;
            } else {
                break;
            }
        }

        cout << "Enter Check-in Date (YYYY-MM-DD): ";
        getline(cin, checkInDate);

        cout << "Is there any fees due to pay? (yes/no): ";
        string response;
        cin >> response;
        if (response == "yes" || response == "Yes") {
            cout << "Enter Fees Due: ";
            while (!(cin >> feesDue)) {
                cout << "[Error] Invalid amount! Enter numeric value: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } else {
            feesDue = 0.0;
        }
        roomNumber = "Unassigned";
        cout << "\n[Info] Student added successfully!" << endl;
    }

    void display() const {
        cout << left << setw(5) << id << setw(20) << name
             << setw(15) << roomNumber << setw(12) << checkInDate 
             << "$" << setw(10) << feesDue << endl;
    }
};

class Room {
public:
    string roomNumber;
    bool isOccupied;
    int occupantId;

    Room(string number) : roomNumber(number), isOccupied(false), occupantId(-1) {}
};

class Hostel {
private:
    vector<Student> students;
    vector<Room> rooms;

public:
    Hostel() {
        for (int i = 1; i <= 100; i++) {
            rooms.push_back(Room("R" + to_string(i)));
        }
    }

    void addStudent() {
        Student s;
        s.input();
        students.push_back(s);
    }

    void allocateRoom(int studentId) {
        printHeader("Available Rooms");
        for (auto &room : rooms) {
            if (!room.isOccupied) {
                cout << room.roomNumber << "  ";
            }
        }
        cout << "\n\nEnter the room name you want (e.g., R1 or 23): ";
        string desiredRoom;
        cin >> desiredRoom;

        if (desiredRoom[0] != 'R' && desiredRoom[0] != 'r') {
            desiredRoom = "R" + desiredRoom;
        } else {
            desiredRoom[0] = 'R';
        }

        int roomNumberInt = -1;
        try {
            roomNumberInt = stoi(desiredRoom.substr(1));
        } catch (exception& e) {
            cout << "[Error] Invalid room format!" << endl;
            return;
        }

        if (roomNumberInt < 1 || roomNumberInt > 100) {
            cout << "[Error] Please enter a room number between 1 and 100." << endl;
            return;
        }

        cout << "Enter Room Type (1-seater / 2-seater / 4-seater): ";
        string type;
        cin >> type;

        cout << "Do you want an attached washroom? (yes/no): ";
        string attach;
        cin >> attach;

        if (attach == "yes") {
            cout << "[Note] Rooms with attached washrooms are being checked...\n";
        }

        for (auto &room : rooms) {
            if (room.roomNumber == desiredRoom && !room.isOccupied) {
                for (auto &student : students) {
                    if (student.id == studentId) {
                        room.isOccupied = true;
                        room.occupantId = studentId;
                        student.roomNumber = room.roomNumber;
                        cout << "[Success] Room " << room.roomNumber << " allocated to " << student.name << "!" << endl;
                        return;
                    }
                }
            }
        }

        cout << "[Error] Either the room is occupied or invalid input." << endl;
    }

    void displayStudents() {
        printHeader("Student List");
        if (students.empty()) {
            cout << "[Info] No students registered yet." << endl;
            return;
        }
        for (const auto &student : students) {
            student.display();
        }
    }

    void payFees() {
        int id;
        cout << "Enter Student ID for fee payment: ";
        cin >> id;

        for (auto &student : students) {
            if (student.id == id) {
                if (student.feesDue > 0) {
                    cout << "Total Due: $" << student.feesDue << "\nEnter amount to pay: ";
                    float payment;
                    while (!(cin >> payment) || payment <= 0) {
                        cout << "[Error] Enter a valid payment amount: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }

                    if (payment >= student.feesDue) {
                        cout << "[Info] Fees fully paid! Change: $" << payment - student.feesDue << endl;
                        student.feesDue = 0;
                    } else {
                        student.feesDue -= payment;
                        cout << "[Info] Partial payment accepted. Remaining Due: $" << student.feesDue << endl;
                    }
                    return;
                } else {
                    cout << "[Info] No pending fees for this student." << endl;
                    return;
                }
            }
        }
        cout << "[Error] Student not found." << endl;
    }

    void roomAvailability() {
        printHeader("Available Rooms");
        for (const auto &room : rooms) {
            if (!room.isOccupied) {
                cout << room.roomNumber << "  ";
            }
        }
        cout << endl;
    }
};

void showMenu() {
    printHeader("Hostel Management System");
    cout << "1. Add Student\n2. Allocate Room\n3. Display All Students\n4. Pay Fees\n5. Room Availability\n6. Exit\n";
}

int main() {
    Hostel hostel;
    int choice;
    do {
        showMenu();
        cout << "Select an option: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "[Error] Invalid choice! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: hostel.addStudent(); break;
            case 2: {
                string input;
                cout << "Enter Student ID: ";
                cin >> input;
                if (input.find_first_not_of("0123456789") != string::npos) {
                    cout << "[Error] Invalid input! Enter digits only." << endl;
                } else {
                    hostel.allocateRoom(stoi(input));
                }
                break;
            }
            case 3: hostel.displayStudents(); break;
            case 4: hostel.payFees(); break;
            case 5: hostel.roomAvailability(); break;
            case 6: cout << "Exiting program." << endl; break;
            default: cout << "[Error] Invalid choice!" << endl;
        }
    } while (choice != 6);

    return 0;
}
