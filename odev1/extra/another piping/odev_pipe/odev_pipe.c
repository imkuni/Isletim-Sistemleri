#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
/* max  receiving buffer size; Note: no check or enforcement is made on this value*/
#define BUF_SIZE 256

/*
    <0 fail 
    =0 child
    >0 parent
 */

int main(int argc, char** argv) {
    int pfd1[2];
    int pfd2[2];
    ssize_t numRead = -1;
    const char* messageOne = "Hello from child ONE.\n";
    const char* messageTwo = "Hello from child TWO.\n";
    const unsigned int commLen = strlen(messageOne) + 1;
    char buf[BUF_SIZE];

    sleep(1);
    //Pipe----------------------------------------------------------------------
    {
        if (pipe(pfd1) == -1) {
            printf("Error opening pipe 1!\n");
            exit(1);
        }

        if (pipe(pfd2) == -1) {
            printf("Error opening pipe 2!\n");
            exit(1);
        }
        printf("Piped opened with success. Forking ...\n");
    }

    sleep(2);
    // child 1------------------------------------------------------------------
    switch (fork()) {
        case -1:
            printf("Error forking child 1!\n");
            exit(1);

        case 0:
            printf("\nChild 1 executing...\n");
            /* close reading end of first pipe */
            if (close(pfd1[0]) == -1) {
                printf("Error closing reading end of pipe 1.\n");
                _exit(1);
            }
            /* close writing end of second pipe */
            if (close(pfd2[1]) == -1) {
                printf("Error closing writing end of pipe 2.\n");
                _exit(1);
            }

            /* write to pipe 1 */
            if (write(pfd1[1], messageOne, commLen) != commLen) {
                printf("Error writing to pipe 1.\n");
                _exit(1);
            }

            if (close(pfd1[1]) == -1) {
                printf("Error closing writing end of pipe 1.\n");
                _exit(1);
            }

            /* reding from pipe 2 */
            numRead = read(pfd2[0], buf, commLen);
            if (numRead == -1) {
                printf("Error reading from pipe 2.\n");
                _exit(1);
            }

            if (close(pfd2[0]) == -1) {
                printf("Error closing reding end of pipe 2.\n");
                _exit(1);
            }

            printf("Message received from child ONE: %s", buf);
            printf("Exiting child 1...\n");
            _exit(0);

        default:
            break;
    }
    
    sleep(3);
    // child 2------------------------------------------------------------------
    switch (fork()) {
        case -1:
            printf("Error forking child 2!\n");
            exit(1);
        case 0:
            printf("\nChild 2 executing...\n");
            /* close reading end of second pipe */
            if (close(pfd2[0]) == -1) {
                printf("Error closing reading end of pipe 2.\n");
                _exit(1);
            }
            /* close writing end of first pipe */
            if (close(pfd1[1]) == -1) {
                printf("Error closing writing end of pipe 1.\n");
                _exit(1);
            }

            /* read from the first pipe */
            if (read(pfd1[0], buf, commLen) == -1) {
                printf("Error reading from pipe 1.\n");
                _exit(EXIT_FAILURE);
            }

            if (close(pfd1[0]) == -1) {
                printf("Error closing reading end of pipe 1.\n");
                _exit(EXIT_FAILURE);
            }

            /* write to the second pipe */
            if (write(pfd2[1], messageTwo, commLen) != commLen) {
                printf("Error writing to the pipe.");
                _exit(EXIT_FAILURE);
            }

            if (close(pfd2[1]) == -1) {
                printf("Error closing writing end of pipe 2.");
                _exit(EXIT_FAILURE);
            }

            printf("Message received from child TWO: %s", buf);
            printf("Exiting child 2...\n");
            _exit(EXIT_SUCCESS);

        default:
            break;
    }

    sleep(4);
    //Pipe Close----------------------------------------------------------------
    {
        printf("Parent closing pipes.\n");

        if (close(pfd1[0]) == -1) {
            printf("Error closing reading end of the pipe.\n");
            exit(EXIT_FAILURE);
        }

        if (close(pfd2[1]) == -1) {
            printf("Error closing writing end of the pipe.\n");
            exit(EXIT_FAILURE);
        }

        if (close(pfd2[0]) == -1) {
            printf("Error closing reading end of the pipe.\n");
            exit(EXIT_FAILURE);
        }

        if (close(pfd1[1]) == -1) {
            printf("Error closing writing end of the pipe.\n");
            exit(EXIT_FAILURE);
        }

        printf("Parent waiting for children completion...\n");
        if (wait(NULL) == -1) {
            printf("Error waiting.\n");
            exit(EXIT_FAILURE);
        }

        if (wait(NULL) == -1) {
            printf("Error waiting.\n");
            exit(EXIT_FAILURE);
        }

        printf("Parent finishing.\n");
        exit(EXIT_SUCCESS);
    }
}


