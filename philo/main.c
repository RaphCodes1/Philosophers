#include "Philosophers.h"

int check_valid_args(char **av)
{
    int i;
	int f;
	size_t check;

	f = 1;
	while(av[f])
	{	
		check = 0;
    	i = 0;
		while(av[f][i])
		{
			if(av[f][i] >= '0' && av[f][i] <= '9')
				check++;
			i++;
		}
		if(ft_strlen(av[f]) != check)
			return (1);
		f++;
	}
	return (0);
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
void philo_struct_int(t_philo **philo, t_program *program, pthread_mutex_t *forks, char **av)
{	
	int num_of_philo;
	int i;
	int holder;

	i = 0;
	num_of_philo = ft_atoi(av[1]);
	(*philo) = malloc(num_of_philo * sizeof(t_philo));
	if(!(*philo))
		perror("Malloc Fail");
	while(i < num_of_philo)
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
		if(i == 0)
			(*philo)[i].r_fork = &forks[(*philo)->num_of_philos - 1];
		else
			(*philo)[i].r_fork = &forks[i - 1];
		i++;
	}
}

void program_init(t_program *program, t_philo *philos)
{
	program->dead_flag = 0;
	program->philos = philos;
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
	pthread_mutex_init(&program->write_lock, NULL);
}

void forks_init(pthread_mutex_t **forks, int num_of_philos)
{
	int i;

	i = 0;
	(*forks) = malloc(num_of_philos * sizeof(pthread_mutex_t));
	if(!(*forks))
		perror("malloc failed!");
	while(i < num_of_philos)
	{
		pthread_mutex_init(&(*forks)[i], NULL);
		i++;
	}
}

int main(int ac, char **av)
{   
	t_philo 	*philos;
	t_program	program;
	pthread_mutex_t	*forks;
    if(ac == 5 || ac == 6)
    {	
		
		if(!check_valid_args(av))
		{	
			program_init(&program,philos);
			forks_init(&forks, ft_atoi(av[1]));
			philo_struct_int(&philos, &program, forks, av);
		}
		int i = 0;
		
		while(i < 5)
		{
			printf("id: %d\n", philos[i].id);
			printf("num of philos: %d\n", philos[i].num_of_philos);
			printf("time to die: %d\n", philos[i].time_to_die);
			printf("time to sleep: %d\n", philos[i].time_to_sleep);
			printf("time to eat: %d\n", philos[i].num_times_to_eat);
			i++;
		}
    }
    else
        printf("enter valid arguments :)\n");
}