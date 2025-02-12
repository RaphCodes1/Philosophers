/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:12:02 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/05 19:56:12 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	which_philo_check(t_philo *philo)
{
	int	l_fork_pos;
	int	r_fork_pos;

	mutex_handle(&philo->r_fork->fork, LOCK);
	r_fork_pos = philo->r_fork->fork_id;
	mutex_handle(&philo->r_fork->fork, UNLOCK);
	mutex_handle(&philo->r_fork->fork, LOCK);
	l_fork_pos = philo->l_fork->fork_id;
	mutex_handle(&philo->r_fork->fork, UNLOCK);
	if (philo->program->eat_stat[l_fork_pos] != philo->id
		&& philo->program->eat_stat[r_fork_pos] != philo->id)
	{
		// lock_forks(philo);
		return (1);
	}
	return (0);
}

void	lock_forks(t_philo *philo)
{	
	if (philo->id % 2 == 0)
	{
		mutex_handle(&philo->r_fork->fork, LOCK);
		mutex_handle(&philo->l_fork->fork, LOCK);
		// gs_logs(philo, philo->id, "has taken a fork");
		// gs_logs(philo, philo->id, "has taken a fork");
		write_status(TAKE_R_FORK, philo);
		write_status(TAKE_L_FORK, philo);
	}
	else if (philo->id % 2)
	{
		mutex_handle(&philo->l_fork->fork, LOCK);
		mutex_handle(&philo->r_fork->fork, LOCK);
		// gs_logs(philo, philo->id, "has taken a fork");
		// gs_logs(philo, philo->id, "has taken a fork");
		write_status(TAKE_L_FORK, philo);
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
		else if (i % 2 == 0 && i + 2 < prog->num_of_philos)
			prog->eat_stat[i] = i + 2;
		else if (i % 2 && i + 1 < prog->num_of_philos)
			prog->eat_stat[i] = i + 1;
	}
}
