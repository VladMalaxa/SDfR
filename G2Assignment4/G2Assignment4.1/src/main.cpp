#include <iostream>
#include <string>
#include <vector>

class Package {
protected:
    std::string senderName;
    std::string senderAddress;
    std::string receiverName;
    std::string receiverAddress;
    double weight;

public:
    Package(std::string sName, std::string sAddr,
            std::string rName, std::string rAddr,
            double w);

    virtual double calculateCost() const;

    virtual ~Package() {}
};

Package::Package(std::string sName, std::string sAddr,
                 std::string rName, std::string rAddr,
                 double w)
    : senderName(sName),
      senderAddress(sAddr),
      receiverName(rName),
      receiverAddress(rAddr),
      weight(w) {}

double Package::calculateCost() const {
    return weight * 2.50 + 5.0; // 2.50 euro/kg + 5 euro flat fee
}

class TwoDayPackage : public Package {
public:
    TwoDayPackage(std::string sName, std::string sAddr,
                  std::string rName, std::string rAddr,
                  double w);
};

TwoDayPackage::TwoDayPackage(std::string sName, std::string sAddr,
                             std::string rName, std::string rAddr,
                             double w)
    : Package(sName, sAddr, rName, rAddr, w) {}

class OvernightPackage : public Package {
public:
    OvernightPackage(std::string sName, std::string sAddr,
                     std::string rName, std::string rAddr,
                     double w);

    double calculateCost() const override {
        return Package::calculateCost() + (weight * weight * 1.10);
    }
};

OvernightPackage::OvernightPackage(std::string sName, std::string sAddr,
                                   std::string rName, std::string rAddr,
                                   double w)
    : Package(sName, sAddr, rName, rAddr, w) {}

void printCosts(const std::vector<Package*>& packages) {
    for (const auto& pkg : packages) {
        std::cout << "Shipping cost: "
                  << pkg->calculateCost()
                  << " euro" << std::endl;
    }
}

int main() {
    std::vector<Package*> packages;

    packages.push_back(new TwoDayPackage(
        "Alice", "Street 1",
        "Bob", "Street 2",
        3.0));

    packages.push_back(new OvernightPackage(
        "Charlie", "Street 3",
        "Dave", "Street 4",
        2.5));

    packages.push_back(new TwoDayPackage(
        "Eva", "Street 5",
        "Frank", "Street 6",
        5.0));

    printCosts(packages);

    for (auto p : packages) {
        delete p;
    }

    return 0;
}