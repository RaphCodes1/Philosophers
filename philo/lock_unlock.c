/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcreer <rcreer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:29 by rcreer            #+#    #+#             */
/*   Updated: 2025/01/27 17:59:30 by rcreer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	set_val(pthread_mutex_t *mutex, long *dest, long value)
{
	mutex_handle(mutex, LOCK);
	*dest = value;
	mutex_handle(mutex, UNLOCK);
}

long	get_val(pthread_mutex_t *mutex, long *val)
{
	long	res;

	mutex_handle(mutex, LOCK);
	res = *val;
	mutex_handle(mutex, UNLOCK);
	return (res);
}

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	mutex_handle(mutex, LOCK);
	*dest = value;
	mutex_handle(mutex, UNLOCK);
}

bool	get_bool(pthread_mutex_t *mutex, bool *val)
{
	bool	res;

	mutex_handle(mutex, LOCK);
	res = *val;
	mutex_handle(mutex, UNLOCK);
	return (res);
}

bool	sim_finished(t_prog *prog)
{
	return (get_bool(&prog->dead_mutex, &prog->end_sim));
}
