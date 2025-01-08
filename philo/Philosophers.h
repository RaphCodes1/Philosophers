#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

typedef struct s_philo
{
    pthread_t	thread;
	int			num_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_times_to_eat;
}   t_philo;
// helpers
int	ft_atoi(const char *nptr);
size_t	ft_strlen(const char *s);
