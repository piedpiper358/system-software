//4. Основной поток раз в секунду поочерёдно пробуждает один из потоков и дожидается конца его работы, 
//после чего выводит полученный массив на стандартный вывод. 
//Для реализации синхронизации необходимо использовать sem_init(3RT), sem_post(3RT), sem_wait(3RT).

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include <signal.h>
//#include <time.h>


sem_t sem_reverse, sem_invert, sem_print;


void panic(char* str){
	perror(str);
}

/*size_t post(sem_t* sem) {
	if (sem_post(sem)!= 0) {
		panic("sem_post");
		//return -1;
	}
}*/


int reverse(char* str){
	while(1){
		if (sem_wait(&sem_reverse)!= 0) {
			panic("sem_wait");
			return -1;
		}

		size_t len = strlen(str);

		for(size_t i=0; i< len/2; i++){
			char c = str[i];
			str[i] = str[len-i-1];
			str[len-i-1] = c;
		}
		if (sem_post(&sem_print)!= 0) {
			panic("sem_post");
			return -1;
		}
	}
}

int invert(char* str){
	while(1){
		if (sem_wait(&sem_invert)!= 0) {
			panic("sem_wait");
			return -1;
		}

		for(int i=0;i<strlen(str);i++)
		{
	        if(islower(str[i]))
	            str[i]=toupper(str[i]);
	        else   
	            str[i]=tolower(str[i]);
		}
		if (sem_post(&sem_print)!= 0) {
			panic("sem_post");
			return -1;
		}
	}
}

void pr_exit(int sig){
	if (sem_destroy(&sem_reverse)!= 0) {
		panic("sem_destroy");
	}
	if (sem_destroy(&sem_invert)!= 0) {
		panic("sem_destroy");
	}
   	exit(-1);
}

int main(int argc, char** argv) {

	char alphabet[] = "abcdefghijklmnopqrstuvwxyz";


	pthread_t th_reverse, th_invert;
	int ret;
	int status_addr;

	
    ret = sem_init(&sem_reverse, 0, 0);
	if (ret!= 0) {
		panic("sem_init");
		return -1;
	}
	ret = sem_init(&sem_invert, 0, 0);
	if (ret!= 0) {
		panic("sem_init");
		return -1;
	}
	ret = sem_init(&sem_print, 0, 0);
	if (ret!= 0) {
		panic("sem_init");
		return -1;
	}
	signal(SIGINT, pr_exit);

	ret = pthread_create(&th_reverse, NULL, (void* (*)(void*)) reverse, (void*)alphabet);
	if (ret!= 0) {
		panic("pthread_create");
		return -1;
	}
	ret = pthread_create(&th_invert, NULL, (void* (*)(void*)) invert, (void*)alphabet);
	if (ret != 0) {
		panic("pthread_create");
		return -1;
	}

	printf("%s\n", alphabet);
	/*print_stdout(alphabet);
	print_stdout("\n");*/

	while(1){
		if (sem_post(&sem_reverse)!= 0) {
			panic("sem_post");
			return -1;
		}
		sleep(1);
		if (sem_wait(&sem_print)!= 0) {
			panic("sem_wait");
			return -1;
		}
		
		printf("%s\n", alphabet);
		/*print_stdout(alphabet);
		print_stdout("\n");*/
		
		if (sem_post(&sem_invert)!= 0) {
			panic("sem_post");
			return -1;
		}
		sleep(1);
		if (sem_wait(&sem_print)!= 0) {
			panic("sem_wait");
			return -1;
		}
		
		printf("%s\n", alphabet);
		/*print_stdout(alphabet);
		print_stdout("\n");*/
	}
	
	//sem_init(3RT), sem_post(3RT), sem_wait(3RT).
    /*sem_open and sem_close*/
	//clock_t start = clock();
	//clock()-start; nanostart


    return 0;
}