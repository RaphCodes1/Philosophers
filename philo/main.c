#include "Philosophers.h"

void exit_err(char *s)
{
	printf("%s\n",s);
	exit(EXIT_FAILURE);
}
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

void wait_threads(t_philo *philo)
{
	while(!get_bool(&philo->program->table_mutex, 
		&philo->program->threads_ready))
		;
}
void *dinner_sim(void *data)
{	
	t_philo *philo;

	philo = (t_philo *)data;

	// //spinlock
	wait_threads(philo);

	//set last meal time
	while(!sim_finished(philo->program))
	{	
		//if full
		if(philo->full)
			break;
		
		//eating
		//sleeping
		write_status(SLEEPING, philo, DEBUG_MODE);
		prec_usleep(philo->program->time_to_sleep, philo->program);
		//thinking
	}
	return (NULL);
}

void creation_thread(t_prog *prog)
{	
	int i;

	i = -1;
	if(prog->num_times_to_eat == 0)
		return ;
	else if(prog->num_times_to_eat == 1)
		; // TODO 
	else
	{
		while(++i < prog->num_of_philos)
		{
			thread_handle(&prog->philos[i].thread_id, dinner_sim,
				&prog->philos[i], CREATE);
			printf("philo id: %d\n",prog->philos[i].id);
		}
	}
	prog->start_sim = get_time(MILLISECOND);
	set_bool(&prog->table_mutex, &prog->threads_ready, true);
	i = -1;
	while(++i < prog->num_of_philos)
		thread_handle(&prog->philos[i].thread_id, NULL, NULL, JOIN);


}
void *safe_malloc(int num_philo)
{
	void *res;

	res = malloc(num_philo);
	if(!res)
	{
		perror("malloc error!\n");
		// add a nuke function to free everything before exit of program
		exit(EXIT_FAILURE);
	}
	return (res);
}

int main(int ac, char **av)
{   
	t_philo 	*philos;
	t_prog		program;

    if(ac == 5 || ac == 6)
    {	
		if(!check_valid_args(av))
		{	
			av_input(&program, av);
			data_init(&program, av);
			creation_thread(&program);
			//add cleanup function for the threads
		}
		else
			printf("Invalid Args\n");
		
    }
    else
        printf("Invalid Args\n");
}