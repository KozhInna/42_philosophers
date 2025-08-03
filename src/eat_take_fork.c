/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_take_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:17:47 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/03 14:17:56 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void  assign_fork_order(int left, int right, int *first, int *second)
{
    if (left < right)
    {
        *first = left;
        *second = right;
    }
    else
    {
        *first = right;
        *second = left;
    }
}

int	take_forks(t_philo *philo, int left, int right, t_data *data)
{
	t_fork	*forks;
    int     first;
    int     second;

    assign_fork_order(left, right, &first, &second);
	forks = data->forks;
	pthread_mutex_lock(&forks[first].mutex);
    if (!is_sim_running(data))
    {
        pthread_mutex_unlock(&forks[first].mutex);
        return (1);
    }
	forks[first].is_available = false;
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&forks[second].mutex);
    if (!is_sim_running(data))
    {
		forks[first].is_available = true;
        pthread_mutex_unlock(&forks[first].mutex);
        pthread_mutex_unlock(&forks[second].mutex);
        return (1);
    }
	forks[second].is_available = false;
	print_state(philo, "has taken a fork");
	return (0);
}