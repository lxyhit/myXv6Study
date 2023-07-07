#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//负责提取传入文件全名(及./a/b)中的文件名(b)
char*
extname(char* path){
  //static是为了便于字符串的产生
  static char buf[DIRSIZ+1];
  char *p;
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return file name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', 1);
  return buf;
}

void
find(char *path,char* name){
  //该函数负责寻找并打印出该目录下所有与传入文件名相同的文件目录
  char buf[512],*p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(extname(path),name)==0){
      printf("%s\n",path);
    } 
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);  
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // printf("%s %d\n",de.name,strlen(de.name));
      memmove(p, de.name, strlen(de.name));
      p[strlen(de.name)] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if(strcmp(extname(buf),".")!=0 && strcmp(extname(buf),"..")!=0){
        find(buf,name);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc,char *argv[]){
  if(argc != 3){
      //当传入的指令数量不等于2时输出参数错误
      printf("Arguments error,input size has to equal to two\n");
  }
  else{
      //否则调用find函数进行查找
      find(argv[1],argv[2]);
  }
  exit(0);
}