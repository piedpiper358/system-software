/*Сервер при запуске должен сохранить:
-идентификатор процесса;
-идентификатор пользователя процесса;
-идентификатор группы процесса;
и раз в секунду обновлять:
-количество секунд, прошедших с момента запуска процесса;
-среднюю загрузку системы за последние 1, 5 и 15 минут.
Сервер должен быть одним процессом с одним потоком.
Клиент должен один раз выводить текущую информацию от сервера.

Для получения необходимой информации будут полезны man страницы:
getpid(2), getuid(2), getgid(2), time(2), getloadavg(3C).*/

//1. System V shared memory
//Для хранения требуемой информации и обмена информацией между клиентом и сервером
//используется разделяемый сегмент памяти. man shmget(2), shmat(2), shmdt(2).

#define _BSD_SOURCE
#define my_key 207555

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/shm.h>
//#include <sys/loadavg.h>

typedef struct info_t{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t time;
    double loadavg[3];
} info_t;


void panic(char* str){
    perror(str);
}

int main()
{
    int shmid;    /* IPC дескриптор для области разделяемой памяти */
    info_t* newinfo;
   
    /* Пытаемся эксклюзивно создать разделяемую память для сгенерированного ключа*/
    if((shmid = shmget(my_key, sizeof(info_t),0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            /* Если по другой причине – прекращаем работу */
            panic("shmget");
            return -1;
        } 
        else {
            /* разделяемая память уже существует*/
            if((shmid = shmget(my_key, sizeof(info_t), 0)) < 0){
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
   
    printf("PID: %i\n", newinfo->pid);
    printf("UID: %i\n", newinfo->uid);
    printf("GID: %i\n", newinfo->gid);
    printf("Количество секунд, прошедших с момента запуска процесса: %li\n", newinfo->time);
    printf("Cредняя загрузка системы за последнюю 1 минуту: %lf\n", newinfo->loadavg[0]);
    printf("Cредняя загрузка системы за последние 5 минут: %lf\n", newinfo->loadavg[1]);
    printf("Cредняя загрузка системы за последние 15 минут: %lf\n", newinfo->loadavg[2]);

    if(shmdt((void*)newinfo) < 0){ 
        panic("shmdt");
        return -1;
    }
    return 0;
}
