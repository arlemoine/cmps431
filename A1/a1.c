#include <stdio.h> 
#include <unistd.h> 
#include <sys/wait.h>

int main(void) { 
int i;
int mainpid=getpid();
printf("Main process ID: %d \n", mainpid);

for (i=0; i<3; i++) {
    fork();
    printf("Iteration i=%d My process ID is %d and my parent is %d \n", i, getpid(), getppid());
    int newpid=getpid();
    if (newpid != mainpid) { // Add conditional to end process if it is not the parent process
        return 0;
    }
}

wait(NULL);   // Any process reaching this point will wait until all its children terminate. 
printf("Process %d all done \n", getpid());
return 0;
}
