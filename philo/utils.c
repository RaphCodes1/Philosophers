/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:00:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 20:15:15 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

long	get_time(t_time t_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	if (t_code == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (t_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (t_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		return (0);
	return (0);
}

void	prec_usleep(long usec, t_prog *prog)
{
	long	start;

	start = get_time(MILLISECOND);
	while (get_time(MILLISECOND) - start < usec)
	{
		if (sim_finished(prog))
			break ;
		usleep(500);
	}
}

void	write_stat_debug(t_philo_stat status, t_philo *philo, long elapsed)
{
	if ((TAKE_R_FORK == status && !sim_finished(philo->program)))
		printf("%-6ld %d has taken R fork %d\n", elapsed, philo->id,
			philo->r_fork->fork_id);
	else if ((TAKE_L_FORK == status && !sim_finished(philo->program)))
		printf("%-6ld %d has taken L fork %d\n", elapsed, philo->id,
			philo->l_fork->fork_id);
	else if (EATING == status && !sim_finished(philo->program))
		printf(BLUE "%-6ld %d is eating\n" RESET, elapsed, philo->id);
	else if (SLEEPING == status && !sim_finished(philo->program))
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if (THINKING == status && !sim_finished(philo->program))
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if (DIED == status && !sim_finished(philo->program))
		printf(RED "%-6ld %d has died\n" RESET, elapsed, philo->id);
}

void	gs_logs(t_philo *philo, int id, char *msg)
{
	pthread_mutex_lock(&philo->program->write_lock);
	// if (check_dead(phdata))
	// {
	// 	pthread_mutex_unlock(&phdata->print);
	// 	return (1);
	// }
	printf("%ld %d %s\n", get_time(MILLISECOND) - philo->program->start_sim, id, msg);
	pthread_mutex_unlock(&philo->program->write_lock);
	// return (0);
}

void	write_status(t_philo_stat status, t_philo *philo)
{
	// long	elapsed;

	// elapsed = get_time(MILLISECOND) - philo->program->start_sim;
	mutex_handle(&philo->program->write_lock, LOCK);
	if ((TAKE_R_FORK == status || TAKE_L_FORK == status)
		&& !sim_finished(philo->program))
		printf(WHITE "%-6ld" RESET "%d has taken a fork\n", get_time(MILLISECOND) - philo->program->start_sim, philo->id);
	else if (EATING == status && !sim_finished(philo->program))
		printf(BLUE "%-6ld%d is eating\n" RESET, get_time(MILLISECOND) - philo->program->start_sim, philo->id);
	else if (SLEEPING == status && !sim_finished(philo->program))
		printf(WHITE "%-6ld%d is sleeping\n" RESET, get_time(MILLISECOND) - philo->program->start_sim, philo->id);
	else if (THINKING == status && !sim_finished(philo->program))
		printf(WHITE "%-6ld" RESET "%d is thinking\n", get_time(MILLISECOND) - philo->program->start_sim, philo->id);
	else if (DIED == status && !sim_finished(philo->program))
		printf(RED "%-6ld%d died\n" RESET, get_time(MILLISECOND) - philo->program->start_sim, philo->id);
	mutex_handle(&philo->program->write_lock, UNLOCK);
}
