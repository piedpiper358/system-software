//Неименованные каналы
//Процесс запускает дочерний подпроцесс, которому подменяется стандартный ввод. В подпроцессе запускается системная утилита wc. 
//Основной процесс читает указанный через аргументы файл и передаёт в подпроцесс каждый чётный символ. 
//man fork(2), execl(2), pipe(2), dup2(3C). При использовании system(3C) или popen(3C) задание принято не будет.
#include <fcntl.h> /* open */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strlen strcpy */
#include <unistd.h> /* для getopt и read, pipe, fork */
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <errno.h>
#include <stdarg.h> /*va_arg*/
#include <sys/types.h> 
#include <sys/stat.h> 


#define BUF_SIZE 4*1024*1024


size_t ret=0;

void panic(char* str);
size_t print_stdout(char* str);
size_t print_stderr(char* str);

size_t count_by_fd(size_t fd){

	size_t num_of_read_chars=0; /*wchar*/
    ssize_t num_of_read_bytes=0;
    size_t is_last_char_space=1;

    char buf[BUF_SIZE];
    wchar_t wchar=0;

    setlocale(LC_ALL, "");

    
    size_t rest=0;
    size_t offset=0;

    int is_even=0;

    /*занулить переменные!!!!*/
    while((num_of_read_bytes = read(fd, buf+rest, BUF_SIZE-rest))!=0){

        if(num_of_read_bytes==-1){
            panic("read");
            return 0;
        }
        
        
        ret=0;
        offset=0;
        while((ret=mbtowc(&wchar, buf + offset, MB_CUR_MAX)) > 0  && offset+ret<= num_of_read_bytes+rest){
            if(is_even){
            	write(1, buf + offset, ret);
            	is_even=0;
            }
            else
            	is_even++;
            offset+=ret;
        }
        rest=num_of_read_bytes+rest-offset;

        if(ret==-1){
            memmove(buf, buf+offset, rest);
        }
    }

	return 0;
}

void panic(char* str){
	perror(str);
}


size_t print_stdout(char* str) {
    ret = write(1, str, strlen(str));
    if(ret==-1){
        panic("write");
        return 0;
    }
}

size_t print_stderr(char* str) {
    ret = write(2, str, strlen(str));
    if(ret==-1){
        //perror("wc");
        //panic();
        return 0;
    }
}



int main(int argc, char **argv){

	pid_t pid;
	int fd[2];
	pipe(fd);

	switch(pid=fork()) {
 		case -1:
        	panic("fork"); /* произошла ошибка */
 			return 1; /*выход из родительского процесса*/
  		case 0:
  		/*процесс-потомок*/
  			dup2(fd[0],0);
  			close(fd[0]);
  			close(fd[1]);
  			execl("/usr/bin/wc","wc",(void*)0);//execl("wc");
        	return 0;
  		default:
  		/*процесс-родиель*/
  			dup2(fd[1],1);
			close(fd[1]);
			close(fd[0]);
			
			int ret=0;
			char** inputFiles = argv + optind;
    		size_t numInputFiles = argc - optind;

    if(numInputFiles!=0)
    {
        for(size_t i=0; i<numInputFiles; i++)
        {

            struct stat filestat;
            ret = stat(inputFiles[i], &filestat);
            if(ret==0)
            {
                if( filestat.st_blocks < filestat.st_size/512 && filestat.st_size/512> BUF_SIZE)
                {
                    print_stderr(inputFiles[i]);
                    print_stderr(": It's a sparse file. I don't want to work with it.\n");
                    continue;
                }
            }          

            ret = open(inputFiles[i], O_RDONLY);
            if(ret==-1)
            {
                panic("open");
                continue;
            }
            
            count_by_fd(ret);
            close(ret);
        }
    }
    else
    {
        //count_by_fd(0); //читать stdin
    }
  }

}