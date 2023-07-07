#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc,char *argv[])
{
  int count = argc-1;//计算当前的包括执行程序名字的参数个数，防止参数个数超过限制
  char *argument[MAXARG+1];//从标准输入传入的每行读出的参数
  char tag,tag1;//每次读入一个字符,tag1表示前一个字符,用来分割参数中间的空格
  tag1 = ' ';
  if(argc == 1 ){
    //当没有输入参数时默认执行echo函数
    count += 1;
    argument[0] = "echo";
  }
  else{
    //否则执行函数为传入的第一个参数名
    argument[0] = argv[1];
  }
  for(int i = 1;i<=count-1;i++){
    argument[i] = argv[i+1]; 
  }
  //当时忘了要求只要额外的一个参数，因此就简单的只会额外分配一个参数的空间
  argument[count] = malloc(strlen(argv[1]) + 1);

  int initial = count;//记录了初始参数的个数
  int length = 0;//记录了当前参数的长度

  while(read(0,&tag,1)!=0){
    if(tag == '\n'){
      //当读到换行符时进行是否执行程序的判断
      tag1 = ' ';//表示下一个读到的非" "字符将被视为新的参数
      length = 0;
      if(count > MAXARG){
        printf("arguments too long!\n");
        count = initial;
      }
      else{
        if(fork()==0){
          printf("enter\n");
          argument[count] = 0;
          exec(argument[0],argument);
        }else{
          wait(0);
          count = initial;
        }
      }
    }
    else if(tag == ' '){
      tag1 = tag;
    }
    else if(tag != ' ' && tag1 ==' '){
      //此时说明进入新的参数
      length = 0;tag1 = tag;count += 1;
      argument[count-1][length] = tag;
      argument[count-1][length+1] = '\0';
    }
    else if(tag !=' ' && tag1 != ' '){
      //此时说明还在旧的参数
      length += 1;tag1 = tag;
      argument[count-1][length] = tag;
      argument[count-1][length+1] = '\0';
    }
  }
  exit(0);
}