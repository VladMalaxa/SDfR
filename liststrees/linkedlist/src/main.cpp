//==============================================================
// Filename    : 
// Authors     : 
// Group       :
// License     :  N.A. or opensource license like LGPL
// Description : 
//==============================================================
#include <iostream>
#include "List.h"
#include "ListNode.h"
using namespace std;
int main() {
    // Create two lists (automatic storage duration)
    List<int> list1;
    List<int> list2;

    // Populate list1
    list1.insertAtBack(1);
    list1.insertAtBack(2);
    list1.insertAtBack(3);

    // Populate list2
    list2.insertAtFront(10);
    list2.insertAtFront(20);
    list2.insertAtFront(30);

    // Display both lists
    std::cout << "list1 contents:\n";
    list1.print();

    std::cout << "list2 contents:\n";
    list2.print();

    std::cout << "End of main() reached. Destructors will now run.\n\n";

    // When main ends, destructors are called automatically.
    // Destruction order for local variables is reverse of creation:
    // list2 will be destroyed first, then list1.
    return 0;
}