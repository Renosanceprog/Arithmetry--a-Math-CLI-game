#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    int grade;
    struct Node *next;
} StudentNode;

// We only track the start of the chain!
StudentNode *head = NULL;

// The Dynamic Add Function
void add_student(int new_id, int new_grade) {
    
    // 1. CREATE THE NEW BACKPACK
    // malloc a new StudentNode pointer called 'new_node'
    // Assign new_id to its id, and new_grade to its grade
    // Set its next pointer to NULL (because they are the newest guy at the end)
    StudentNode *new_node = (StudentNode *) malloc(sizeof(StudentNode));
    new_node->id = new_id;
    new_node->grade = new_grade;
    new_node->next = NULL;
    
    // 2. CHECK THE EDGE CASE (Is the list completely empty?)
    // Write an if statement: if head is NULL, set head equal to new_node, then return.
    if (head == NULL)
    {
        head = new_node;
        return;
    }
    
    // 3. THE TRAVERSAL (Find the end of the line)
    // Create a walker pointer and start it at head.
    // Write a while loop: while walker->next is NOT NULL...
    //     Move the walker forward (walker = walker->next)
    StudentNode *walker = head;
    while (walker->next != NULL)
    {
        walker = walker->next;
    }
    
    // 4. THE LINK
    // Once the while loop breaks, walker is currently standing on the very last node.
    // Tell the walker's 'next' to point to your 'new_node'.
    walker->next = new_node;
    return;
}

// A helper function to print the list (I wrote this one for you!)
void print_list() {
    StudentNode *walker = head;
    printf("\n--- DYNAMIC STUDENT LIST ---\n");
    while (walker != NULL) {
        printf("ID: %d | Grade: %d\n", walker->id, walker->grade);
        walker = walker->next;
    }
    printf("----------------------------\n");
}

int main(void) {
    // Let's test your function by pumping 5 students into the infinite chain!
    add_student(101, 85);
    add_student(102, 90);
    add_student(103, 92);
    add_student(104, 88);
    add_student(105, 95);

    // If your add_student function works, this will print all 5 perfectly.
    print_list();
    system("pause");
    return 0;
}