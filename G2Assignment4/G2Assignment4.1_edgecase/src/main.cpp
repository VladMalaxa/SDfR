//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the main function for assignment 4.1, where we take the 
// weight of the package and calculate the cost of shipping it using the calculate_Cost function. 
// We also print out the sender and receiver information, as well as the weight and cost of the package.
// We test edge cases for TwoDayPackage and OvernightPackage, including zero weight, large weight,
// empty sender/receiver names and addresses, 
// and negative weight.
//==============================================================


#include <iostream>
#include <vector>

#include "Package.h"
#include "TwoDayPackage.h"
#include "OvernightPackage.h"

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

int main() {//Testing edge cases for TwoDayPackage and OvernightPackage
    std::vector<Package*> packages;

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        2));//Standard weight

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        3.0));//Standard weight

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        0.0));//Edge case: zero weight

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        0.0));//Edge case: large weight

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        10000.0));//Edge case: large weight

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        10000.0));//Edge case: large weight
    
    packages.push_back(new TwoDayPackage(
        "", "Street 1",
        "Vlad", "Street 2",
        3.0));//Edge case: empty sender name

    packages.push_back(new OvernightPackage(
        "", "Street 1",
        "Vlad", "Street 2",
        3.0));//Edge case: large weight

    packages.push_back(new TwoDayPackage(
        "Richie", "",
        "Vlad", "Street 2",
        3.0));//Edge case: empty sender address

    packages.push_back(new OvernightPackage(
        "Richie", "",
        "Vlad", "Street 2",
        3.0));//Edge case: empty sender address

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "", "Street 2",
        3.0));//Edge case: empty receiver name

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "", "Street 2",
        3.0));//Edge case: empty receiver name

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "Vlad", "",
        3.0));//Edge case: empty receiver address

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "VLad", "",
        3.0));//Edge case: empty receiver address

    packages.push_back(new TwoDayPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        -20.0));//Edge case: negative weight

    packages.push_back(new OvernightPackage(
        "Richie", "Street 1",
        "Vlad", "Street 2",
        -20.0));//Edge case: negative weight
    for (auto p : packages) {
        delete p;
    }

    return 0;
}