//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the main function for assignment 3.1, where we create 
// some empty char lists, fill them with strings and then concatenate them to finally
// print them.
//==============================================================

#include <iostream>
#include <string>
#include "List.h"
#include "ListNode.h"
using namespace std;

void fillList(const std::string& str, List<char>& list) {       //Fill list by inserting characters of string from the back.
    for (char c : str) {
        list.insertAtBack(c);
    }
}

int main() {    
    List<char> list1;                           //Initialize 4 empty lists
    List<char> list2;

    List<char> list3;       
    List<char> list4;

    fillList("singlylinkedlist", list1);        //Fill each of the empty lists with the characters of the specified string.
    fillList("abcdefg", list2);
    fillList("hijklmnop", list3);
    fillList("qrstuvw", list4);

    list2.concatenate(list3);                   //Concatenate list 3 into list 2
    list2.concatenate(list4);                   //Concatenate list 4 into list 2

    list1.print();                              //Print lists 1 and 2
    list2.print();
    return 0;
}
