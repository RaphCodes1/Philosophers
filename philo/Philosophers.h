/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:43 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/03 16:09:01 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// /*write function macro*/
// #define DEBUG_MODE 0

typedef struct s_prog	t_prog;
typedef struct s_fork
{
	pthread_mutex_t		fork;
	int					fork_id;

}						t_fork;

typedef struct s_philo
{
	int					id;
	pthread_t			thread_id;
	long				meal_count;
	long				last_meal_time;
	bool				full;
	t_fork				*l_fork;
	t_fork				*r_fork;
	t_prog				*program;
	pthread_mutex_t		philo_mutex;
}						t_philo;

struct					s_prog
{
	long				num_of_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				num_times_to_eat; // -1 if not in argv
	long				start_sim;
	bool				end_sim;
	bool				threads_ready;
	pthread_mutex_t		table_mutex;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		which_philo_eat_lock;
	pthread_t			monitor;
	long				threads_running_nbr;
	int					*eat_stat;
	t_fork				*forks;
	t_philo				*philos;
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
}						t_operation;

typedef enum time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}						t_time;

typedef enum philo_stat
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_R_FORK,
	TAKE_L_FORK,
	DIED,
}						t_philo_stat;

// helpers
int						ft_atoi(const char *nptr);
size_t					ft_strlen(const char *s);
void					ft_putstr_fd(char *s, int fd);
unsigned long long		ft_atol(char *s);

// init
void					assign_forks(t_philo *philo, t_fork *forks,
							int curr_pos);
void					philo_init(t_prog *prog);
int						data_init(t_prog *prog, char **av);
int						av_input(t_prog *prog, char **av);

// handlers and mallocs
void					mutex_handle(pthread_mutex_t *mutex,
							t_operation opcode);
void					thread_handle(pthread_t *thread, void *(*func)(void *),
							void *data, t_operation opcode);
int						malloc_check(t_prog *prog);

// locks and unlocks
void					set_val(pthread_mutex_t *mutex, long *dest, long value);
long					get_val(pthread_mutex_t *mutex, long *val);
void					set_bool(pthread_mutex_t *mutex, bool *dest,
							bool value);
bool					get_bool(pthread_mutex_t *mutex, bool *val);
bool					sim_finished(t_prog *prog);

// time, prec_usleep
long					get_time(t_time t_code);
void					prec_usleep(long usec, t_prog *prog);

// write status
void					write_status(t_philo_stat status, t_philo *philo);

// sync
void					wait_threads(t_philo *philo);
bool					threads_run_check(pthread_mutex_t *mutex,
							long philo_num, long *threads);
void					increase_val(pthread_mutex_t *mutex, long *val);
void					desync_philo(t_philo *philo);

// philo status
void					think(t_philo *philo, bool pre_sim);
void					eat(t_philo *philo);
void					*one_philo(void *data);
void					*dinner_sim(void *data);
void					sleeping(t_philo *philo);
void					set_eat_stat(t_philo *philo);
int						which_philo_check(t_philo *philo);
void					lock_forks(t_philo *philo);
void					eat_stat_init(t_prog *prog);

// monitor
void					*monitor_dinner(void *data);
bool					all_philo_full(t_prog *prog);