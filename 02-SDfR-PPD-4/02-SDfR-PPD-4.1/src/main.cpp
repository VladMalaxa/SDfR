//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the main function for assignment 4.1, where we take the 
// weight of the package and calculate the cost of shipping it using the calculate_Cost function. 
//==============================================================


#include <iostream>
#include <vector>

#include "Package.h"
#include "TwoDayPackage.h"
#include "OvernightPackage.h"


// Function for printing information and shipping cost for each package
// The function receives a vector containing pointers to Package objects
// these pinters can also point to child classes like TwoDayPackage or OvernightPackage.
void print_Costs(const std::vector<Package*>& packages) {
    for (const auto& pkg : packages) {
        std::cout << "Sender: " << pkg->getSenderName()
                  << " | Address: " << pkg->getSenderAddress() << std::endl;

        std::cout << "Receiver: " << pkg->getReceiverName()
                  << " | Address: " << pkg->getReceiverAddress() << std::endl;

        std::cout << "Weight: " << pkg->getWeight() << " kg" << std::endl;

        std::cout << "Shipping cost: "
                  << pkg->calculate_Cost()
                  << " euro" << std::endl;

        std::cout << "--------------------------" << std::endl;
    }
}

int main() {
    // Creates vector storing pointers to Package objects
    std::vector<Package*> packages;

    // Adds a TwoDayPackage object to the vector
    packages.push_back(new TwoDayPackage(
        "Alice", "Street 1",
        "Bob", "Street 2",
        2));

    // Adds an OvernightPackage object to the vector
    packages.push_back(new OvernightPackage(
        "Charlie", "Street 3",
        "Dave", "Street 4",
        0.0));

    packages.push_back(new TwoDayPackage(
        "Eva", "Street 5",
        "Frank", "Street 6",
        5.0));

    packages.push_back(new TwoDayPackage(
        "Eva", "Street 5",
        "Frank", "Street 6",
        5.0));

    // Calls function to print all package details and shipping costs
    print_Costs(packages);

    //Calls destructors for each package created using "new"
    for (auto p : packages) {
        delete p;
    }

    return 0;
} 