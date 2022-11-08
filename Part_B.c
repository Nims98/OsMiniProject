#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
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

int main()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        // error handling wtih fprintf
        fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    else if (pid == 0)
    { //  Child Procees 1

        printf("Child Process 1 --- %d --- %d\n", getpid(), getppid());
    }
    else
    {
        // Parent Process
        pid_t pid = fork();
        if (pid == -1)
        {
            // error handling wtih fprintf
            fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        else if (pid == 0)
        {
            // Child Procees 2
            printf("Child Process 2 --- %d --- %d\n", getpid(), getppid());
        }
        else
        {
            // Parent Process
            pid_t pid = fork();
            if (pid == -1)
            {
                // error handling wtih fprintf
                fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }

            else if (pid == 0)
            {
                // Child Procees 3
                printf("Child Process 3 --- %d --- %d\n", getpid(), getppid());
                pid_t cpid = fork();
                if (pid == -1)
                {
                    // error handling wtih fprintf
                    fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
                else if (cpid == 0)
                {
                    // Child Procees 3.1
                    printf("Child Process 3.1 --- %d --- %d\n", getpid(), getppid());
                }
                waitpid(cpid, NULL, 0);
            }

            else
            {

                waitpid(pid, NULL, 0);
                printf("Parent Process --- %d\n", getpid());
            }
        }
    }
}
