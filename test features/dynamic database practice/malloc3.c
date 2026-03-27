#include <stdio.h>
#include <stdlib.h>

// 1. The Upgraded Struct (The Node)
typedef struct Node {
    int id;
    int grade;
    struct Node *next; // THE ROPE: Points to the next backpack
} StudentNode;

int main(void) {
    // 2. Malloc three separate nodes
    StudentNode *node1 = (StudentNode *)malloc(sizeof(StudentNode));
    StudentNode *node2 = (StudentNode *)malloc(sizeof(StudentNode));
    StudentNode *node3 = (StudentNode *)malloc(sizeof(StudentNode));

    // 3. Assign their IDs and Grades
    node1->id = 123;
    node1->grade = 95;

    node2->id = 246;
    node2->grade = 96;

    node3->id = 4812;
    node3->grade = 97;


    // 4. LINK THEM TOGETHER
    node1->next = node2;
    node2->next = node3;
    node3->next = NULL;
    // Make node1 point to node2, node2 point to node3, and node3 point to NULL


    // 5. THE TRAVERSAL (Walk the line)
    StudentNode *walker = node1; // Start at the beginning
    
    printf("Walking the Linked List:\n");
    // Write a while loop here that prints the walker's ID and Grade, 
    // then moves the walker to the next node.
    while (walker != NULL)
    {
        printf("Entry Node: %p\nNext Entry Node: %p\nStudent ID: %d\nStudent Grade: %d\n\n",walker, walker->next, walker->id, walker->grade);
        walker = walker->next;
    }

    // 6. Free the memory
    free(node1);
    free(node2);
    free(node3);
    return 0;
}