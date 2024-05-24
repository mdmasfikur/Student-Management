#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_SUBJECT_NAME_LENGTH 20
#define MAX_ID_LENGTH 10

typedef struct Subject {
    char name[MAX_SUBJECT_NAME_LENGTH];
    float grade;
    struct Subject *next;
} Subject;

typedef struct Student {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    Subject *subjects;
    struct Student *next;
} Student;

void addStudent(Student **head);
void displayStudents(const Student *head);
Student* searchStudent(const Student *head, const char *id);
void updateStudent(Student *head, const char *id);
void deleteStudent(Student **head, const char *id);
void freeList(Student *head);
float calculateOverallGrade(const Subject *subjects);

void addStudent(Student **head) {
    Student *newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed.\n");
        return;
    }
    printf("Enter student ID: ");
    scanf("%s", &newStudent->id);
    getchar(); // Consume the newline character
    printf("Enter name: ");
    scanf("%[^\n]", &newStudent->name);

    newStudent->subjects = NULL;
    Subject *lastSubject = NULL;

    char choice;
    do {
        Subject *newSubject = (Subject*)malloc(sizeof(Subject));
        if (!newSubject) {
            printf("Memory allocation failed.\n");
            return;
        }
        printf("Enter subject name: ");
        scanf(" %[^\n]", newSubject->name);
        printf("Enter grade for %s: ", newSubject->name);
        scanf("%f", &newSubject->grade);
        newSubject->next = NULL;

        if (lastSubject == NULL) {
            newStudent->subjects = newSubject;
        } else {
            lastSubject->next = newSubject;
        }
        lastSubject = newSubject;

        printf("Do you want to add another subject? (y/n): ");
        scanf(" %c", &choice);
        getchar(); // Consume the newline character
    } while (choice == 'y' || choice == 'Y');

    newStudent->next = *head;
    *head = newStudent;
}

float calculateOverallGrade(const Subject *subjects) {
    float total = 0;
    int count = 0;
    const Subject *current = subjects;
    while (current) {
        total += current->grade;
        count++;
        current = current->next;
    }
    return count == 0 ? 0 : total / count;
}

void displayStudents(const Student *head) {
    if (!head) {
        printf("No students to display.\n");
        return;
    }
    const Student *current = head;
    while (current) {
        printf("ID: %s\n", current->id);
        printf("Name: %s\n", current->name);
        const Subject *subject = current->subjects;
        while (subject) {
            printf("Subject: %s, Grade: %.2f\n", subject->name, subject->grade);
            subject = subject->next;
        }
        printf("Overall Grade: %.2f\n\n", calculateOverallGrade(current->subjects));
        current = current->next;
    }
}

Student* searchStudent(const Student *head, const char *id) {
    const Student *current = head;
    while (current) {
        if (strcmp(current->id, id) == 0) {
            return (Student*)current;
        }
        current = current->next;
    }
    return NULL;
}

void updateStudent(Student *head, const char *id) {
    Student *student = searchStudent(head, id);
    if (!student) {
        printf("Student with ID %s not found.\n", id);
        return;
    }
    int choice;
    printf("Update Student Information\n");
    printf("1. Update name\n");
    printf("2. Update subjects and grades\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume the newline character

    if (choice == 1) {
        printf("Enter new name: ");
        scanf(" %[^\n]", &student->name);
    } else if (choice == 2) {
        Subject *currentSubject = student->subjects;
        Subject *lastSubject = NULL;

        // Free the current list of subjects
        while (currentSubject) {
            Subject *temp = currentSubject;
            currentSubject = currentSubject->next;
            free(temp);
        }
        student->subjects = NULL;

        char subChoice;
        do {
            Subject *newSubject = (Subject*)malloc(sizeof(Subject));
            if (!newSubject) {
                printf("Memory allocation failed.\n");
                return;
            }
            printf("Enter new subject name: ");
            scanf(" %[^\n]", newSubject->name);
            printf("Enter grade for %s: ", newSubject->name);
            scanf("%f", &newSubject->grade);
            newSubject->next = NULL;

            if (lastSubject == NULL) {
                student->subjects = newSubject;
            } else {
                lastSubject->next = newSubject;
            }
            lastSubject = newSubject;

            printf("Do you want to add another subject? (y/n): ");
            scanf(" %c", &subChoice);
            getchar(); // Consume the newline character
        } while (subChoice == 'y' || subChoice == 'Y');
    } else {
        printf("Invalid choice.\n");
    }
}

void deleteStudent(Student **head, const char *id) {
    Student *current = *head;
    Student *previous = NULL;
    while (current && strcmp(current->id, id) != 0) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        printf("Student with ID %s not found.\n", id);
        return;
    }
    if (previous) {
        previous->next = current->next;
    } else {
        *head = current->next;
    }

    Subject *currentSubject = current->subjects;
    while (currentSubject) {
        Subject *temp = currentSubject;
        currentSubject = currentSubject->next;
        free(temp);
    }

    free(current);
}

void freeList(Student *head) {
    Student *current;
    while (head) {
        current = head;
        head = head->next;

        Subject *currentSubject = current->subjects;
        while (currentSubject) {
            Subject *temp = currentSubject;
            currentSubject = currentSubject->next;
            free(temp);
        }

        free(current);
    }
}

int main() {
    Student *head = NULL;
    int choice;
    char id[MAX_ID_LENGTH];

    while (1) {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch (choice) {
            case 1:
                addStudent(&head);
                break;
            case 2:
                displayStudents(head);
                break;
            case 3:
                printf("Enter student ID to search: ");
                scanf(" %[^\n]", id);
                getchar(); // Consume the newline character
                Student *student = searchStudent(head, id);
                if (student) {
                    printf("Student found: %s\n", student->name);
                } else {
                    printf("Student not found.\n");
                }
                break;
            case 4:
                printf("Enter student ID to update: ");
                scanf(" %[^\n]", id);
                getchar(); // Consume the newline character
                updateStudent(head, id);
                break;
            case 5:
                printf("Enter student ID to delete: ");
                scanf(" %[^\n]", id);
                getchar(); // Consume the newline character
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

