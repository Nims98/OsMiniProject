#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// genereat the marks randomly
// but should have the abulity to insert delete update manully

struct student_marks
{
    char student_index[12]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
};

int main(int argc, char *argv[])
{
    FILE *fp;
    fp = fopen("marks.txt", "r+");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    // genrate the marks randomly
    if (argc == 1)
    {
        /* code */

        struct student_marks marks;
        for (int i = 0; i < 100; i++)
        {
            sprintf(marks.student_index, "EG/%04d/%04d", i, i);
            marks.assgnmt01_marks = (float)(rand() % 16);
            marks.assgnmt02_marks = (float)(rand() % 16);
            marks.project_marks = (float)(rand() % 21);
            marks.finalExam_marks = (float)(rand() % 51);
            printf("%d\n", (rand() % 16));
            fwrite(&marks, sizeof(struct student_marks), 1, fp);
        }
    }
    else if (argc == 2 && strcmp(argv[1], "-r") == 0)
    {

        struct student_marks marks;
        printf("Student Index\tAssignment 01\tAssignment 02\tProject\t\tFinal Exam\n");
        while (fread(&marks, sizeof(struct student_marks), 1, fp) == 1)
        {
            printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", marks.student_index, marks.assgnmt01_marks, marks.assgnmt02_marks, marks.project_marks, marks.finalExam_marks);
        }
    }
    else if (argc == 7 && strcmp(argv[1], "-i") == 0)
    {
        struct student_marks marks;
        strcpy(marks.student_index, argv[2]);
        marks.assgnmt01_marks = atof(argv[3]);
        marks.assgnmt02_marks = atof(argv[4]);
        marks.project_marks = atof(argv[5]);
        marks.finalExam_marks = atof(argv[6]);
        fwrite(&marks, sizeof(struct student_marks), 1, fp);
    }

    fclose(fp);
    return 0;
}