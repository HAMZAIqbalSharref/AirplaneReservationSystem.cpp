#include <iostream>
using namespace std;

// Constants for seat rows
const int ROWS = 12; // Total rows
const int COLUMNS = 7; // Seats in each row
int seating[ROWS][COLUMNS]; // 2D array for seat reservations (0 = available, 1 = reserved)

// Initialize the seating array with all seats available
void initializeSeating() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            seating[i][j] = 0; // All seats start as available
        }
    }
}

// Display the seating arrangement
void displaySeating() {
    cout << "Seating Plan (0 = Available, 1 = Reserved):" << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < COLUMNS; j++) {
            cout << seating[i][j] << " ";
        }
        cout << endl;
    }
}

// Display fare for all classes
void displayFare() {
    cout << "--------------------------------------------" << endl;
    cout << "Fare for each class:" << endl;
    cout << "First Class: 18,000 per seat" << endl;
    cout << "Business Class: 14,000 per seat" << endl;
    cout << "Economy Class: 10,000 per seat" << endl;
    cout << "--------------------------------------------" << endl;
}

// Book seats in the specified rows
void bookSeats(int startRow, int endRow, int numSeats) {
    int booked = 0;
    for (int i = startRow; i <= endRow && booked < numSeats; i++) {
        for (int j = 0; j < COLUMNS && booked < numSeats; j++) {
            if (seating[i][j] == 0) { // Seat is available
                seating[i][j] = 1; // Reserve the seat
                booked++;
            }
        }
    }

    if (booked == numSeats) {
        cout << numSeats << " seat(s) successfully reserved!" << endl;
    } else {
        cout << "Not enough seats available in the selected class!" << endl;
    }
}

// Reset the entire seating system
void resetSystem() {
    initializeSeating(); // Reset seating array
    cout << "System has been reset. All seats are now available." << endl;
}

int main() {
    initializeSeating(); // Initialize the seating array
    int choice, numSeats;

    while (true) {
        cout << "--------------------------------------------------" << endl;
        cout << "  Welcome to Airplane Reservation System " << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1: Book First Class Seats (Rows 0-1)" << endl;
        cout << "2: Book Business Class Seats (Rows 2-3)" << endl;
        cout << "3: Book Economy Class Seats (Rows 4-11)" << endl;
        cout << "4: View Airplane Seating Plan" << endl;
        cout << "5: View Fare for All Classes" << endl;
        cout << "6: System Reset" << endl;
        cout << "7: Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the number of First Class seats to book: ";
                cin >> numSeats;
                bookSeats(0, 1, numSeats);
                break;
            case 2:
                cout << "Enter the number of Business Class seats to book: ";
                cin >> numSeats;
                bookSeats(2, 3, numSeats);
                break;
            case 3:
                cout << "Enter the number of Economy Class seats to book: ";
                cin >> numSeats;
                bookSeats(4, 11, numSeats);
                break;
            case 4:
                displaySeating();
                break;
            case 5:
                displayFare();
                break;
            case 6:
                resetSystem();
                break;
            case 7:
                cout << "Exiting the booking system. Thank you!" << endl;
                return 0;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
