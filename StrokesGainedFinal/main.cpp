// Matthew Cichowlas
// ITCS-2530
// 
// This is a strokes gained calculator based on 
// PGA Tour data.
// Golf Statistics
// Chat GPT-Model 4o

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_SHOTS = 20; // Assume max 20 shots per hole (very high for safety)

// Enum for Lie type
enum LieType { TEE, FAIRWAY, ROUGH, SAND, GREEN, UNKNOWN };

LieType convertLie(const string& str) {
    if (str == "tee") return TEE;
    if (str == "fairway") return FAIRWAY;
    if (str == "rough") return ROUGH;
    if (str == "sand") return SAND;
    if (str == "green") return GREEN;
    return UNKNOWN;
}

string lieToString(LieType lie) {
    switch (lie) {
    case TEE: return "Tee";
    case FAIRWAY: return "Fairway";
    case ROUGH: return "Rough";
    case SAND: return "Sand";
    case GREEN: return "Green";
    default: return "Unknown";
    }
}

// Simplified benchmark values based on tour stats
double getBenchmark(double distance, LieType lie) {
    if (lie == GREEN) return distance / 8.0; // estimate putts from feet
    if (lie == TEE && distance >= 400) return 4.08;
    if (lie == FAIRWAY && distance > 180) return 3.18;
    if (lie == ROUGH && distance <= 60) return 2.91;
    if (distance > 250) return 3.9;
    if (distance > 200) return 3.6;
    if (distance > 150) return 3.3;
    if (distance > 100) return 2.8;
    if (distance > 50) return 2.2;
    if (distance > 20) return 1.8;
    return 1.2;
}

// User-defined struct
struct Shot {
    double startDistance;
    LieType startLie;
    double endDistance;
    LieType endLie;
    double startSG;
    double endSG;
    double strokesGained;
};

// User-defined class
class Stats {
private:
    double totalStrokesGained;
public:
    Stats() { totalStrokesGained = 0; }
    void add(double sg) { totalStrokesGained += sg; }
    double getTotal() const { return totalStrokesGained; }
};

void displayMenu() {
    cout << "\n===== STROKES GAINED FOR SINGLE HOLE =====\n";
    cout << "1. Enter and calculate strokes gained\n";
    cout << "2. Read saved strokes data from file\n";
    cout << "3. Exit\n";
    cout << "Select an option: ";
}

void readShotsFromFile() {
    ifstream inFile("hole_data.txt");
    string line;
    cout << "\n-- Strokes Data from File --\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

void enterAndCalculate(Shot shots[], int& shotCount, Stats& stats) {
    ofstream outFile("hole_data.txt");
    int par, strokes;
    cout << "\nPar for the hole: ";
    cin >> par;
    cout << "Shots taken on this hole: ";
    cin >> strokes;
    shotCount = strokes;

    for (int i = 0; i < strokes; ++i) {
        cout << "\n--- Shot " << i + 1 << " ---\n";
        cout << "Start distance to hole (yards or feet): ";
        cin >> shots[i].startDistance;
        string startLieStr;
        cout << "Start lie (tee, fairway, rough, sand, green): ";
        cin >> startLieStr;
        shots[i].startLie = convertLie(startLieStr);

        cout << "Finish distance to hole (yards or feet): ";
        cin >> shots[i].endDistance;
        string endLieStr;
        cout << "Finish lie (tee, fairway, rough, sand, green): ";
        cin >> endLieStr;
        shots[i].endLie = convertLie(endLieStr);

        shots[i].startSG = getBenchmark(shots[i].startDistance, shots[i].startLie);
        shots[i].endSG = getBenchmark(shots[i].endDistance, shots[i].endLie);
        shots[i].strokesGained = shots[i].startSG - shots[i].endSG - 1.0;
        stats.add(shots[i].strokesGained);

        outFile << fixed << setprecision(2)
            << "Shot " << i + 1 << ": StartDist=" << shots[i].startDistance << " (" << startLieStr << "), EndDist="
            << shots[i].endDistance << " (" << endLieStr << "), SG=" << shots[i].strokesGained << "\n";
    }

    outFile << "\nTotal Strokes Gained for Hole: " << fixed << setprecision(2) << stats.getTotal() << endl;
    outFile.close();
    cout << "\nTotal Strokes Gained for Hole: " << fixed << setprecision(2) << stats.getTotal() << endl;
}

int main() {
    Shot shots[MAX_SHOTS];
    Stats stats;
    int shotCount = 0;
    int option;

    do {
        displayMenu();
        cin >> option;

        switch (option) {
        case 1:
            enterAndCalculate(shots, shotCount, stats);
            break;
        case 2:
            readShotsFromFile();
            break;
        case 3:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 3);

    return 0;
}
