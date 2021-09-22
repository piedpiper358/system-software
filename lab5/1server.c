
#define my_key 207555

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/shm.h>
#include <signal.h>
//#include <sys/loadavg.h>



int working = 1;

struct info_t{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t time;
    double loadavg[3];
} *newinfo;

void panic(char* str){
    perror(str);
}

void pr_exit(int sig){
    working = 0;  
}

 
int main()
{
    time_t start_time = time(NULL);
    int shmid;/* IPC дескриптор для области разделяемой памяти */
  
    /* Пытаемся эксклюзивно создать разделяемую память для сгенерированного ключа*/
    if((shmid = shmget(my_key, sizeof(*newinfo), 0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            /* Если по другой причине – прекращаем работу */
            panic("shmget");
            return -1;
        } 
        else {
            /* разделяемая память уже существует*/
            if((shmid = shmget(my_key, sizeof(*newinfo), 0)) < 0){
                panic("shmget");
                return -1;
            }
        }
    }
    /*отображаем в память*/
    if((newinfo = shmat(shmid, NULL, 0)) == (void*)(-1)){
        panic("shmat");
        return -1;
    }

    signal(SIGINT, pr_exit);

    newinfo->pid=getpid();
    newinfo->uid=getuid();
    newinfo->gid=getgid();
    //time_t cur_time;
    while(working){
        //newinfo->time = clock() / CLOCKS_PER_SEC;
        //time(&newinfo->time);
        time_t finish_time = time(NULL);

        newinfo->time = finish_time - start_time;
        //newinfo->time = (float)(clock()-start_time)/CLOCKS_PER_SEC;
        getloadavg(newinfo->loadavg, 3);
        sleep(1);
    }

    if((void*)shmdt(newinfo) < 0){ 
        panic("shmdt");
    }
    return 0;
}
