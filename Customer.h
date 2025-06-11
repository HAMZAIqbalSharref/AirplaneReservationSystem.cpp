// Customer.h
#pragma once

using namespace System;

namespace CppCLRWinFormsProject {

    public ref class Customer {
    private:
        String^ _firstName;
        String^ _lastName;
        String^ _phoneNumber;

    public:
        // Constructor
        Customer(String^ firstName, String^ lastName, String^ phoneNumber)
            : _firstName(firstName), _lastName(lastName), _phoneNumber(phoneNumber) {
        }

        // Properties
        property String^ FirstName {
            String^ get() { return _firstName; }
            void set(String^ value) { _firstName = value; }
        }

        property String^ LastName {
            String^ get() { return _lastName; }
            void set(String^ value) { _lastName = value; }
        }

        property String^ PhoneNumber {
            String^ get() { return _phoneNumber; }
            void set(String^ value) { _phoneNumber = value; }
        }

        // Method to get full name (example)
        String^ GetFullName() {
            return _firstName + " " + _lastName;
        }
    };
}