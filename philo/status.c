/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:46 by rcreer            #+#    #+#             */
/*   Updated: 2025/01/27 19:17:11 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	set_eat_stat(t_philo *philo)
{	
	int i;

	i = 0;
	while(++i <= philo->program->num_of_philos)
	{	
		mutex_handle(&philo->program->table_mutex, LOCK);
		if(i == philo->id)
		{	
			if(i == philo->program->num_of_philos)
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
		mutex_handle(&philo->program->table_mutex, UNLOCK);
	}
}
void	eat(t_philo *philo)
{
	write_status(EATING, philo);
	prec_usleep(philo->program->time_to_eat, philo->program);
	philo->meal_count++;
	set_val(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	if (philo->program->num_times_to_eat > 0
		&& philo->meal_count == philo->program->num_times_to_eat)
		set_bool(&philo->philo_mutex, &philo->full, true);
	mutex_handle(&philo->r_fork->fork, UNLOCK);
	mutex_handle(&philo->l_fork->fork, UNLOCK);
	set_eat_stat(philo);
}

void	think(t_philo *philo, bool pre_sim)
{
	if (!pre_sim)
		write_status(THINKING, philo);
	if (philo->program->num_of_philos % 2 == 0)
		return ;
	else
		prec_usleep(2000, philo->program);
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

void lock_forks(t_philo *philo)
{	
	if (philo->id % 2 == 0)
	{
		mutex_handle(&philo->r_fork->fork, LOCK);
		write_status(TAKE_R_FORK, philo);
		mutex_handle(&philo->l_fork->fork, LOCK);
		write_status(TAKE_L_FORK, philo);
	}
	else if (philo->id % 2)
	{
		mutex_handle(&philo->l_fork->fork, LOCK);
		write_status(TAKE_L_FORK, philo);
		mutex_handle(&philo->r_fork->fork, LOCK);
		write_status(TAKE_R_FORK, philo);
	}
}

int which_philo_check(t_philo *philo)
{
	int i;

	i = -1;
	if(philo->program->num_of_philos % 2 == 0)
	{
		lock_forks(philo);
		return (1);
	}
	else
	{
		while(++i <= philo->program->num_of_philos)
		{	
			if(i == philo->id)
			{
				if(philo->program->eat_stat[i - 1] != philo->id
					&& philo->program->eat_stat[i] != philo->id)
				{
					lock_forks(philo);
					return(1);
				}
				return(0);
			}
		}
	}
	return(0);
}


void	*dinner_sim(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_threads(philo);
	set_val(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	increase_val(&philo->program->table_mutex,
		&philo->program->threads_running_nbr);
	desync_philo(philo);
	while (!sim_finished(philo->program))
	{
		if (get_bool(&philo->philo_mutex, &philo->full))
			break ;
		if(which_philo_check(philo))
		{
			eat(philo);
			sleeping(philo);
			think(philo, false);
		}
	}
	return (NULL);
}
