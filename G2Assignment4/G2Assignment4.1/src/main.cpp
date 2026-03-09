#include <iostream>
#include <vector>

#include "Package.h"
#include "TwoDayPackage.h"
#include "OvernightPackage.h"

void print_Costs(const std::vector<Package*>& packages) {
    for (const auto& pkg : packages) {
        std::cout << "Shipping cost: "
                  << pkg->calculate_Cost()
                  << " euro" << std::endl;
    }
}

int main() {
    std::vector<Package*> packages;

    packages.push_back(new TwoDayPackage(
        "Alice", "Street 1",
        "Bob", "Street 2",
        0.0));

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

    print_Costs(packages);

    for (auto p : packages) {
        delete p;
    }

    return 0;
}