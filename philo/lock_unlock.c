#include "Philosophers.h"

void set_val(pthread_mutex_t *mutex, int *dest, int value)
{
    mutex_handle(mutex, LOCK);
    *dest = value;
    mutex_handle(mutex, UNLOCK);
}

int get_val(pthread_mutex_t *mutex, int *val)
{   
    int res;

    mutex_handle(mutex, LOCK);
    res = *val;
    mutex_handle(mutex, UNLOCK);
    return (res);
}

void set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
    mutex_handle(mutex, LOCK);
    *dest = value;
    mutex_handle(mutex, UNLOCK);
}

int get_bool(pthread_mutex_t *mutex, bool *val)
{   
    bool res;

    mutex_handle(mutex, LOCK);
    res = *val;
    mutex_handle(mutex, UNLOCK);
    return (res);
}

bool sim_finished(t_prog *prog)
{
    return(get_bool(&prog->table_mutex, &prog->end_sim));
}