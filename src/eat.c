/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:34:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 23:07:29 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo, int left, int right, t_data *data)
{
	t_fork	*forks;
    int     first;
    int     second;

    first = (left < right) ? left : right;
    second = (left < right) ? right : left;
	forks = philo->main_data->waiter.forks;
    if (!data->sim_running)
    {
        return ;
    }
	pthread_mutex_lock(&forks[first].mutex);
    if (!data->sim_running)
    {
        pthread_mutex_unlock(&forks[first].mutex);
        return ;
    }
	forks[first].is_available = false;
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&forks[second].mutex);
    if (!data->sim_running)
    {
        pthread_mutex_unlock(&forks[first].mutex);
        pthread_mutex_unlock(&forks[second].mutex);
        return ;
    }
	forks[second].is_available = false;
	print_state(philo, "has taken a fork");
}

void	release_forks(t_philo *philo, int left, int right)
{
	t_fork	*forks;

	forks = philo->main_data->waiter.forks;
	forks[left].is_available = true;
	pthread_mutex_unlock(&forks[left].mutex);
	forks[right].is_available = true;
	pthread_mutex_unlock(&forks[right].mutex);
}

void	handle_one_philo(t_philo *philo, t_data *data)
{
	t_fork	*forks;

	forks = data->waiter.forks;
	pthread_mutex_lock(&forks[0].mutex);
	forks[0].is_available = false;
	philo->state = WAITING_FORK;
	print_state(philo, "has taken a fork");
	while (data->sim_running)
		ft_usleep(1000);
	forks[0].is_available = true;
	pthread_mutex_unlock(&forks[0].mutex);
}

void	is_eating(t_philo *philo)
{
	t_data	*data;
	int		left;
	int		right;

	data = philo->main_data;
	left = philo->id % data->num_philos;
	right = philo->id - 1;
    
	if (data->num_philos == 1)
		return (handle_one_philo(philo, data));
	take_forks(philo, left, right, data);
	if (!data->sim_running)
    {
        release_forks(philo, left, right);
		return ;
    }
	pthread_mutex_lock(&data->waiter.waiter_mutex);
	philo->last_eat_time = time_since_sim_start(data);
	philo->state = EATING;
	pthread_mutex_unlock(&data->waiter.waiter_mutex);
	print_state(philo, "is eating");
	ft_usleep(data->time_to_eat);
	pthread_mutex_lock(&data->waiter.waiter_mutex);
	philo->num_eaten++;
	pthread_mutex_unlock(&data->waiter.waiter_mutex);
	release_forks(philo, left, right);
}
