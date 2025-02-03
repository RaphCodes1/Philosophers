/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:12:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/03 16:46:47 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	which_philo_check(t_philo *philo)
{
	int	i;

	i = -1;
	if (philo->program->num_of_philos % 2 == 0)
	{
		lock_forks(philo);
		return (1);
	}
	else
	{
		while (++i <= philo->program->num_of_philos)
		{
			if (i == philo->id)
			{
				if (philo->program->eat_stat[i - 1] != philo->id
					&& philo->program->eat_stat[i] != philo->id)
				{
					lock_forks(philo);
					return (1);
				}
				return (0);
			}
		}
	}
	return (0);
}

void	lock_forks(t_philo *philo)
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

void	set_eat_stat(t_philo *philo)
{
	int	i;

	i = 0;
	while (++i <= philo->program->num_of_philos)
	{
		// mutex_handle(&philo->program->which_philo_eat_lock, LOCK);
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
		// mutex_handle(&philo->program->which_philo_eat_lock, UNLOCK);
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
		else if (i % 2 == 0)
			prog->eat_stat[i] = i + 2;
		else if (i % 2)
			prog->eat_stat[i] = i + 1;
	}
}
