// PriceCalculator.h
#pragma once

using namespace System;

namespace CppCLRWinFormsProject {

    public ref class PriceCalculator {
    public:
        
        static double CalculateTotalPrice(double pricePerSeat, int numberOfSeats) {
            if (pricePerSeat < 0 || numberOfSeats < 0) {
                return 0.0; 
            }
            return pricePerSeat * numberOfSeats;
        }
    };
}