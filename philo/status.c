#include "Philosophers.h"

void eat(t_philo *philo)
{	
	if(philo->full)
		return;
	if (philo->id % 2 == 0) {
        mutex_handle(&philo->r_fork->fork, LOCK);
        write_status(TAKE_R_FORK, philo, DEBUG_MODE);
        mutex_handle(&philo->l_fork->fork, LOCK);
        write_status(TAKE_L_FORK, philo, DEBUG_MODE);
    } else {
        mutex_handle(&philo->l_fork->fork, LOCK);
        write_status(TAKE_L_FORK, philo, DEBUG_MODE);
        mutex_handle(&philo->r_fork->fork, LOCK);
        write_status(TAKE_R_FORK, philo, DEBUG_MODE);
    }
	set_val(&philo->philo_mutex, &philo->last_meal_time, 
		get_time(MILLISECOND));
	prec_usleep(philo->program->time_to_eat, philo->program);
	if(philo->program->num_times_to_eat > 0 &&
		philo->meal_count == philo->program->num_times_to_eat)
		set_bool(&philo->philo_mutex, &philo->full, true);
	if(!get_bool(&philo->philo_mutex, &philo->full))
	{
		philo->meal_count++;
		write_status(EATING, philo, DEBUG_MODE);
	}
	mutex_handle(&philo->r_fork->fork, UNLOCK);
	mutex_handle(&philo->l_fork->fork, UNLOCK);
}

void think(t_philo *philo, bool pre_sim)
{	
	if(!pre_sim)	
		write_status(THINKING, philo, DEBUG_MODE);
	if(philo->program->num_of_philos % 2 == 0)
		return ;
	else
		prec_usleep(2000, philo->program);
}

void sleeping(t_philo *philo)
{
	write_status(SLEEPING, philo, DEBUG_MODE);
	prec_usleep(philo->program->time_to_sleep, philo->program);
}

void *one_philo(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	wait_threads(philo);
	set_val(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_val(&philo->program->table_mutex,
		&philo->program->threads_running_nbr);
	write_status(TAKE_R_FORK, philo, DEBUG_MODE);
	while(!sim_finished(philo->program))
		usleep(200);
	return (NULL);
}

void *dinner_sim(void *data)
{	
	t_philo *philo;

	philo = (t_philo *)data;
	wait_threads(philo);
	set_val(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_val(&philo->program->table_mutex,
		&philo->program->threads_running_nbr);
	desync_philo(philo);
	while(!sim_finished(philo->program))
	{	
		if(philo->full)
			break;
		eat(philo);
		sleeping(philo);
		think(philo, false);
	}
	return (NULL);
}