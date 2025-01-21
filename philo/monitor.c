#include "Philosophers.h"

void monitor_dinner(void *data)
{
    t_prog *prog;
    int     i;
    prog = (t_prog *)data;
    while(threads_run_check(&prog->table_mutex, 
		prog->num_of_philos, &prog->threads_running_nbr))
        ;
    
    while(!sim_finished(prog))
    {   
        i = -1;
        while(++i < prog->num_of_philos && !sim_finished(prog))
        {
            if(philo_dead(prog->philos + i))
            {
                set_bool(&prog->table_mutex, &prog->end_sim, true);
                write_status(DIED, &prog->philos + i, DEBUG_MODE);
            }

        }
    }
}