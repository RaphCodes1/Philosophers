#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>
#include <stdbool.h>

#define RESET       "\033[0m"  // Reset to default
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

/*write function macro*/
#define DEBUG_MODE 0

typedef struct s_prog t_prog;
typedef struct s_fork
{
	pthread_mutex_t fork;
	int fork_id;

}	t_fork;

typedef struct s_philo
{
	int id;
	pthread_t thread_id;
	long meal_count;
	long last_meal_time;
	bool full;
	t_fork *l_fork;
	t_fork *r_fork;
	t_prog *program;
	pthread_mutex_t philo_mutex;
}	t_philo;

struct s_prog
{
	long 			num_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			num_times_to_eat; // -1 if not in argv
	long			start_sim;
	bool			end_sim;
	bool			threads_ready; //sync philo
	pthread_mutex_t table_mutex; //avoid races while reading from the program
	pthread_mutex_t write_lock; // for the table dinner sim (eat, sleep, think)
	t_fork			*forks;
	t_philo			*philos;
};

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

typedef enum time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time;

typedef enum philo_stat
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_R_FORK,
	TAKE_L_FORK,
	DIED,
}	t_philo_stat;

// helpers
int		ft_atoi(const char *nptr);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
unsigned long long	ft_atol(char *s);

void 	exit_err(char *s);

//init 
void assign_forks(t_philo *philo, t_fork *forks, int curr_pos);
void philo_init(t_prog *prog);
void data_init(t_prog *prog, char **av);
void av_input(t_prog *prog, char **av);

//handlers and mallocs
void mutex_handle(pthread_mutex_t *mutex, t_operation opcode);
void thread_handle(pthread_t *thread, void *(*func)(void *), 
	void *data, t_operation opcode);
void *safe_malloc(int num_philo);

//locks and unlocks
void set_val(pthread_mutex_t *mutex, long *dest, int value);
int get_val(pthread_mutex_t *mutex, long *val);
void set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
int get_bool(pthread_mutex_t *mutex, bool *val);
bool sim_finished(t_prog *prog);

//time, prec_usleep
long get_time(t_time t_code);
void prec_usleep(long usec, t_prog *prog);

//write status
void write_status(t_philo_stat status, t_philo *philo, bool debug);