// * Sabaragamuwa S.B.N.M. - EG/2018/3443
// * Link to GitHub   :   https://github.com/Nims98/OsMiniProject

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

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

    int noOfRecords = 0;
    while (fread(&marks, sizeof(student_marks), 1, fp) == 1)
    {
        fmarks[noOfRecords] = marks.finalExam_marks;
        noOfRecords++;
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
    {
        //  Child Procees C1 -------------------------------------------------------------------{

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

        for (int j = 0; j < noOfRecords; j++)
        {
            if (c1marks[j] < min)
            {
                min = c1marks[j];
            }
        }
        printf("The minimum mark is %.2f\n", min);
        // printf("Child Process 1 --- %d --- %d : marks = %.2f\n", getpid(), getppid(), c1marks[1]);

        if (shmdt((void *)c1marks) == -1)
        {
            perror("shmdt parent error");
            printf("errno: %d\n", errno);
            exit(1);
        }
        // --------------------------------------------------------------------------------------}
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
            // Child Procees C2 ------------------------------------------------------------------------------{

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

            for (int j = 0; j < noOfRecords; j++)
            {
                if (c2marks[j] > max)
                {
                    max = c2marks[j];
                }
            }
            printf("The maximum mark is %.2f\n", max);
            // printf("Child Process 2 --- %d --- %d : marks = %.2f\n", getpid(), getppid(), c2marks[0]);

            if (shmdt((void *)c2marks) == -1)
            {
                perror("shmdt parent error");
                printf("errno: %d\n", errno);
                exit(1);
            }

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
                // Child Procees CC1    (Grand Child of P1) -----------------------------------{

                float *c21marks;
                c21marks = (float *)shmat(SMID, NULL, SHM_R | SHM_W);

                // * Error handling for shared memory attach {--------------
                if (c21marks == (float *)-1)
                {
                    fprintf(stderr, "ERROR (%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                    exit(1);
                }
                // * ---------------------------------------------- }

                // Calculate the number below 17.5
                int count = 0;
                for (int j = 0; j < noOfRecords; j++)
                {
                    if (c21marks[j] < 17.5)
                    {
                        count++;
                    }
                }
                printf("The number of students who got below 17.5 is %d\n", count);
                // printf("Child Process 2.1 --- %d --- %d : marks = %.2f\n", getpid(), getppid(), c21marks[0]);

                if (shmdt((void *)c21marks) == -1)
                {
                    perror("shmdt parent error");
                    printf("errno: %d\n", errno);
                    exit(1);
                }
                // ------------------------------------------------------------------------}
            }
            else
            { // Wait for child process CC1 to finish
                if (waitpid(cpid, NULL, 0) == -1)
                {
                    fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                }
            }

            // --------------------------------------------------------------------------------------}
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
            // * -------------------------------------- }

            else if (pid == 0)
            {
                // Child Procees 3 -------------------------------------------------------------------{

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
                for (int j = 0; j < noOfRecords; j++)
                {
                    sum += c3marks[j];
                }

                float avg = sum / noOfRecords;
                printf("The average mark is %.2f\n", avg);
                // printf("Child Process 3 --- %d --- %d : marks = %.2f\n", getpid(), getppid(), c3marks[0]);

                if (shmdt((void *)c3marks) == -1)
                {
                    perror("shmdt parent error");
                    printf("errno: %d\n", errno);
                    exit(1);
                }

                // --------------------------------------------------------------------------------------}
            }

            else
            { // Wait for child process to finish
                if (waitpid(pid, NULL, 0) == -1)
                {
                    fprintf(stderr, "Error(%s:%d)\n -- %s", __FILE__, __LINE__, strerror(errno));
                }
                // printf("Parent Process --- %d\n", getpid());
                // Detach the shared memory
                if (shmdt((void *)fmarks) == -1)
                {
                    perror("shmdt parent error");
                    printf("errno: %d\n", errno);
                    exit(1);
                }
                // Remove the shared memory
                if (shmctl(SMID, IPC_RMID, NULL) == -1)
                {
                    perror("shmctl parent error");
                    printf("errno: %d\n", errno);
                    exit(1);
                }
            }
        }
    }
}
