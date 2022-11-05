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

int main()
{
    FILE *fp;
    fp = fopen("marks.txt", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    // genrate the marks randomly
    struct student_marks marks;
    for (int i = 0; i < 100; i++)
    {
        sprintf(marks.student_index, "EG/%04d/%04d", i, i);
        marks.assgnmt01_marks = (float)(rand() % 100);
        marks.assgnmt02_marks = (float)(rand() % 100);
        marks.project_marks = (float)(rand() % 100);
        marks.finalExam_marks = (float)(rand() % 100);

        fwrite(&marks, sizeof(struct student_marks), 1, fp);
    }
}