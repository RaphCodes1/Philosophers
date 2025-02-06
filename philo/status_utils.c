/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:12:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/06 17:17:39 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

unsigned long	gs_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		exit(1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	gs_sleep(unsigned long time, t_philo *philo)
{
	unsigned long	start;

	start = gs_time();
	while (gs_time() - start < time)
	{
		if (sim_finished(philo->program))
			return (1);
		usleep(500);
	}
	return (0);
}

void	order_pick(t_philo *philo, int *first, int *second)
{
	if (philo->l_fork < philo->r_fork)
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
	else
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
}

int	which_philo_check(t_philo *philo)
{
	int	first;
	int	second;
	int f1_open;
	int f2_open;

	order_pick(philo, &first, &second);
	mutex_handle(&philo->program->forks_mutex[first], LOCK);
	f1_open = philo->program->eat_stat[first];
	mutex_handle(&philo->program->forks_mutex[first], UNLOCK);
	mutex_handle(&philo->program->forks_mutex[second], LOCK);
	f1_open = philo->program->eat_stat[second];
	mutex_handle(&philo->program->forks_mutex[second], UNLOCK);
	if (f1_open != philo->id && f2_open != philo->id)
		return (1);
	return (0);
	// mutex_handle(&philo->r_fork->fork, LOCK);
	// r_fork_pos = philo->r_fork->fork_id;
	// mutex_handle(&philo->r_fork->fork, UNLOCK);
	// mutex_handle(&philo->r_fork->fork, LOCK);
	// l_fork_pos = philo->l_fork->fork_id;
	// mutex_handle(&philo->r_fork->fork, UNLOCK);
	// if (philo->program->eat_stat[l_fork_pos] != philo->id
	// 	&& philo->program->eat_stat[r_fork_pos] != philo->id)
	// {
	// 	// lock_forks(philo);
	// 	return (1);
	// }
	// return (0);
}

void	forks_down(t_philo *philo)
{
	int f1;
	int f2;
	order_pick(philo, &f1, &f2);
	philo->program->eat_stat[f1] = philo->id;
	philo->program->eat_stat[f2] = philo->id;
	mutex_handle(&philo->program->forks_mutex[f1], UNLOCK);
	mutex_handle(&philo->program->forks_mutex[f2], UNLOCK);
}
void	lock_forks(t_philo *philo)
{	

	int f1;
	int f2;
	order_pick(philo, &f1, &f2);
	mutex_handle(&philo->program->forks_mutex[f1], LOCK);
	philo->program->eat_stat[f1] = 0;
	mutex_handle(&philo->program->forks_mutex[f2], LOCK);
	philo->program->eat_stat[f2] = 0;
	// if (philo->id % 2 == 0)
	// {
	// 	mutex_handle(&philo->r_fork->fork, LOCK);
	// 	mutex_handle(&philo->l_fork->fork, LOCK);
	// 	// gs_logs(philo, philo->id, "has taken a fork");
	// 	// gs_logs(philo, philo->id, "has taken a fork");
	// 	write_status(TAKE_R_FORK, philo);
	// 	write_status(TAKE_L_FORK, philo);
	// }
	// else if (philo->id % 2)
	// {
	// 	mutex_handle(&philo->l_fork->fork, LOCK);
	// 	mutex_handle(&philo->r_fork->fork, LOCK);
	// 	// gs_logs(philo, philo->id, "has taken a fork");
	// 	// gs_logs(philo, philo->id, "has taken a fork");
	// 	write_status(TAKE_L_FORK, philo);
	// 	write_status(TAKE_R_FORK, philo);
	// }
}

void	set_eat_stat(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i <= philo->program->num_of_philos)
	{
		if (i == philo->id)
		{
			if (i == philo->program->num_of_philos)
			{
				philo->program->eat_stat[i - 1] = philo->program->num_of_philos;
				philo->program->eat_stat[0] = philo->program->num_of_philos;
			}
			else
			{
				philo->program->eat_stat[i - 1] = philo->id;
				philo->program->eat_stat[i] = philo->id;
			}
		}
	}
}

void	eat_stat_init(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i < prog->num_of_philos)
	{
		if (i == 0 || i == prog->num_of_philos - 1)
			prog->eat_stat[i] = prog->num_of_philos;
		else if (i % 2 == 0 && i + 2 < prog->num_of_philos)
			prog->eat_stat[i] = i + 2;
		else if (i % 2 && i + 1 < prog->num_of_philos)
			prog->eat_stat[i] = i + 1;
	}
}
