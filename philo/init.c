#include "Philosophers.h"

void philo_struct_int(t_philo **philo, t_program *program, 
	pthread_mutex_t *forks, char **av)
{	
	int num_of_philo;
	int i;

	i = -1;
	num_of_philo = ft_atoi(av[1]);
	(*philo) = safe_malloc(num_of_philo * sizeof(t_philo));
	while(++i < num_of_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].eating = 0;
		(*philo)[i].meals_eaten = 0;
		int_argv_content(&(*philo)[i],av);
		(*philo)[i].start_time = 0; // get_curr_time();
		(*philo)[i].last_meal = 0; // get_curr_time();
		(*philo)[i].write_lock = &program->write_lock;
		(*philo)[i].dead_lock = &program->dead_lock;
		(*philo)[i].meal_lock = &program->meal_lock;
		(*philo)[i].table_mtx = &program->table_mtx;
		(*philo)[i].dead = &program->dead_flag;
		(*philo)[i].l_fork = &forks[i];
		if(i == 0)
			(*philo)[i].r_fork = &forks[(*philo)->num_of_philos - 1];
		else
			(*philo)[i].r_fork = &forks[i - 1];
	}
}

void program_init(t_program *program, t_philo *philos)
{	
	// program = safe_malloc(sizeof(t_program));
	program->dead_flag = 0;
	program->sync_philos = 0;
	program->start_sim = 0;
	program->end_sim = 0;
	program->philos = philos;
	mutex_handle(&program->dead_lock, INIT);
	mutex_handle(&program->meal_lock, INIT);
	mutex_handle(&program->write_lock, INIT);
	mutex_handle(&program->table_mtx, INIT);
}

void forks_init(pthread_mutex_t **forks, int num_of_philos)
{
	int i;

	i = 0;
	(*forks) = safe_malloc(num_of_philos * sizeof(pthread_mutex_t));
	while(i < num_of_philos)
	{
		mutex_handle(&(*forks)[i], INIT);
		i++;
	}
}

void int_argv_content(t_philo *philo, char **av)
{	
	philo->num_of_philos = ft_atoi(av[1]);
	philo->time_to_die = ft_atoi(av[2]);
	philo->time_to_eat = ft_atoi(av[3]);
	philo->time_to_sleep = ft_atoi(av[4]);
	if(av[5])
		philo->num_times_to_eat = ft_atoi(av[5]);
	else
		philo->num_times_to_eat = -1;
}