/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:46 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 17:23:23 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	eat(t_philo *philo)
{
	if (sim_finished(philo->program))
		return (0);
	if (!sim_finished(philo->program))
	{
		if(!write_status(TAKE_L_FORK, philo))
			return (0);
		if(!write_status(TAKE_R_FORK, philo))
			return (0);
		if(!write_status(EATING, philo))
			return (0);
	}
	else
		return (0);
	if (!prec_usleep(philo->program->time_to_eat, philo->program))
		return (0);
	down_forks(philo);
	if(!sim_finished(philo->program))
	{
		mutex_handle(&philo->program->philo_full_mutex, LOCK);
		philo->meal_count++;
		philo->last_meal_time = get_time(MILLISECOND);
		mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	}
	return (1);
}

int	think(t_philo *philo)
{
	if (sim_finished(philo->program))
		return (0);
	if(!write_status(THINKING, philo))
		return (0);
	return (1);
}

int	sleeping(t_philo *philo)
{
	if (sim_finished(philo->program))
		return (0);
	if(!write_status(SLEEPING, philo))
		return (0);
	if (!prec_usleep(philo->program->time_to_sleep, philo->program))
		return (0);
	return (1);
}

void	*one_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	write_status(TAKE_R_FORK, philo);
	while (!sim_finished(philo->program))
		usleep(200);
	return (NULL);
}

void	*dinner_sim(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (!sim_finished(philo->program))
	{
		if (which_philo_check(philo))
		{
			lock_forks(philo);
			if (!eat(philo))
			{
				down_forks(philo);
				return (NULL);
			}
			if (!sleeping(philo))
				return (NULL);
			if(!think(philo))
				return (NULL);
		}
	}
	return (NULL);
}
