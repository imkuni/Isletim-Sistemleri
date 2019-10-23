//
//  fexec.c
//  
//
//  Created by acar on 4.10.2019.
//

#include <stdio.h>
#include <unistd.h>
int main()
{
    char * argv[2]={"l",NULL};
    
    int pid=fork();
    if(pid==0)
    {
        execvp("ls",argv);
        
    }
    
    wait(NULL);
    
    printf("Finished executing the parent process- the child wont get the result\n");
    
    return 0;
    
}
