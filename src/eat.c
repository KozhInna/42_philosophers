/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:34:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/04 12:28:54 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	release_forks(t_philo *philo, int left, int right)
{
	t_fork	*forks;

	forks = philo->main_data->forks;
	forks[left].is_available = true;
	pthread_mutex_unlock(&forks[left].mutex);
	forks[right].is_available = true;
	pthread_mutex_unlock(&forks[right].mutex);
}

static void	handle_one_philo(t_philo *philo, t_data *data)
{
	t_fork	*forks;

	forks = data->forks;
	pthread_mutex_lock(&forks[0].mutex);
	forks[0].is_available = false;
	philo->state = WAITING_FORK;
	print_state(philo, "has taken a fork");
	while (is_sim_running(data))
		ft_usleep(1000);
	forks[0].is_available = true;
	pthread_mutex_unlock(&forks[0].mutex);
}

static void	update_eating_state(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	philo->last_eat_time = time_since_sim_start(data);
	philo->state = EATING;
	pthread_mutex_unlock(&data->state_mutex);
}

static void	update_eating_count(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	philo->num_eaten++;
	pthread_mutex_unlock(&data->state_mutex);
}

int	is_eating(t_philo *philo)
{
	t_data	*data;
	int		left;
	int		right;

	data = philo->main_data;
	left = philo->id % data->num_philos;
	right = philo->id - 1;
	if (data->num_philos == 1)
	{
		handle_one_philo(philo, data);
		return (1);
	}
	if (!is_sim_running(data) || take_forks(philo, left, right, data))
		return (1);
	if (!is_sim_running(data))
	{
		release_forks(philo, left, right);
		return (1);
	}
	update_eating_state(philo, data);
	print_state(philo, "is eating");
	ft_usleep_interupt(data->time_to_eat, data);
	update_eating_count(philo, data);
	release_forks(philo, left, right);
	return (0);
}
