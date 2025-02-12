/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:36 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 17:23:17 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

bool	philo_dead(t_philo *philo)
{
	mutex_handle(&philo->program->table_mutex, LOCK);
	if (get_time(MILLISECOND) - get_val(&philo->program->philo_full_mutex,&philo->last_meal_time)
		> philo->program->time_to_die)
	{
		set_bool(&philo->program->dead_mutex, &philo->program->end_sim, true);
		mutex_handle(&philo->program->write_lock, LOCK);
		printf(RED "%-6ld%d died\n" RESET,
			get_time(MILLISECOND) - philo->program->start_sim, philo->id);
		mutex_handle(&philo->program->write_lock, UNLOCK);
		mutex_handle(&philo->program->table_mutex, UNLOCK);
		return (true);
	}
	mutex_handle(&philo->program->table_mutex, UNLOCK);
	return (false);
}

bool	philo_full_check(t_philo *philo)
{
	int	i;
	int	check;

	check = 0;
	i = -1;
	while (++i < philo->program->num_of_philos)
	{
		mutex_handle(&philo->program->philo_full_mutex, LOCK);
		if (philo[i].meal_count >= philo->program->num_times_to_eat
			&& philo->program->num_times_to_eat != -1)
			check++;
		mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	}
	if (check >= philo->program->num_of_philos)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	t_prog	*prog;
	int		i;

	prog = (t_prog *)data;
	while (!sim_finished(prog))
	{
		i = 0;
		while (i < prog->num_of_philos)
		{
			if (philo_dead(&prog->philos[i]))
			{
				return (NULL);
			}
			i++;
		}
		if (philo_full_check(prog->philos))
		{
			set_bool(&prog->dead_mutex, &prog->end_sim, true);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
