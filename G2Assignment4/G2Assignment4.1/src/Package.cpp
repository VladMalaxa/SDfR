#include "Package.h"

Package::Package(std::string sName, std::string sAddr,
                 std::string rName, std::string rAddr,
                 double w)
    : senderName(sName),
      senderAddress(sAddr),
      receiverName(rName),
      receiverAddress(rAddr),
      weight(w) {}

double Package::calculate_Cost() const {
    return weight * 2.50 + 5.0;
}