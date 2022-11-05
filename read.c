#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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
    fp = fopen("marks.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    struct student_marks marks;
    printf("Student Index\tAssignment 01\tAssignment 02\tProject\t\tFinal Exam\n");
    while (fread(&marks, sizeof(struct student_marks), 1, fp) == 1)
    {
        printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", marks.student_index, marks.assgnmt01_marks, marks.assgnmt02_marks, marks.project_marks, marks.finalExam_marks);
    }
}
