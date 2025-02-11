/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:12:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 17:23:33 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	order_picked(t_philo *philo, int *first, int *second)
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
	int f1;
	int f2;
	int f1_check;
	int f2_check;

	order_picked(philo, &f1, &f2);
	mutex_handle(&philo->program->forks[f1], LOCK);
	f1_check = philo->program->eat_stat[f1];
	mutex_handle(&philo->program->forks[f1], UNLOCK);
	mutex_handle(&philo->program->forks[f2], LOCK);
	f2_check = philo->program->eat_stat[f2];
	mutex_handle(&philo->program->forks[f2], UNLOCK);
	if(f1_check != philo->id && f2_check != philo->id)
		return (1);
	return (0);
}

void	lock_forks(t_philo *philo)
{
	int f1;
	int f2;
	order_picked(philo, &f1, &f2);
	mutex_handle(&philo->program->forks[f1], LOCK);
	philo->program->eat_stat[f1] = 0;
	mutex_handle(&philo->program->forks[f2], LOCK);
	philo->program->eat_stat[f2] = 0;
}

void	down_forks(t_philo *philo)
{	
	int f1;
	int	f2;
	order_picked(philo, &f1, &f2);
	philo->program->eat_stat[f1] = philo->id;
	philo->program->eat_stat[f2] = philo->id;
	mutex_handle(&philo->program->forks[f1], UNLOCK);
	mutex_handle(&philo->program->forks[f2], UNLOCK);
}

void	eat_stat_init(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i < prog->num_of_philos)
	{
		if (i == 0 || i == prog->num_of_philos - 1)
			prog->eat_stat[i] = prog->num_of_philos;
		else if (i % 2 == 0)
			prog->eat_stat[i] = i + 2;
		else if (i % 2)
			prog->eat_stat[i] = i + 1;
	}
}
