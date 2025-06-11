// Seat.h
#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms; // Required for TextBox

namespace CppCLRWinFormsProject {

    // Base class for a seat
    public ref class Seat {
    protected: // Protected so derived classes can access
        String^ _seatNumber;
        bool _isBooked;
        TextBox^ _associatedTextBox; // The UI element for this seat

    public:
        // Constructor
        Seat(String^ seatNumber, TextBox^ associatedTextBox)
            : _seatNumber(seatNumber), _isBooked(false), _associatedTextBox(associatedTextBox) {
            UpdateTextBoxAppearance(); // Initial appearance
        }

        // Properties
        property String^ SeatNumber{
            String ^ get() { return _seatNumber; }
        }

            property bool IsBooked{
                bool get() { return _isBooked; }
                void set(bool value) {
                    _isBooked = value;
                    UpdateTextBoxAppearance(); // Update UI when booking status changes
                }
        }

            property TextBox^ AssociatedTextBox{
                TextBox ^ get() { return _associatedTextBox; }
        }

            // Methods
            virtual void Book() { 
            if (!_isBooked) {
                IsBooked = true;
                
            }
           
        }

        virtual void Unbook() { 
            if (_isBooked) {
                IsBooked = false;
              
            }
           
        }

        // Method to update the UI (color) based on booking status
        virtual void UpdateTextBoxAppearance() {
            if (_associatedTextBox != nullptr) {
                if (_isBooked) {
                    _associatedTextBox->BackColor = Color::Red; // Booked seats are red
                    _associatedTextBox->ForeColor = Color::White; // Keep text white on red
                    _associatedTextBox->Enabled = false; // Disable booked seats
                }
                else {
                    _associatedTextBox->BackColor = Color::Black; // Available seats are black
                    _associatedTextBox->ForeColor = Color::White; // Keep text white on black
                    _associatedTextBox->Enabled = true; // Enable available seats
                }
            }
        }
    };
}