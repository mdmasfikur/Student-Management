#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

typedef struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    float grade;
    struct Student *next;
} Student;

void addStudent(Student **head, int *count);
void displayStudents(const Student *head);
Student* searchStudent(const Student *head, int id);
void updateStudent(Student *head, int id);
void deleteStudent(Student **head, int id);
void freeList(Student *head);

void addStudent(Student **head, int *count) {
    Student *newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed.\n");
        return;
    }
    newStudent->id = ++(*count);
    printf("Enter name: ");
    scanf(" %[^\n]s", newStudent->name);
    printf("Enter age: ");
    scanf("%d", &newStudent->age);
    printf("Enter grade: ");
    scanf("%f", &newStudent->grade);
    newStudent->next = *head;
    *head = newStudent;
}

void displayStudents(const Student *head) {
    if (!head) {
        printf("No students to display.\n");
        return;
    }
    const Student *current = head;
    while (current) {
        printf("ID: %d\n", current->id);
        printf("Name: %s\n", current->name);
        printf("Age: %d\n", current->age);
        printf("Grade: %.2f\n\n", current->grade);
        current = current->next;
    }
}

Student* searchStudent(const Student *head, int id) {
    const Student *current = head;
    while (current) {
        if (current->id == id) {
            return (Student*)current;
        }
        current = current->next;
    }
    return NULL;
}

void updateStudent(Student *head, int id) {
    Student *student = searchStudent(head, id);
    if (!student) {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    printf("Enter new name: ");
    scanf(" %[^\n]s", student->name);
    printf("Enter new age: ");
    scanf("%d", &student->age);
    printf("Enter new grade: ");
    scanf("%f", &student->grade);
}

void deleteStudent(Student **head, int id) {
    Student *current = *head;
    Student *previous = NULL;
    while (current && current->id != id) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    if (previous) {
        previous->next = current->next;
    } else {
        *head = current->next;
    }
    free(current);
}

void freeList(Student *head) {
    Student *current;
    while (head) {
        current = head;
        head = head->next;
        free(current);
    }
}

int main() {
    Student *head = NULL;
    int studentCount = 0;
    int choice, id;

    while (1) {
        printf("Student Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(&head, &studentCount);
                break;
            case 2:
                displayStudents(head);
                break;
            case 3:
                printf("Enter student ID to search: ");
                scanf("%d", &id);
                Student *student = searchStudent(head, id);
                if (student) {
                    printf("Student found: %s\n", student->name);
                } else {
                    printf("Student not found.\n");
                }
                break;
            case 4:
                printf("Enter student ID to update: ");
                scanf("%d", &id);
                updateStudent(head, id);
                break;
            case 5:
                printf("Enter student ID to delete: ");
                scanf("%d", &id);
                deleteStudent(&head, id);
                break;
            case 6:
                freeList(head);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
