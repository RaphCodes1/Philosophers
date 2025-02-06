/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:36 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/06 17:15:38 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

bool	philo_dead(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	elapsed = gs_time() - get_val(&philo->philo_mutex,
			&philo->last_meal_time);
	time_to_die = philo->program->time_to_die;
	if (elapsed > time_to_die)
		return (true);
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
		if (philo[i].meal_count == philo->program->num_times_to_eat)
			check++;
		mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	}
	if (check == philo->program->num_of_philos)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	t_prog	*prog;
	int		i;

	prog = (t_prog *)data;
	// while (!threads_run_check(&prog->table_mutex, prog->num_of_philos,
	// 		&prog->threads_running_nbr))
	// 	;
	while (!sim_finished(prog))
	{
		i = 0;
		while (i < prog->num_of_philos)
		{
			if (philo_dead(&prog->philos[i]))
			{
				write_status(DIED, &prog->philos[i]);
				set_bool(&prog->table_mutex, &prog->end_sim, true);
			}
			i++;
		}
		if (philo_full_check(prog->philos))
			set_bool(&prog->table_mutex, &prog->end_sim, true);
		// usleep(100);
	}
	return (NULL);
}
