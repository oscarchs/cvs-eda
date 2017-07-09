#include <dirent.h>
#include <stdio.h>
#include <iostream>

int main(){
DIR * dir;
struct dirent * file;

dir = opendir("/home/oscar/Documentos/EDA/CVS");
if (dir != NULL){
   while (file = readdir(dir)){
    //  printf("Filename: %s",epdf->d_name);
       std::cout << file->d_name << std::endl;
   }
}

  return 0;
}