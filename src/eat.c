/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:34:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 16:48:36 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo, int left, int right)
{
	t_fork	*forks;

	forks = philo->main_data->waiter.forks;
	pthread_mutex_lock(&forks[left].mutex);
	forks[left].is_available = false;
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&forks[right].mutex);
	forks[right].is_available = false;
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
	int		philo_index;

	data = philo->main_data;
	philo_index = philo->id - 1;
	left = (philo_index + 1 + data->num_philos) % data->num_philos;
	right = philo_index;
	if (data->num_philos == 1)
		return (handle_one_philo(philo, data));
	take_forks(philo, left, right);
	if (!data->sim_running)
		return (release_forks(philo, left, right));
	philo->last_eat_time = time_since_sim_start(data);
	philo->state = EATING;
	print_state(philo, "is eating");
	ft_usleep(data->time_to_eat);
	philo->num_eaten++;
	if (data->num_must_eat > 0 && all_eaten_enough(data))
		data->sim_running = 0;
	release_forks(philo, left, right);
}
