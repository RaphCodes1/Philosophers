/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:33 by rcreer            #+#    #+#             */
/*   Updated: 2025/02/04 20:15:16 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	check_valid_args(char **av)
{
	int		i;
	int		f;
	size_t	check;

	f = 1;
	while (av[f])
	{
		check = 0;
		i = 0;
		while (av[f][i])
		{
			if (av[f][i] >= '0' && av[f][i] <= '9')
				check++;
			i++;
		}
		if (ft_strlen(av[f]) != check)
			return (1);
		f++;
	}
	return (0);
}

void	creation_thread(t_prog *prog)
{
	int	i;

	i = -1;
	if (prog->num_of_philos == 0)
		return ;
	else if (prog->num_of_philos == 1)
		thread_handle(&prog->philos[0].thread_id, one_philo, &prog->philos[0],
			CREATE);
	else
	{
		while (++i < prog->num_of_philos)
		{
			thread_handle(&prog->philos[i].thread_id, dinner_sim,
				&prog->philos[i], CREATE);
		}
	}
	thread_handle(&prog->monitor, monitor_dinner, prog, CREATE);
	prog->start_sim = get_time(MILLISECOND);
	set_bool(&prog->table_mutex, &prog->threads_ready, true);
	i = -1;
	while (++i < prog->num_of_philos)
		thread_handle(&prog->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&prog->table_mutex, &prog->end_sim, true);
	thread_handle(&prog->monitor, NULL, NULL, JOIN);
}

int	malloc_check(t_prog *prog)
{
	prog->eat_stat = malloc(sizeof(t_prog) * prog->num_of_philos);
	if (!prog->eat_stat)
		return (0);
	prog->philos = malloc(sizeof(t_philo) * prog->num_of_philos);
	if (!prog->philos)
	{
		free(prog->eat_stat);
		return (0);
	}
	prog->forks = malloc(sizeof(t_fork) * prog->num_of_philos);
	if (!prog->forks)
	{
		free(prog->eat_stat);
		free(prog->philos);
		return (0);
	}
	return (1);
}

void	clean(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i < prog->num_of_philos)
		mutex_handle(&prog->philos[i].philo_mutex, DESTROY);
	mutex_handle(&prog->write_lock, DESTROY);
	mutex_handle(&prog->table_mutex, DESTROY);
	mutex_handle(&prog->which_philo_eat_lock, DESTROY);
	mutex_handle(&prog->philo_full_mutex, DESTROY);
	free(prog->eat_stat);
	free(prog->philos);
	free(prog->forks);
}

int	main(int ac, char **av)
{
	t_philo	*philos;
	t_prog	program;

	if ((ac == 5 || ac == 6) && !check_valid_args(av))
	{
		if (!av_input(&program, av))
			return (1);
		if (!data_init(&program, av))
			return (1);
		creation_thread(&program);
		clean(&program);
	}
	else
		printf(RED "Invalid Args\n" RESET);
}
