/* #define _USE_MATH_DEFINES
#include <math.h>  */
#define M_PI 3.14159265358979323846
#include <fcntl.h> /* open */
#include <math.h>
#include <stdint.h> /* uint32_t */
#include <stdio.h>
#include <stdio_ext.h> /* __fpurge */
#include <stdlib.h>
#include <string.h> /* strlen strcpy */
#include <unistd.h> /* read */

#define NUM_MAIN_MENU_ITEMS 5
#define NUM_MORPH_MENU_ITEMS 4
#define NUM_PRINT_MENU_ITEMS 3

/*#include "image.h"*/


int print(char* str);
int usage();


char* USAGE = "usage: /usr/bin/ksh [_ abCefhikmnoprstuvx] [_ o option]... [arg]...\n";
char *PS1 = "s207555@helios:/home/s207555$ ";
char *cmd;
extern char **environ;

int main(int argc, char **argv, char **env) {

  for (int i = 0; i < argc; i++) {
  }

  while (1) {
	//printf("%s", getenv("PS1"),
    printf("%s%s%s%s", getenv("USER"), "@helios:", getenv("PWD"), "$ ");
    //printf("%s", PS1);
    scanf("%s", &cmd);
    //read(0, cmd, );
    __fpurge(stdin);
    // write(1, , strlen);

    usage();
    if(cmd=="exit")
    {
    	break;
    	exit;
    }
//char* exit="exit";
    //if(strncmp(cmd, exit, 4))
    //{
    	//break;
    //	exit;

    //}
  }
}

int main2() {


//    PWD=strcat(strcat(strcat(getenv("USER"), "@helios:"), getenv("PWD")), "$ ");
    //printf("%s", PS1);
    
}

int usage() {
	print(USAGE);
}

int print(char* str) {
	write(1, str, strlen(str));
}
