/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:36 by rcreer            #+#    #+#             */
/*   Updated: 2025/01/27 17:59:37 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

bool	philo_dead(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	elapsed = get_time(MILLISECOND) - get_val(&philo->philo_mutex,
			&philo->last_meal_time);
	time_to_die = philo->program->time_to_die / 1000;
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

bool	all_philo_full(t_prog *prog)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = prog->philos;
	while (++i < prog->num_of_philos)
	{
		if (!get_bool(&philo->philo_mutex, &philo->full))
			return (false);
	}
	return (true);
}

void	*monitor_dinner(void *data)
{
	t_prog	*prog;
	int		i;

	prog = (t_prog *)data;
	while (!threads_run_check(&prog->table_mutex, prog->num_of_philos,
			&prog->threads_running_nbr))
		;
	while (!sim_finished(prog))
	{
		i = 0;
		while (i < prog->num_of_philos && !sim_finished(prog))
		{
			if (all_philo_full(prog))
				set_bool(&prog->table_mutex, &prog->end_sim, true);
			if (philo_dead(&prog->philos[i]))
			{
				write_status(DIED, &prog->philos[i]);
				set_bool(&prog->table_mutex, &prog->end_sim, true);
			}
			i++;
		}
	}
	return (NULL);
}
