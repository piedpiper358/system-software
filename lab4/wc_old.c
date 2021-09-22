#include <fcntl.h> /* open */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strlen strcpy */
#include <unistd.h> /* для getopt() и read */
#include <wchar.h>

#include <wctype.h>
#include <locale.h>

#include <errno.h>

#include <stdarg.h> /*va_arg*/
#define BUF_SIZE 4

/*typedef struct {
    unsigned c : 1;
    unsigned l : 1;
    unsigned m : 1;
    unsigned w : 1;
}  opt_t;
typedef union {
    unsigned val:4; 
    opt_t;
} Opt_t;*/

typedef union {
    unsigned val:4; 
    struct {
        unsigned l : 1;
        unsigned w : 1;
        unsigned c : 1;
        unsigned m : 1;
    };
} Opt_t;

int print(char* str);
int display_usage();
int count_by_fd(int fd);
int print_file_result(Opt_t options, char* filename);
int print_total_result(Opt_t options/*, char* filename*/);
int ext_print(unsigned int num, ...);

//Global vars
size_t num_of_bytes = 0;
size_t num_of_chars = 0;
size_t num_of_lines = 0;
size_t num_of_words = 0;

size_t sum_of_bytes = 0;
size_t sum_of_chars = 0;
size_t sum_of_lines = 0;
size_t sum_of_words = 0;


//char* USAGE = "usage: wc [cClmw] [file]...\n";
char* USAGE = "usage: wc [-c | -m | -C] [-lw] [file ...]\n";

//extern char **environ;

int main(int argc, char **argv, char **env) {

  char* optString = "cClmw";
  char opt;
  int ret;

    //Opt_t options.<anonymous> = {1, 1, 0, 1};
  Opt_t options;
  options.val = 0; 


    while( (opt = getopt( argc, argv, optString ))!= -1 ) {
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

        switch( opt ) {
            case 'c':
                if(options.m){
                    display_usage(); //error
                }
                options.c=1;
                break;
            case 'l':
                options.l=1;
                break;
            case 'C': /* намеренный проход в следующий case-блок */
            case 'm':
                if(options.c){
                    display_usage(); //error
                }
                options.m=1;
                break;
            case 'w':
                options.w=1;
                break;   
            case '?':
            default:
                display_usage();
                break;
        }
    }
    if(options.val==0){
        options.val=7; /*by default -lwc*/
    }
   
    
    char** inputFiles = argv + optind;
    int numInputFiles = argc - optind;

    if(numInputFiles!=0)
    {
        for(size_t i=0; i<numInputFiles; i++)
        {
            ret = open(inputFiles[i], O_RDONLY);
            if(ret==-1)
            {
                //errno=ENOENT;
                perror(inputFiles[i]);
                //ext_print(3, "wc: cannot open ", inputFiles[i], "\n");
                continue;
            }
            count_by_fd(ret);
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

   

    /*printf("\n---------------------------------\n");
    printf("bytes: %lu \t chars: %lu\n", sum_of_bytes, sum_of_chars);
    printf("words: %lu \t lines: %lu\n", sum_of_words, sum_of_lines);
    printf("---------------------------------\n");*/
}



int display_usage() {
	print(USAGE);
    exit(1);
}

int count_by_fd(int fd){
    size_t num_of_read_chars=0; /*wchar*/
    ssize_t num_of_read_bytes=0;
    int is_last_char_space=1;

    char buf[BUF_SIZE];
    wchar_t wbuf[BUF_SIZE];

    setlocale(LC_ALL, "");

        int ret=0;

        int rest=0;
        size_t offset=0;

    /*занулить переменные!!!!*/
    while((num_of_read_bytes = read(fd, buf+rest, BUF_SIZE-rest))!=0){

        if(num_of_read_bytes==-1){
            return 0;
        }
        memset(buf+num_of_read_bytes, '\0', BUF_SIZE - num_of_read_bytes );
        //printf("Исходная строка:\n");
        //write(1, buf, num_of_read_bytes);
        num_of_bytes += num_of_read_bytes;
        
        //printf("----%li-----\n", num_of_read_bytes);
        


        num_of_read_chars = mbstowcs(wbuf, buf, BUF_SIZE);

        if(num_of_read_chars==-1)
        {

            //return 0;
            printf("Страшное случилось");
            continue;
        }
        num_of_chars += num_of_read_chars;
        //printf("%ls", wbuf);
        
        for(size_t i=0; i<num_of_read_chars; i++ )
        {
            if( !iswspace(wbuf[i]) &&  is_last_char_space )
            {
                num_of_words++;
            }
            is_last_char_space = iswspace(wbuf[i]);

            if( wbuf[i]==L'\n' )
            {
                num_of_lines++;
            }
        }
        ///*{ /*вывод 
        //int ret = wcstombs( buf, wbuf, BUF_SIZE);
        //printf("\nПолучилось %lu:\n", num_of_read_chars);
        //write(1, buf, ret );
        //}*/
    }
   
}



int print(char* str) {
	write(1, str, strlen(str));
}
int ext_print(unsigned int num, ...) {
    char* str;
    va_list args;
    va_start(args, num);
    for(;num;num--)
    {
        str = va_arg(args, char*);

        write(1, str, strlen(str));
    }
    va_end(args);
}


int print_file_result(Opt_t options, char* filename){
    int offset=0;
    char str[50];
    if(options.l){
        offset += sprintf(str+offset, "\t%lu", num_of_lines);
    }
    if(options.w){
        offset += sprintf(str+offset, "\t%lu", num_of_words);
    }
    if(options.c){
        offset += sprintf(str+offset, "\t%lu", num_of_bytes);
    }
    if(options.m){
        offset += sprintf(str+offset, "\t%lu", num_of_chars);
    }

    ext_print(4, str, "\t", filename, "\n");

    sum_of_bytes += num_of_bytes;
    sum_of_chars += num_of_chars;
    sum_of_lines += num_of_lines;
    sum_of_words += num_of_words;

    num_of_bytes = 0;
    num_of_chars = 0;
    num_of_lines = 0;
    num_of_words = 0;
}

int print_total_result(Opt_t options){
    int offset=0;
    char str[50];

    if(options.l){
        offset += sprintf(str+offset, "\t%lu", sum_of_lines);
    }
    if(options.w){
        offset += sprintf(str+offset, "\t%lu", sum_of_words);
    }
    if(options.c){
        offset += sprintf(str+offset, "\t%lu", sum_of_bytes);
    }
    if(options.m){
        offset += sprintf(str+offset, "\t%lu", sum_of_chars);
    }
    ext_print(2, str, "\ttotal\n");
}