#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
int main(){
    int p1[2],p2[2];//定义father->child,child->father的两个pipeline
    pipe(p1);pipe(p2);
    int pid = 0;
    char *tag = "3";
    if(fork() == 0){
        //子进程
        close(p1[1]);
        read(p1[0],tag,1);

        if(atoi(tag)==1){
            pid = getpid();
            fprintf(1,"%d: received ping\n",pid);
            close(p1[0]);
            //向父进程发送消息
            close(p2[0]);
            write(p2[1],"0",1);
            close(p2[1]);
        }else{
            fprintf(1,"%d: received ping error\n",pid);
            close(p1[0]);
        }
    }else{
        pid = getpid();
        //父进程
        close(p1[0]);
        write(p1[1],"1",1);
        close(p1[1]);

        close(p2[1]);
        read(p2[0],tag,1);
        if(atoi(tag)==0){
            fprintf(1,"%d: received pong\n",pid);
            close(p2[0]);
            
        }else{
            fprintf(1,"%d: received pong error\n",pid);
            close(p2[0]);
        }
    }
    exit(0);
}