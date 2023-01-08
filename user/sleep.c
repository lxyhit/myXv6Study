#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int seconds;
    if(argc == 1){
        write(1,"No argument!\n",13);
    }else if(argc > 2){
        write(1,"Argument error!\n",16);
    }
    else{
        seconds = atoi(argv[1]);
        sleep(seconds);
    }
    exit(0);
}