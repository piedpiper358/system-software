#include <fcntl.h> /* open */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strlen strcpy */
#include <unistd.h> /* для getopt и read */
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <errno.h>
#include <stdarg.h> /*va_arg*/
#include <sys/types.h> 
#include <sys/stat.h> 

//#include <ctime.h> 

//#define _GNU_SOURCE

#define BUF_SIZE 4*1024*1024

typedef union {
    unsigned val:4; 
    struct {
        unsigned l : 1;
        unsigned w : 1;
        unsigned c : 1;
        unsigned m : 1;
    };
} Opt_t;

size_t print_stdout(char* str);

size_t count_by_fd(size_t fd);
size_t print_file_result(Opt_t options, char* filename);
size_t print_total_result(Opt_t options);
size_t print_stderr(char* str);
void panic();
/*size_t ext_print(int num, ...);*/

//Global vars
size_t num_of_bytes = 0;
size_t num_of_chars = 0;
size_t num_of_lines = 0;
size_t num_of_words = 0;

size_t sum_of_bytes = 0;
size_t sum_of_chars = 0;
size_t sum_of_lines = 0;
size_t sum_of_words = 0;

size_t ret;

//char* USAGE = "usage: wc [cClmw] [file]...\n";
char* USAGE = "usage: wc [-c | -m | -C] [-lw] [file ...]\n";

//extern char **environ;

int main(size_t argc, char **argv, char **env) {

    char* optString = "cClmw";
    char opt;

    //Opt_t options.<anonymous> = {1, 1, 0, 1};
    Opt_t options;
    options.val = 0; 


    while( (opt = getopt( argc, argv, optString ))!= -1 ) {
        
        switch( opt ) {
            case 'c':
                if(options.m){
                    print_stderr(USAGE);
                    return 1; //error
                }
                options.c=1;
                break;
            case 'l':
                options.l=1;
                break;
            case 'C': /* намеренный проход в следующий case-блок */
            case 'm':
                if(options.c){
                    print_stderr(USAGE);
                    return 1; //error
                }
                options.m=1;
                break;
            case 'w':
                options.w=1;
                break;   
            case '?':
            default:
                print_stderr(USAGE);
                return 1;
                break;
        }
        //options[0]='\0';
        //options = {0, 0, 0, 0};
        //options.val=0;
        /*switch( opt ) {
            case 'c':
                //strcat(options, "c");
                options.c=1;
                break;
            case 'l':
                options.l=1;
                break;
            case 'C':    // намеренный проход в следующий case-блок 
            case 'm':
                options.m=1;
                break;
            case 'w':
                options.w=1;
                break;   
            case '?':
            default:
                display_usage();
                break;
        }*/
    }
    if(options.val==0){
        options.val=7; /*by default -lwc*/
    }
   
    
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
                panic();
                continue;
            }
            
            count_by_fd(ret);
            close(ret);
            print_file_result(options, inputFiles[i]);
        }
         if(numInputFiles>1){
            print_total_result(options);
        }
    }
    else
    {
        count_by_fd(0); //читать stdin
        print_file_result(options, "");
    }
}


size_t count_by_fd(size_t fd){
    size_t num_of_read_chars=0; /*wchar*/
    ssize_t num_of_read_bytes=0;
    size_t is_last_char_space=1;

    char buf[BUF_SIZE];
    wchar_t wchar=0;

    setlocale(LC_ALL, "");

    size_t ret=0;
    size_t rest=0;
    size_t offset=0;

    /*занулить переменные!!!!*/
    while((num_of_read_bytes = read(fd, buf+rest, BUF_SIZE-rest))!=0){

        if(num_of_read_bytes==-1){
            panic();
            return 0;
        }
        
        num_of_bytes += num_of_read_bytes;
        
        ret=0;
        offset=0;
        while((ret=mbtowc(&wchar, buf + offset, MB_CUR_MAX)) > 0  && offset+ret<= num_of_read_bytes+rest){

            offset+=ret;
            num_of_chars++;
            if( !iswspace(wchar) && is_last_char_space ){
                num_of_words++;
            }
            is_last_char_space = iswspace(wchar);

            if( wchar==L'\n' )
                num_of_lines++;
            
        }

        rest=num_of_read_bytes+rest-offset;

        if(ret==-1){
            memmove(buf, buf+offset, rest);
        }
    }
}

void panic(){
    //perror("wc");
    //perror(inputFiles[i]);
    char* errstr = strerror(errno);
    print_stderr("wc:");
    print_stderr(errstr);
    print_stderr("\n");
}

size_t print_stdout(char* str) {
    ret = write(1, str, strlen(str));
    if(ret==-1){
        panic();
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


size_t print_file_result(Opt_t options, char* filename){
    
    char *newstr=NULL;
    if(options.l){
        asprintf(&newstr, "\t%lu", num_of_lines);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.w){
        asprintf(&newstr, "\t%lu", num_of_words);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.c){
        asprintf(&newstr, "\t%lu", num_of_bytes);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.m){
        asprintf(&newstr, "\t%lu", num_of_chars);
        print_stdout(newstr);
        free(newstr);
    }

    //ext_print(3 "\t", filename, "\n");
    print_stdout("\t");
    print_stdout(filename);
    print_stdout("\n");

    sum_of_bytes += num_of_bytes;
    sum_of_chars += num_of_chars;
    sum_of_lines += num_of_lines;
    sum_of_words += num_of_words;

    num_of_bytes = 0;
    num_of_chars = 0;
    num_of_lines = 0;
    num_of_words = 0;
}

size_t print_total_result(Opt_t options){
    char *newstr=NULL;
    if(options.l){
        asprintf(&newstr, "\t%lu", sum_of_lines);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.w){
        asprintf(&newstr, "\t%lu", sum_of_words);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.c){
        asprintf(&newstr, "\t%lu", sum_of_bytes);
        print_stdout(newstr);
        free(newstr);
    }
    if(options.m){
        asprintf(&newstr, "\t%lu", sum_of_chars);
        print_stdout(newstr);
        free(newstr);
    }
    //ext_print(1 "\ttotal\n");
    print_stdout("\ttotal\n");
}



/*size_t ext_print(int num, ...) {
    char* str;
    va_list args;
    va_start(args, num);
    for(;num;num--)
    {
        str = va_arg(args, char*);

        ret = write(1, str, strlen(str));

        if(ret==-1)
        {
            perror("wc");

        }
    }
    va_end(args);
}*/