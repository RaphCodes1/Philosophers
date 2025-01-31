/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:17 by rcreer            #+#    #+#             */
/*   Updated: 2025/01/27 17:59:18 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	assign_forks(t_philo *philo, t_fork *forks, int curr_pos)
{
	int	philo_nbr;

	philo_nbr = philo->program->num_of_philos;
	philo[curr_pos].r_fork = &forks[(curr_pos + 1) % philo_nbr];
	philo[curr_pos].l_fork = &forks[curr_pos];
	if (philo->id % 2 == 0)
	{
		philo[curr_pos].r_fork = &forks[curr_pos];
		philo[curr_pos].l_fork = &forks[(curr_pos + 1) % philo_nbr];
	}
}

void	philo_init(t_prog *prog)
{
	int		i;
	t_philo	*philo;

	i = -1;
	philo = prog->philos;
	while (++i < prog->num_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].full = false;
		philo[i].meal_count = 0;
		philo[i].program = prog;
		assign_forks(philo, prog->forks, i);
		mutex_handle(&philo[i].philo_mutex, INIT);
	}
}
void eat_stat_init(t_prog *prog)
{
	int i;

	i = -1;
	while(++i < prog->num_of_philos)
	{
		if(i == 0 || i == prog->num_of_philos - 1)
			prog->eat_stat[i] = prog->num_of_philos;
		else if(i % 2 == 0)
			prog->eat_stat[i] = i + 2;
		else if(i % 2)
			prog->eat_stat[i] = i + 1;
	}
	i = -1;
}

int	data_init(t_prog *prog, char **av)
{
	int	i;

	i = -1;
	if (!malloc_check(prog))
		return (0);
	prog->end_sim = false;
	prog->threads_ready = false;
	prog->threads_running_nbr = 0;
	mutex_handle(&prog->table_mutex, INIT);
	mutex_handle(&prog->write_lock, INIT);
	while (++i < prog->num_of_philos)
	{
		mutex_handle(&prog->forks[i].fork, INIT);
		prog->forks[i].fork_id = i;
	}
	eat_stat_init(prog);
	philo_init(prog);
	return (1);
}

int	valid_numbers(char **av)
{
	if (ft_atol(av[1]) > 200)
	{
		printf(RED "no more than 200 philos\n" RESET);
		return (0);
	}
	if (ft_atol(av[2]) > INT_MAX || ft_atol(av[3]) > INT_MAX
		|| ft_atol(av[4]) > INT_MAX)
	{
		printf(RED "error args\n" RESET);
		return (0);
	}
	if (av[5])
	{
		if (ft_atol(av[5]) > INT_MAX)
		{
			printf(RED "error args\n" RESET);
			return (0);
		}
	}
	return (1);
}

int	av_input(t_prog *prog, char **av)
{
	prog->num_of_philos = ft_atol(av[1]);
	prog->time_to_die = ft_atol(av[2]) * 1000;
	prog->time_to_eat = ft_atol(av[3]) * 1000;
	prog->time_to_sleep = ft_atol(av[4]) * 1000;
	if (!valid_numbers(av))
		return (0);
	if (prog->time_to_die < 6e4 || prog->time_to_eat < 6e4
		|| prog->time_to_sleep < 6e4)
	{
		printf(RED "use more than 60ms\n" RESET);
		return (0);
	}
	if (av[5])
		prog->num_times_to_eat = ft_atol(av[5]);
	else
		prog->num_times_to_eat = -1;
	return (1);
}
