#include "Philosophers.h"

void wait_threads(t_philo *philo)
{
	while(!get_bool(&philo->program->table_mutex, 
		&philo->program->threads_ready))
		;
}

void increase_val(pthread_mutex_t *mutex, long *val)
{
    mutex_handle(mutex, LOCK);
	(*val)++;
	mutex_handle(mutex, UNLOCK);
}

bool threads_run_check(pthread_mutex_t *mutex,long philo_num, long *threads)
{
    bool check;

    check = false;
    mutex_handle(mutex, LOCK);
    if(*threads == philo_num)
        check = true;
    mutex_handle(mutex, UNLOCK);
}
