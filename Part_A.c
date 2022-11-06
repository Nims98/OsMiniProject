#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
 * @param[in] argc
 * @param[in] argv
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
        {
            sprintf(marks.student_index, "EG/%04d/%04d", i, i);
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
    }
    // * ---------------------------------------------------------------------------------------------------------}
    // * ----------------------------------- Insert new records to the file { ------------------------------------
    else if (argc == 7 && strcmp(argv[1], "-i") == 0)
    {
        FILE *fp;
        fp = fopen("marks.txt", "a");

        // * Error handling for file opening {--------------
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        student_marks marks;
        strcpy(marks.student_index, argv[2]);
        marks.assgnmt01_marks = atof(argv[3]);
        marks.assgnmt02_marks = atof(argv[4]);
        marks.project_marks = atof(argv[5]);
        marks.finalExam_marks = atof(argv[6]);
        fseek(fp, 0, SEEK_END);
        if (fwrite(&marks, sizeof(student_marks), 1, fp) != 1) // fwrite with error handling
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        printf("Marks for %s inserted successfully\n", marks.student_index);
        fclose(fp);
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
        {
            if (strcmp(marks.student_index, argv[2]) == 0)
            {
                found = 1;
                if (strcmp(argv[3], "-") != 0)
                    marks.assgnmt01_marks = atof(argv[3]);
                if (strcmp(argv[4], "-") != 0)
                    marks.assgnmt02_marks = atof(argv[4]);
                if (strcmp(argv[5], "-") != 0)
                    marks.project_marks = atof(argv[5]);
                if (strcmp(argv[6], "-") != 0)
                    marks.finalExam_marks = atof(argv[6]);
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
        {
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
        remove("marks.txt");
        rename("temp.txt", "marks.txt");
        printf("Marks for %s deleted successfully\n", argv[2]);
        fclose(fp);
    }
    // * -------------------------------------------------------------------------------------------------------------}
    else
    {
        printf("Invalid arguments\n");
    }

    return 0;
}