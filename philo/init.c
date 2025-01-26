#include "Philosophers.h"

void assign_forks(t_philo *philo, t_fork *forks, int curr_pos)
{	
	int philo_nbr;

	philo_nbr = philo->program->num_of_philos;
	philo[curr_pos].r_fork = &forks[(curr_pos + 1) % philo_nbr];
	philo[curr_pos].l_fork = &forks[curr_pos];
	if(philo->id % 2 == 0)
	{
		philo[curr_pos].r_fork = &forks[curr_pos];
		philo[curr_pos].l_fork = &forks[(curr_pos + 1) % philo_nbr];
	}
}
void philo_init(t_prog *prog)
{
	int i;
	t_philo *philo;

	i = -1;
	philo = prog->philos;
	while(++i < prog->num_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].full = false;
		philo[i].meal_count = 0;
		philo[i].program = prog;
		assign_forks(philo, prog->forks, i);
		mutex_handle(&philo[i].philo_mutex, INIT);
	}
}
void data_init(t_prog *prog, char **av)
{	
	int i;
	
	i = -1;
	prog->end_sim = false;
	prog->threads_ready = false;
	prog->threads_running_nbr = 0;
	prog->philos = safe_malloc(sizeof(t_philo) * prog->num_of_philos);
	mutex_handle(&prog->table_mutex, INIT);
	mutex_handle(&prog->write_lock, INIT);
	prog->forks = safe_malloc(sizeof(t_philo) * prog->num_of_philos);
	while(++i < prog->num_of_philos)
	{
		mutex_handle(&prog->forks[i].fork, INIT);
		prog->forks[i].fork_id = i;
	}
	philo_init(prog);
}

int av_input(t_prog *prog, char **av)
{	
	//need to PROTECT from overflow INTMIN INTMAX LONGMAX LONGMIN
	prog->num_of_philos = ft_atol(av[1]);
	prog->time_to_die = ft_atol(av[2]) * 1000; // * 1e3; equals 1000;
	prog->time_to_eat = ft_atol(av[3]) * 1000; // *1e3; 
	prog->time_to_sleep = ft_atol(av[4]) * 1000; // * 1e3;
	if(prog->time_to_die < 6e4 ||
		prog->time_to_eat < 6e4 ||
			prog->time_to_sleep < 6e4)
	{
		printf(RED "use more than 60ms\n" RESET);
		return (0);
	}
	if(prog->num_of_philos > 200)
	{
		printf(RED "no more than 200 philos\n" RESET);
		return (0);
	}
	if(av[5])
		prog->num_times_to_eat = ft_atol(av[5]);
	else
		prog->num_times_to_eat = -1;
	return (1);
}