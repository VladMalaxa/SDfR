//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of an edgecase of the main function for assignment 3.1, where
// all lists are left empty
//==============================================================

#include <iostream>
#include <string>
#include "List.h"
#include "ListNode.h"
using namespace std;

void fillList(const std::string& str, List<char>& list) {       //Fill a list by inserting characters of a string from the back
    for (char c : str) {
        list.insertAtBack(c);
    }
}

int main() {    
    List<char> list1;
    List<char> list2;

    List<char> list3;
    List<char> list4;

    fillList("", list1);
    fillList("", list2);
    fillList("", list3);
    fillList("", list4);

    list2.concatenate(list3);
    list2.concatenate(list4);

    list1.print();
    list2.print();
    return 0;
}
