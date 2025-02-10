/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:46 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/10 19:50:43 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	eat(t_philo *philo)
{
	write_status(EATING, philo);
	prec_usleep(philo->program->time_to_eat, philo->program);
	mutex_handle(&philo->program->philo_full_mutex, LOCK);
	philo->meal_count++;
	mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	set_val(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	if (philo->program->num_times_to_eat > 0
		&& philo->meal_count == philo->program->num_times_to_eat)
		set_bool(&philo->philo_mutex, &philo->full, true);
	mutex_handle(&philo->r_fork->fork, UNLOCK);
	mutex_handle(&philo->l_fork->fork, UNLOCK);
	mutex_handle(&philo->program->which_philo_eat_lock, LOCK);
	set_eat_stat(philo);
	mutex_handle(&philo->program->which_philo_eat_lock, UNLOCK);
}

void	think(t_philo *philo, bool pre_sim)
{
	if (!pre_sim)
		write_status(THINKING, philo);
}

void	sleeping(t_philo *philo)
{
	write_status(SLEEPING, philo);
	prec_usleep(philo->program->time_to_sleep, philo->program);
}

void	*one_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_threads(philo);
	set_val(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	increase_val(&philo->program->table_mutex,
		&philo->program->threads_running_nbr);
	write_status(TAKE_R_FORK, philo);
	while (!sim_finished(philo->program))
		usleep(200);
	return (NULL);
}

void	*dinner_sim(void *data)
{
	t_philo	*philo;
	int		check;

	check = 0;
	philo = (t_philo *)data;
	wait_threads(philo);
	set_val(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	increase_val(&philo->program->table_mutex,
		&philo->program->threads_running_nbr);
	while (!sim_finished(philo->program))
	{
		mutex_handle(&philo->program->which_philo_eat_lock, LOCK);
		check = which_philo_check(philo);
		mutex_handle(&philo->program->which_philo_eat_lock, UNLOCK);
		if (check)
		{
			eat(philo);
			sleeping(philo);
			think(philo, false);
		}
	}
	return (NULL);
}
