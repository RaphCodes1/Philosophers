#include "Philosophers.h"

bool philo_dead(t_philo *philo)
{
    long elapsed;
    long time_to_die;

    if(get_bool(&philo->philo_mutex, &philo->full))
        return (false);
    
    elapsed = get_time(MILLISECOND) - get_val(&philo->philo_mutex, 
        &philo->last_meal_time);
    time_to_die = philo->program->time_to_die / 1000;
    if(elapsed > time_to_die)
        return (true);
    return(false);
}
void *monitor_dinner(void *data)
{
    t_prog *prog;
    int     i;
    prog = (t_prog *)data;
    while(!threads_run_check(&prog->table_mutex, 
		prog->num_of_philos, &prog->threads_running_nbr))
        ;
    while(!sim_finished(prog))
    {   
        i = 0;
        while(i < prog->num_of_philos && !sim_finished(prog))
        {
            if(philo_dead(&prog->philos[i]))
            {
                write_status(DIED, &prog->philos[i], DEBUG_MODE);
                set_bool(&prog->table_mutex, &prog->end_sim, true);
            }
            i++;
        }
    }
    return (NULL);
}