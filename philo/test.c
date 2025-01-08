#include "Philosophers.h"
#define BIG 100000000UL

void *checking(void *arg)
{
    int i;

    i = 0;
    while(i <= 100)
        i++;
    printf("done with loop\n");
    return (NULL);
}

typedef struct s_utils
{
    uint32_t *a;
    pthread_mutex_t *a_lock;
}   t_utils;


void *change_num(void *arg)
{   
    t_utils *utils;

    utils = (t_utils *)arg;
    pthread_mutex_lock(utils->a_lock);
    int i;

    i = 1;
    while(i <= BIG)
    {
        (*(utils->a))++;
        i++;
    }
    pthread_mutex_unlock(utils->a_lock);
    return (NULL);
}
int main(int ac, char **av)
{
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;
    struct s_utils utils;
    pthread_mutex_t mutex_lock;
    uint32_t a;
    
    a = 0;
    pthread_mutex_init(&mutex_lock,NULL);
    utils = (struct s_utils){&a, &mutex_lock};
    // pthread_mutex_lock(&mutex_lock);
    pthread_create(&thread_1, NULL, change_num, &utils);
    pthread_create(&thread_3, NULL, change_num, &utils);
    pthread_create(&thread_2, NULL, &checking, NULL);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);
    // pthread_mutex_unlock(&mutex_lock);
    printf("a is %u\n", a);
}