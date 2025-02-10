/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:12:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/10 20:18:55 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	which_philo_check(t_philo *philo)
{
	int	i;
	int	l_fork_pos;
	int	r_fork_pos;

	i = -1;
	r_fork_pos = philo->r_fork->fork_id;
	l_fork_pos = philo->l_fork->fork_id;
	while (++i <= philo->program->num_of_philos)
	{
		if (i == philo->id)
		{
			if (philo->program->eat_stat[l_fork_pos] != philo->id
				&& philo->program->eat_stat[r_fork_pos] != philo->id)
			{
				// lock_forks(philo);
				return (1);
			}
			return (0);
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

	i = -1;
	while (++i <= philo->program->num_of_philos)
	{
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
