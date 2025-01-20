#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>

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

	pthread_mutex_t *table_mtx;
	struct s_program 		*t_program;
}   t_philo;

typedef struct s_program
{
	int				dead_flag;

	int				start_sim;
	int				end_sim;
	int				sync_philos;
	pthread_mutex_t	table_mtx;

	pthread_mutex_t dead_lock;
	pthread_mutex_t meal_lock;
	pthread_mutex_t write_lock;
	t_philo			*philos;
}	t_program;

typedef enum op_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETATCH,
}	t_operation;

// helpers
int		ft_atoi(const char *nptr);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);

//init 
void philo_struct_int(t_philo **philo, t_program *program, 
	pthread_mutex_t *forks, char **av);
void program_init(t_program *program, t_philo *philos);
void forks_init(pthread_mutex_t **forks, int num_of_philos);
void int_argv_content(t_philo *philo, char **av);

//handlers and mallocs
void mutex_handle(pthread_mutex_t *mutex, t_operation opcode);
void thread_handle(pthread_t *thread, void *(*func)(void *), 
	void *data, t_operation opcode);
void *safe_malloc(int num_philo);

//locks and unlocks
void set_val(pthread_mutex_t **mutex, int *dest, int value);
int get_val(pthread_mutex_t **mutex, int *val);
// int sim_finished(t_program *program);