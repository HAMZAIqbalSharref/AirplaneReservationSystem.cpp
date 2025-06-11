// BookingManager.h
#pragma once

#include "Seat.h"     // Include our Seat class
#include "Customer.h" // Include our Customer class
#include <msclr/marshal_cppstd.h> // For converting String^ to std::string

using namespace System;
using namespace System::Collections::Generic; // For List
using namespace System::IO; // For file operations
using namespace System::Drawing; // For Color
using namespace System::Windows::Forms; // Required for MessageBox (if still used elsewhere)

namespace CppCLRWinFormsProject {

    public ref class BookingManager {
    private:
        // Collection of all Seat objects
        List<Seat^>^ _allSeats;

        // Path for persistently storing booked seats and travel history
        String^ _bookedSeatsFilePath;
        String^ _travelHistoryFilePath;

    public:
        // Constructor: Takes an array of UI TextBoxes to link with Seat objects
        BookingManager(array<TextBox^>^ uiSeatTextBoxes) {
            _allSeats = gcnew List<Seat^>();

            // Mapping seat names for initialization (must match UI TextBox names)
            array<String^>^ seatNames = {
                "A1", "A2", "A3",
                "B1", "B2", "B3",
                "C1", "C2", "C3",
                "D1", "D2", "D3",
                "E1", "E2", "E3",
                "F1", "F2", "F3"
            };

            // Ensure UI TextBoxes are passed in the correct order or map them by name
            for (int i = 0; i < uiSeatTextBoxes->Length; i++) {
                // Find the corresponding TextBox for the current seatName
                TextBox^ currentTextBox = nullptr;
                for each(TextBox ^ tb in uiSeatTextBoxes) {
                    if (tb->Name->Equals(seatNames[i])) {
                        currentTextBox = tb;
                        break;
                    }
                }
                if (currentTextBox != nullptr) {
                    _allSeats->Add(gcnew Seat(seatNames[i], currentTextBox));
                }
                else {
                    // Handle error if a UI TextBox is not found for a seat name
                    // For robustness, you might want to log this or throw an exception.
                    MessageBox::Show("Error: UI TextBox not found for seat " + seatNames[i]);
                }
            }

            // Initialize file paths in the user's Documents folder
            String^ documentsPath = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
            String^ appFolder = Path::Combine(documentsPath, "MyBookingApp");

            // Create the application folder if it doesn't exist
            if (!Directory::Exists(appFolder)) {
                Directory::CreateDirectory(appFolder);
            }

            _bookedSeatsFilePath = Path::Combine(appFolder, "booked_seats.txt");
            _travelHistoryFilePath = Path::Combine(appFolder, "travel_history.txt");
        }

        // Public method to get all seat objects
        List<Seat^>^ GetAllSeats() {
            return _allSeats;
        }

        // Loads booking status from file and updates Seat objects and their UI
        void LoadBookedSeats() {
            // First, reset all seats to available
            for each(Seat ^ seat in _allSeats) {
                seat->Unbook(); // This will also update the UI to black/white
            }

            if (File::Exists(_bookedSeatsFilePath)) {
                try {
                    // Read all lines. Each line is expected to be "FirstName,LastName,PhoneNumber,SeatName"
                    array<String^>^ bookedEntries = File::ReadAllLines(_bookedSeatsFilePath);

                    for each(String ^ entry in bookedEntries) {
                        array<String^>^ parts = entry->Split(',');
                        if (parts->Length == 4) { // Expecting 4 parts: FN, LN, Phone, SeatName
                            String^ seatName = parts[3]->Trim();
                            String^ firstName = parts[0]->Trim();
                            String^ lastName = parts[1]->Trim();
                            String^ phoneNumber = parts[2]->Trim();

                            for each(Seat ^ seat in _allSeats) {
                                if (seat->SeatNumber->Equals(seatName, StringComparison::OrdinalIgnoreCase)) {
                                    seat->Book(); // Mark as booked (updates color to Red)
                                    // Optionally, you might store the customer details with the seat
                                    // if you want to retrieve them when a booked seat is clicked.
                                    // For now, the Seat class just tracks booking status.
                                    seat->AssociatedTextBox->Text = "BOOKED"; // Update text to BOOKED
                                    seat->AssociatedTextBox->Enabled = false; // Disable the seat
                                    break;
                                }
                            }
                        }
                    }
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Error loading booked seats: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        // Saves booking status to file and travel history
        void SaveBooking(Customer^ customer, List<Seat^>^ bookedSeats, String^ selectedClass, String^ selectedPrice) {
            try {
                // Prepare entries for booked_seats.txt
                List<String^>^ newBookedSeatsEntries = gcnew List<String^>();
                for each(Seat ^ seat in bookedSeats) {
                    newBookedSeatsEntries->Add(String::Format("{0},{1},{2},{3}",
                        customer->FirstName, customer->LastName, customer->PhoneNumber, seat->SeatNumber));
                    seat->Book(); // Mark the seat as booked (updates UI to Red)
                    seat->AssociatedTextBox->Text = "BOOKED";
                    seat->AssociatedTextBox->Enabled = false;
                }

                // Read existing entries from booked_seats.txt to avoid overwriting all previous bookings
                List<String^>^ existingBookedSeats = gcnew List<String^>();
                if (File::Exists(_bookedSeatsFilePath)) {
                    existingBookedSeats->AddRange(File::ReadAllLines(_bookedSeatsFilePath));
                }
                existingBookedSeats->AddRange(newBookedSeatsEntries);
                File::WriteAllLines(_bookedSeatsFilePath, existingBookedSeats->ToArray());

                // FIX: Create a separate list of seat numbers for String::Join
                List<String^>^ seatNumbersForTravelHistory = gcnew List<String^>();
                for each(Seat ^ s in bookedSeats) {
                    seatNumbersForTravelHistory->Add(s->SeatNumber);
                }

                // Save to travel_history.txt
                String^ travelHistoryEntry = String::Format("{0},{1},{2},{3},{4},{5}",
                    customer->FirstName, customer->LastName, customer->PhoneNumber, selectedClass, selectedPrice,
                    String::Join(";", seatNumbersForTravelHistory)); // Use the new list here
                File::AppendAllLines(_travelHistoryFilePath, gcnew array<String^>{travelHistoryEntry});

                // Removed: MessageBox::Show("Booking confirmed!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);

            }
            catch (Exception^ ex) {
                MessageBox::Show("Error saving booking data: " + ex->Message, "Save Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // Resets all seats to available and clears booking files
        void ResetSystem() {
            for each(Seat ^ seat in _allSeats) {
                seat->Unbook(); // Unbooks the seat and updates its UI to black/white
                seat->AssociatedTextBox->Text = seat->SeatNumber; // Reset text to seat number
                seat->AssociatedTextBox->Enabled = true; // Re-enable the seat
            }

            try {
                if (File::Exists(_bookedSeatsFilePath)) {
                    File::Delete(_bookedSeatsFilePath);
                }
                // Optionally delete travel_history.txt if you want a full wipe
                // if (File::Exists(_travelHistoryFilePath)) {
                //      File::Delete(_travelHistoryFilePath);
                // }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error clearing booked seats data: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }

            // Removed: MessageBox::Show("System has been reset. All seats are now available.", "Reset Successful", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        // Provides pricing information based on selected class
        List<String^>^ GetPricesForClass(String^ selectedClass) {
            List<String^>^ prices = gcnew List<String^>();
            if (selectedClass == "Economy") {
                prices->Add("150");
            }
            else if (selectedClass == "Business") {
                prices->Add("300");
            }
            else if (selectedClass == "First Class") {
                prices->Add("500");
            }
            return prices;
        }
    };
}