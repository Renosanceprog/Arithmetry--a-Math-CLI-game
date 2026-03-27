#include <stdio.h>
#include <stdlib.h> // Required for memory management!

// 1. Define your struct here
typedef struct {
    int id;
    int grade;
} Student;

int main(void) {
    Student *myPointer = (Student *) malloc(sizeof(Student));
    
    myPointer->id = 8021;
    myPointer->grade = 96;
    
    printf("Student ID: %d\nStudent Grade: %d", myPointer->id, myPointer->grade);
    
    free(myPointer);
    
    return 0;
}