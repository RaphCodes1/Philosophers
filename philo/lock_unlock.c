#include "Philosophers.h"

void set_val(pthread_mutex_t **mutex, int *dest, int value)
{
    mutex_handle((*mutex), LOCK);
    *dest = value;
    mutex_handle((*mutex), UNLOCK);
}

int get_val(pthread_mutex_t **mutex, int *val)
{   
    int res;

    mutex_handle((*mutex), LOCK);
    res = *val;
    mutex_handle((*mutex), UNLOCK);
    return (res);
}

// int sim_finished(t_program *program)
// {
//     return(get_val(&program->table_mtx, &program->end_sim));
// }