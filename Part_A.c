#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>

// Sabaragamuwa S.B.N.M. - EG/2018/3443
// * Link to GitHub   :   https://github.com/Nims98/OsMiniProject

/*
 * A structure to hold the student marks
 */
typedef struct
{
    char student_index[12]; /** Index No of the student EG/XXXX/XXXX */
    float assgnmt01_marks;  /** 15% for Assignment 1 */
    float assgnmt02_marks;  /** 15% for Assignment 2 */
    float project_marks;    /** 20% for Project */
    float finalExam_marks;  /** 50% for Final Exam  */

} student_marks;

/**
 * int main
 * @param[in] argc  : number of arguments
 * @param[in] argv  : array of arguments
 * @return 0 on success
 */
int main(int argc, char *argv[])
{

    // * ------------------------ Genrate the marks randomly and store them in a file { ---------------------
    if (argc == 1)
    {
        FILE *fp;
        fp = fopen("marks.txt", "w");

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        student_marks marks;
        for (int i = 0; i < 100; i++)
        { // * Generate the marks randomly and store them in a file
            sprintf(marks.student_index, "EG/%04d/%04d", (int)(rand() % 2018), (int)(rand() % 3600));
            marks.assgnmt01_marks = (float)(rand() % 16);
            marks.assgnmt02_marks = (float)(rand() % 16);
            marks.project_marks = (float)(rand() % 21);
            marks.finalExam_marks = (float)(rand() % 51);

            if (fwrite(&marks, sizeof(student_marks), 1, fp) != 1) // fwrite with error handling
            {
                fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }
        }
        fclose(fp);

        printf("Successfully generated marks and stored in the file.\n\n"); // * Print the success message

        // * ------------------------ Instructions for usage{ ---------------------
        printf("To Insert new records \n\t\t  $ ./Part_A -i <INDEX> <ASSIGNMENT 1 MARKS> <ASSIGNMENT 2 MARKS> <PROJECT> <FINAL EXAM>\n");
        printf("To Read all records \n\t\t  $ ./Part_A -r\n");
        printf("To Update a particular record \n\t\t  $ ./Part_A -u <INDEX> <ASSIGNMENT 1 MARKS> <ASSIGNMENT 2 MARKS> <PROJECT> <FINAL EXAM> ");
        printf("(If you do not wish to update an certain mark keep the relevant argument as \"-\".)\n");
        printf("To Delete a particular record \n\t\t  $ ./Part_A -d <INDEX>\n");
        // * --------------------------------------------------------------------- }
        exit(0);
    }
    // * ------------------------------------------------------------------------------------------------------- }

    // * ----------------------------------- Read the recorded marks from the file { ---------------------------
    else if (argc == 2 && strcmp(argv[1], "-r") == 0)
    {
        FILE *fp;
        fp = fopen("marks.txt", "r");

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        student_marks marks;
        printf("Student Index\tAssignment 01\tAssignment 02\tProject\t\tFinal Exam\n");
        while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
        {
            printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", marks.student_index, marks.assgnmt01_marks,
                   marks.assgnmt02_marks, marks.project_marks, marks.finalExam_marks);
        }
        fclose(fp);
        exit(0);
    }
    // * ---------------------------------------------------------------------------------------------------------}

    // * ----------------------------------- Insert new records to the file { ------------------------------------
    else if (argc == 7 && strcmp(argv[1], "-i") == 0)
    {
        // * Check the validity of the index number {----------------
        regex_t regex;
        int reti;
        reti = regcomp(&regex, "EG/[0-9][0-9][0-9][0-9]/[0-9][0-9][0-9][0-9]", 0);
        if (reti)
        {
            fprintf(stderr, "Could not compile regex\n");
            exit(1);
        }
        reti = regexec(&regex, argv[2], 0, NULL, 0);
        if (reti == REG_NOMATCH)
        {
            fprintf(stderr, "Invalid Index Number\n");
            printf("Index number should be in the format EG/XXXX/XXXX\n");
            exit(1);
        }
        regfree(&regex);
        // * ------------------------------------------------------- }

        FILE *fp;
        fp = fopen("marks.txt", "a+");

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }
        student_marks marks;

        // * Check whether the index number is already in the file {----------------
        while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
        {
            if (strcmp(marks.student_index, argv[2]) == 0)
            {
                printf("Student Index already exists\n");
                printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", marks.student_index, marks.assgnmt01_marks,
                       marks.assgnmt02_marks, marks.project_marks, marks.finalExam_marks);
                exit(0);
            }
        }
        // * ---------------------------------------------------------------------- }

        strcpy(marks.student_index, argv[2]);
        (atoi(argv[3]) > 15) ? (printf("Assignment 01 marks should be less than 15\n"), exit(0)) : (marks.assgnmt01_marks = atof(argv[3]));
        (atoi(argv[4]) > 15) ? (printf("Assignment 02 marks should be less than 15\n"), exit(0)) : (marks.assgnmt02_marks = atof(argv[4]));
        (atoi(argv[5]) > 20) ? (printf("Project marks should be less than 20\n"), exit(0)) : (marks.project_marks = atof(argv[5]));
        (atoi(argv[6]) > 50) ? (printf("Final exam marks should be less than 50\n"), exit(0)) : (marks.finalExam_marks = atof(argv[6]));
        fseek(fp, 0, SEEK_END);
        if (fwrite(&marks, sizeof(student_marks), 1, fp) != 1) // fwrite with error handling
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        printf("Marks for %s inserted successfully\n", marks.student_index);
        fclose(fp);
        exit(0);
    }
    // * ------------------------------------------------------------------------------------------------------------ }

    // * ------------------------------------------------ Update marks { --------------------------------------------
    else if (argc == 7 && strcmp(argv[1], "-u") == 0)
    {
        FILE *fp;
        fp = fopen("marks.txt", "r+");

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        int found = 0;
        student_marks marks;
        while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
        { // * Search for the student index
            if (strcmp(marks.student_index, argv[2]) == 0)
            {
                found = 1;
                if (strcmp(argv[3], "-") != 0)
                    (atoi(argv[3]) > 15) ? (printf("Assignment 01 marks should be less than 15\n"), exit(0)) : (marks.assgnmt01_marks = atof(argv[3]));
                if (strcmp(argv[4], "-") != 0)
                    (atoi(argv[4]) > 15) ? (printf("Assignment 02 marks should be less than 15\n"), exit(0)) : (marks.assgnmt02_marks = atof(argv[4]));
                if (strcmp(argv[5], "-") != 0)
                    (atoi(argv[5]) > 20) ? (printf("Project marks should be less than 20\n"), exit(0)) : (marks.project_marks = atof(argv[5]));
                if (strcmp(argv[6], "-") != 0)
                    (atoi(argv[6]) > 50) ? (printf("Final exam marks should be less than 50\n"), exit(0)) : (marks.finalExam_marks = atof(argv[6]));
                fseek(fp, -sizeof(student_marks), SEEK_CUR);
                if (fwrite(&marks, sizeof(student_marks), 1, fp) != 1) // fwrite with error handling
                {
                    fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
                printf("Marks for %s updated successfully\n", marks.student_index);
                break;
            }
        }
        if (found == 0)
        {
            printf("Student index %s not found\n", argv[2]);
        }
        fclose(fp);
        exit(0);
    }
    // * -------------------------------------------------------------------------------------------------------------}

    // * ----------------------------------------------- Delete records { --------------------------------------------
    else if (argc == 3 && strcmp(argv[1], "-d") == 0)
    {
        student_marks marks;
        FILE *fp, *temp;
        fp = fopen("marks.txt", "r");
        temp = fopen("temp.txt", "w");
        int found = 0;

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        if (temp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
        { // * Search for the student index and skip the record
            if (strcmp(marks.student_index, argv[2]) != 0)
            {
                found = 1;
                if (fwrite(&marks, sizeof(student_marks), 1, temp) != 1) // fwrite with error handling
                {
                    fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
            }
        }
        if (found == 0)
        {
            printf("Student index %s not found\n", argv[2]);
        }
        fclose(temp);
        remove("marks.txt");             // * Delete the original file
        rename("temp.txt", "marks.txt"); // * Rename the temporary file
        printf("Marks for %s deleted successfully\n", argv[2]);
        fclose(fp);
        exit(0);
    }
    // * -------------------------------------------------------------------------------------------------------------}
    else
    {
        printf("Invalid arguments\n");
    }
    return 0;
}