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

void mutex_err(int status, t_operation opcode)
{
	if(status == 0)
		return ;
	if(EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		ft_putstr_fd("The value specified by mutex is invalid.\n", 2);
	else if(EINVAL == status && INIT == opcode)
		ft_putstr_fd("The value specified by attr is invalid.\n", 2);
	else if(EDEADLK == status)
	{
		ft_putstr_fd("A deadlock would occur if the thread ", 2);
		ft_putstr_fd("blocked waiting for mutex.\n", 2);
	}
	else if(EPERM == status)
		ft_putstr_fd("The current thread does not hold a lock on mutex\n", 2);
	else if(ENOMEM == status)
	{
		ft_putstr_fd("The process cannot allocate ", 2);
		ft_putstr_fd("enough memory to create another mutex.\n", 2);

	}
	else if(EBUSY == status)
		ft_putstr_fd("Mutex is locked\n", 2);
}

void mutex_handle(pthread_mutex_t *mutex, t_operation opcode)
{
	if(opcode == INIT)
		mutex_err(pthread_mutex_init(mutex, NULL), INIT);
	else if(opcode == LOCK)
		mutex_err(pthread_mutex_lock(mutex), LOCK);
	else if(opcode == UNLOCK)
		mutex_err(pthread_mutex_unlock(mutex), UNLOCK);
	else if(opcode == DESTROY)
		mutex_err(pthread_mutex_destroy(mutex), DESTROY);
	else
		printf("wrong opcode for mutex_handling\n");
}

void thread_err(int status, t_operation opcode)
{
	if(status == 0)
		return ;
	if(EAGAIN == status)
		ft_putstr_fd("No resources to create another thread\n.", 2);
	else if(EINVAL == status && opcode == CREATE)
		ft_putstr_fd("The caller does not have appropiate permission\n.", 2);
	else if(EINVAL == status && (opcode == JOIN || opcode == DETATCH))
		ft_putstr_fd("The value specified by thread is not joinable\n", 2);
	else if(ESRCH == status)
	{
		ft_putstr_fd("No thread could be found corresponding to that", 2);
		ft_putstr_fd("specified by the given thread ID, thread.\n", 2);
	}
	else if(EDEADLK == status)
	{
		ft_putstr_fd("A deadlock was detected or the value", 2);
		ft_putstr_fd("of thread specifies the calling thread.\n", 2);
	}
}

void thread_handle(pthread_t *thread, void *(*func)(void *), 
	void *data, t_operation opcode)
{
	if(opcode == CREATE)
		thread_err(pthread_create(thread, NULL, func, data), CREATE);
	else if(opcode == JOIN)
		thread_err(pthread_join(*thread, NULL), JOIN);
	else if(opcode == DETATCH)
		thread_err(pthread_detach(*thread), DETATCH);
	else
		printf("wrong opcode for pthread_handling\n");
}
void wait_threads(t_philo *philo)
{
	while(!get_val(&philo->table_mtx, &philo->t_program->sync_philos))
		;
}
void *dinner_sim(void *data)
{	
	t_philo *program;

	program = (t_philo *)data;

	//spinlock
	printf("bruh\n");
	printf("sync val: %d\n",program->t_program->sync_philos);
	wait_threads(program);
	return(NULL);
}

void creation_thread(t_program *program, pthread_mutex_t *forks)
{
	t_philo *philo;
	int i;

	philo = program->philos;
	i = -1;

	if(philo->num_of_philos == 0)
		return ;
	else if(philo->num_of_philos == 1)
		return ; // TODO
	while(++i < philo->num_of_philos)
	{
		thread_handle(&philo[i].thread, dinner_sim, &philo[i], CREATE);
		printf("id: %d\n",philo[i].id);
	}
	//threads ready
	set_val(&program->philos->table_mtx, &program->sync_philos, 1);

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
			forks_init(&forks, ft_atoi(av[1]));
			philo_struct_int(&philos, &program, forks, av);
			program_init(&program,philos);
			creation_thread(&program, forks);
			//add cleanup function for the threads
		}
		else
			printf("Invalid Args\n");
		
    }
    else
        printf("Invalid Args\n");
}