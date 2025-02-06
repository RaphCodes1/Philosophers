/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:46 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 20:19:13 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void 	eat(t_philo *philo)
{
	// if(!sim_finished(philo->program))
	// 	return (1);
	write_status(TAKE_L_FORK, philo);
	write_status(TAKE_R_FORK, philo);
	write_status(EATING, philo);
	prec_usleep(philo->program->time_to_eat, philo->program);
	// gs_sleep(philo->program->time_to_eat, philo);
	forks_down(philo);
	mutex_handle(&philo->program->philo_full_mutex, LOCK);
	philo->meal_count++;
	mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	set_val(&philo->program->table_mutex, &philo->last_meal_time, get_time(MILLISECOND));

	// lock_forks(philo);
	// write_status(EATING, philo);
	// // gs_logs(philo, philo->id, "is eating");
	// prec_usleep(philo->program->time_to_eat, philo->program);
	// mutex_handle(&philo->program->philo_full_mutex, LOCK);
	// philo->meal_count++;
	// mutex_handle(&philo->program->philo_full_mutex, UNLOCK);
	// set_val(&philo->program->table_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	// if (philo->id % 2 == 0)
	// {
	// 	mutex_handle(&philo->r_fork->fork, UNLOCK);
	// 	mutex_handle(&philo->l_fork->fork, UNLOCK);
	// }
	// else if (philo->id % 2)
	// {
	// 	mutex_handle(&philo->l_fork->fork, UNLOCK);
	// 	mutex_handle(&philo->r_fork->fork, UNLOCK);
	// }
	// mutex_handle(&philo->r_fork->fork, LOCK);
	// mutex_handle(&philo->l_fork->fork, LOCK);
	// philo->program->eat_stat[philo->l_fork->fork_id] = philo->id;
	// philo->program->eat_stat[philo->r_fork->fork_id] = philo->id;
	// mutex_handle(&philo->r_fork->fork, UNLOCK);
	// mutex_handle(&philo->l_fork->fork, UNLOCK);
}

void	think(t_philo *philo, bool pre_sim)
{
	if (!pre_sim)
		write_status(THINKING, philo);
		// gs_logs(philo, philo->id, "is thinking");
	// if (philo->program->num_of_philos % 2 == 0)
	// 	return ;
	// else
	// 	prec_usleep(100, philo->program);
	// prec_usleep(100, philo->program);
}

void	sleeping(t_philo *philo)
{
	write_status(SLEEPING, philo);
	// gs_logs(philo, philo->id, "is sleeping");
	prec_usleep(philo->program->time_to_sleep, philo->program);
	// gs_sleep(philo->program->time_to_eat, philo);
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
	// desync_philo(philo);
	while (!sim_finished(philo->program))
	{
		// mutex_handle(&philo->program->which_philo_eat_lock, LOCK);
		// check = which_philo_check(philo);
		// mutex_handle(&philo->program->which_philo_eat_lock, UNLOCK);
		if (which_philo_check(philo))
		{	
			lock_forks(philo);
			eat(philo);
			sleeping(philo);
			think(philo, false);
		}
	}
	return (NULL);
}
