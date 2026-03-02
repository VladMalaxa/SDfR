//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of an edgecase the main function for assignment 3.1, where
// list2 is concatenated with itself. The expected result is an empty list, as 
// after using a list for concatenation, it gets wiped (it's turned into an empty list).
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
    fillList("abcdefg", list2);
    fillList("hijklmnop", list3);
    fillList("qrstuvw", list4);

    list2.concatenate(list2);           //Try concatenating list2 with itself
    list2.print();                      //Expected result is an empty list
    return 0;
}
