/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:30:30 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/03 14:23:27 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, char *state)
{
	uint64_t	time;
	t_data		*data;

	data = philo->main_data;
	pthread_mutex_lock(&data->print_mutex);
	if (!is_sim_running(data) && philo->state != DEAD)
	{
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	time = time_since_sim_start(data);
	printf("%llu %d %s\n", time, philo->id, state);
	pthread_mutex_unlock(&data->print_mutex);
}

static int	is_sleeping(t_philo *philo)
{
	t_data		*data;
	// uint64_t	time_since_last_meal;

	data = philo->main_data;
	if (!is_sim_running(data))
		return (1);
	// pthread_mutex_lock(&data->waiter.waiter_mutex);
	// time_since_last_meal = time_since_sim_start(data) - philo->last_eat_time;
	// pthread_mutex_unlock(&data->waiter.waiter_mutex);
	// if (time_since_last_meal + data->time_to_sleep >= data->time_to_die)
	// {
	// 	return (0);
	// }
	philo->state = SLEEPING;
	print_state(philo, "is sleeping");
	ft_usleep_interupt(data->time_to_sleep, data);
	return (0);
}

static int	think_sleep(t_philo *philo, uint64_t time)
{
	t_data		*data;
	// uint64_t	time_since_last_meal;

	data = philo->main_data;
	if (!is_sim_running(data))
		return (1);
	// pthread_mutex_lock(&data->waiter.waiter_mutex);
	// time_since_last_meal = time_since_sim_start(data) - philo->last_eat_time;
	// pthread_mutex_unlock(&data->waiter.waiter_mutex);
	// if (time_since_last_meal + time >= data->time_to_die)
	// 	return (0);
	philo->state = THINKING;
	print_state(philo, "is thinking");
	ft_usleep_interupt(time, data);
	return (0);
}

static void	sim_delay(t_philo *philo, t_data *data)
{
	// bool	is_even;
	bool	is_odd;

	// is_even = (philo->id % 2 == 0);
    if (data->num_philos == 1)
        return ;
	is_odd = (philo->id % 2 != 0);
	if (is_odd && philo->id == data->num_philos && data->num_philos != 1)
		think_sleep(philo, data->time_to_eat * 2);
	else if (is_odd)
		think_sleep(philo, data->time_to_eat);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->main_data;
	while (get_curr_time() < data->start_time)
		ft_usleep(50);
	sim_delay(philo, data);
	while (1)
	{
		if (is_eating(philo))
			break ;
		if (is_sleeping(philo))
			break ;
		if (data->num_philos % 2 != 0)
			if (think_sleep(philo, 1))
				break ;
	}
	return (NULL);
}
