#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

typedef struct s_philo
{
    pthread_t	thread;
	int 		id;
	int			eating;
	int			meals_eaten;
	int			last_meal;
	int			num_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_times_to_eat;
	int			start_time;
	int			*dead;
	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *write_lock;
	pthread_mutex_t *dead_lock;
	pthread_mutex_t *meal_lock;
}   t_philo;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t dead_lock;
	pthread_mutex_t meal_lock;
	pthread_mutex_t write_lock;
	t_philo			*philos;
}	t_program;

// helpers
int	ft_atoi(const char *nptr);
size_t	ft_strlen(const char *s);
