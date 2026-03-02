//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of an edgecase the main function for assignment 3.1, where
// the function fillList is used twice on the same list. No overwriting should happen.
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

    fillList("singlylinkedlist", list1);
    fillList("abcdefgh", list2);

    fillList("TESTINGTESTING", list2);      //Using fillList twice on the same list.
    list2.print();                          // Print to check results are as expected. Expected "a b c d e f g h T E S T I N G T E S T I N G" (No overwriting)

    fillList("hijklmnop", list3);
    fillList("qrstuvw", list4);

    list2.concatenate(list3);
    list2.concatenate(list4);

    list1.print();
    list2.print();
    return 0;
}
