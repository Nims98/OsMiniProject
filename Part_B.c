#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

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

int main()
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
    int i = 0;

    key_t key = ftok("marks.txt", 65); // ftok to generate unique key

    // * Error handling for file token {--------------
    if (key == -1)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    // * ---------------------------------------------- }

    int SMID = shmget(key, 4096, 0666 | IPC_CREAT); // shmget returns an identifier in shmid

    // * Error handling for shared memory {--------------
    if (SMID == -1)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    // * ---------------------------------------------- }

    float *fmarks;
    fmarks = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // attach to shared memory

    // * Error handling for shared memory attach {--------------
    if (fmarks == (float *)-1)
    {
        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    // * ---------------------------------------------- }

    while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
    {
        fmarks[i] = marks.finalExam_marks;
        i++;
    }
    fclose(fp);
    pid_t pid = fork();

    // * Error handling for fork {--------------
    if (pid == -1)
    {
        fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    // * ---------------------------------------------- }

    else if (pid == 0)
    { //  Child Procees 1
        float *c1marks;
        c1marks = (float *)shmat(SMID, NULL, SHM_R | SHM_W);

        // * Error handling for shared memory attach {--------------
        if (c1marks == (float *)-1)
        {
            fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }

        // Calculate the minimum
        float min = c1marks[0];
        for (int i = 0; i < 100; i++)
        {
            if (c1marks[i] < min)
            {
                min = c1marks[i];
            }
        }
        // printf("The minimum mark is %.2f\n", min);
        printf("Child Process 1 --- %d --- %d : fmarks = %.2f\n", getpid(), getppid(), c1marks[0]);
    }
    else
    {
        pid_t pid = fork();
        // * Error handling for fork {--------------
        if (pid == -1)
        {
            fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        // * ---------------------------------------------- }
        else if (pid == 0)
        {
            // Child Procees 2

            float *c2marks;
            c2marks = (float *)shmat(SMID, NULL, SHM_R | SHM_W);
            // * Error handling for shared memory attach {--------------
            if (c2marks == (float *)-1)
            {
                fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }
            // * ---------------------------------------------- }
            // Calculate the maximum
            float max = c2marks[0];
            for (int i = 0; i < 100; i++)
            {
                if (c2marks[i] > max)
                {
                    max = c2marks[i];
                }
            }
            // printf("The maximum mark is %.2f\n", max);
            printf("Child Process 2 --- %d --- %d : fmarks = %.2f\n", getpid(), getppid(), c2marks[0]);
        }
        else
        {
            pid_t pid = fork();
            // * Error handling for fork {--------------
            if (pid == -1)
            {
                fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }
            // * ---------------------------------------------- }

            else if (pid == 0)
            {
                // Child Procees 3

                float *c3marks;
                c3marks = (float *)shmat(SMID, NULL, SHM_R | SHM_W);

                // * Error handling for shared memory attach {--------------
                if (c3marks == (float *)-1)
                {
                    fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
                // * ---------------------------------------------- }

                // Calculate the average
                float sum = 0;
                for (int i = 0; i < 100; i++)
                {
                    sum += c3marks[i];
                }
                float avg = sum / 100;
                // printf("The average mark is %.2f\n", avg);
                printf("Child Process 3 --- %d --- %d : fmarks = %.2f\n", getpid(), getppid(), c3marks[0]);

                pid_t cpid = fork();
                // * Error handling for fork {--------------
                if (pid == -1)
                {
                    fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
                // * ---------------------------------------------- }
                else if (cpid == 0)
                {
                    // Child Procees 3.1
                    float *c31marks;
                    c31marks = (float *)shmat(SMID, NULL, SHM_R | SHM_W);

                    // * Error handling for shared memory attach {--------------
                    if (c31marks == (float *)-1)
                    {
                        fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                        exit(1);
                    }
                    // * ---------------------------------------------- }

                    // Calculate the number below 17.5
                    int count = 0;
                    for (int i = 0; i < 100; i++)
                    {
                        if (c31marks[i] < 17.5)
                        {
                            count++;
                        }
                    }
                    // printf("The number of students who got below 17.5 is %d\n", count);
                    printf("Child Process 3.1 --- %d --- %d : fmarks = %.2f\n", getpid(), getppid(), c31marks[0]);
                }
                else
                {
                    if (waitpid(cpid, NULL, 0) == -1)
                    {
                        fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    }
                }
            }

            else
            {
                if (waitpid(pid, NULL, 0) == -1)
                {
                    fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                }
                printf("Parent Process --- %d\n", getpid());
            }
        }
    }
}
