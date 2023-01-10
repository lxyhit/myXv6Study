#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int p[35][2];
    close(0);
    //create pipeline
    pipe(p[34]);
    for(int i = 35; i>=2 ;i--){
        pipe(p[i-2]);
        if(fork()==0){
            int tag;
            close(p[i-1][0]);
            close(p[i-2][1]);
            while(read(p[i-2][0],&tag,4)!=0){
                if(tag!=i&&tag%i==0){
                    continue;
                }
                else if(tag!=i && tag%i!=0){
                    write(p[i-1][1],&tag,4);
                }else if(tag == i){
                    fprintf(1,"prime %d\n",tag);
                }
            }
            close(p[i-1][1]);
            close(p[i-2][0]);
            exit(0);
        }
        else{
            close(p[i-1][0]);
            close(p[i-1][1]);
        }
    }
    close(p[0][0]);
    for(int i = 2;i<=35;i++){
        int a = i;
        write(p[0][1],&a,4);
    }
    close(p[0][1]);
    for(int i = 2;i<=35;i++){
        wait(0);
    }
    exit(0);
}