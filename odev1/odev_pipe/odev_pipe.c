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
char* first_reading();
char* first_reading() {
    FILE *fp;
    char buff[255];
    fp = fopen("OperatingSystemNotes.txt", "r");

    char string[255];
    memset(string, 0, sizeof (string));

    while (!feof(fp)) {
        fscanf(fp, "%s", buff);
        strcat(string, "\n");
        strcat(string, buff);

        memset(buff, 0, sizeof (buff));
        printf(" ");
    }
    const char* messageOne_ = (char *) malloc(strlen(string) + 1);
    strcpy(messageOne_, string);

    //    printf("%s", messageOne_);
    //    for(int i=0; i<strlen(string); i++){
    //        printf("%c",string[i]);
    //    }
    fclose(fp);
    return messageOne_;
}
void second_writing(char buff[]);
void second_writing(char buff[]) {
    FILE *fp;
    fp = fopen("OperatingSystemNotes2.txt", "w+");

    char string[255];
    memset(string, 0, sizeof (string));

    size_t len = strlen(buff);
    buff[0] = NULL;
    char c = ' ';
    for (int i = 1; i < len; i++) {
        c = buff[i];
        if (!c == NULL) {
            if (c == '\n') {
                fprintf(fp, "%s", "\n");
            } else {
                fprintf(fp, "%c", c);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}
unsigned long hash(unsigned char *str);
unsigned long hash(unsigned char *str) {

    //read
    FILE *fp;
    char buff[255];
    char string[255];
    memset(buff, 0, sizeof (buff));
    memset(string, 0, sizeof (string));
    fp = fopen(str, "r");

    while (!feof(fp)) {
        fscanf(fp, "%s", buff);
        strcat(string, buff);
        strcat(string, "\n");
        memset(buff, 0, sizeof (buff));
    }
    printf(" ");
    const char* messageOne_ = (char *) malloc(strlen(string) + 1);
    strcpy(messageOne_, string);
    fclose(fp);

    //convert
    unsigned long hash = 5381;
    int c;

    while (c = *messageOne_++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int main(int argc, char** argv) {
    //    unsigned long hash1 = hash("OperatingSystemNotes.txt");   
    //    unsigned long hash2 = hash("OperatingSystemNotes2.txt"); 
    //    printf("\nhas 1 is %lu",hash1);
    //    printf("\nhas 1 is %lu",hash1);


    int fd[2], nbytes;
    pid_t childpid1, childpid2;
    char* string = first_reading();
    char readBuffer[80];

    pipe(fd);
    childpid1 = fork();
    if (childpid1 == -1) {
        printf("Fork error !\n");
    }
    if (childpid1 == 0) {
        sleep(1);
        /*Child process is executing*/
        close(fd[0]);
        printf("1 Child is writing to the pipe \n");
        printf("child[1] --> pid = %d and ppid = %d\n",
                getpid(), getppid());
        write(fd[1], string, (strlen(string) + 1));
        unsigned long hash1 = hash("OperatingSystemNotes.txt");
        printf("OperatingSystemNotes hash is %lu (pid=%d)\n"
                , hash1, getpid());
        //exit(0);
    } else {
        childpid2 = fork();
        if (childpid2 == 0) {
            sleep(2);
            /*Child process is executing*/
            close(fd[1]);
            printf("2 Child is reading from the pipe \n");
            printf("child[2] --> pid = %d and ppid = %d\n",
                    getpid(), getppid());
            //write readBuffer to OperatingSystemNotes2
            nbytes = read(fd[0], readBuffer, sizeof (readBuffer));
            printf("Received String %s\n", readBuffer);
            //write to second file
            second_writing(readBuffer);
            unsigned long hash2 = hash("OperatingSystemNotes.txt");
            printf("OperatingSystemNotes2 hash is %lu (pid=%d)\n"
                    , hash2, getpid());
        } else {
            sleep(3);
            printf("parent --> pid = %d\n", getpid());
        }
    }
    return 0;
}

