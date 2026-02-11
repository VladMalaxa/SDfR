//=============================================================================
// Authors : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group : 2
// License : LGPL open source license
//
// Brief : Main function for the example division code
//
//=============================================================================

#include <iostream>
#include "divide.h"

int main(void) {
int a = 6;
int b = 2;
int c = divide(a, b);
std :: cout << a << "/" << b << "=" << c << std::endl;
return 0;
}