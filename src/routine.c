/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:30:30 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 16:49:33 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, char *state)
{
	uint64_t	time;
	t_data		*data;

	data = philo->main_data;
	time = time_since_sim_start(data);
	pthread_mutex_lock(&data->print_mutex);
	if (!data->sim_running)
		return ;
	if (data->sim_running || (philo->state == DEAD))
		printf("%lu %d %s\n", time, philo->id, state);
	pthread_mutex_unlock(&data->print_mutex);
}

void	is_sleeping(t_philo *philo)
{
	t_data	*data;

	data = philo->main_data;
	philo->state = SLEEPING;
	print_state(philo, "is sleeping");
	ft_usleep(data->time_to_sleep);
}

void	think_sleep(t_philo *philo, uint64_t time)
{
	t_data	*data;

	data = philo->main_data;
	if (!data->sim_running)
		return ;
	philo->state = THINKING;
	print_state(philo, "is thinking");
	ft_usleep(time);
}

void	sim_delay(t_philo *philo, t_data *data)
{
	bool	is_even;
	bool	is_odd;

	is_even = (philo->id % 2 == 0);
	is_odd = (philo->id % 2 != 0);
	if (is_odd && philo->id == data->num_philos && data->num_philos != 1)
		think_sleep(philo, data->time_to_eat + data->time_to_sleep);
	else if (is_even)
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
	while (data->sim_running)
	{
		if (!data->sim_running)
			break ;
		is_eating(philo);
		if (!data->sim_running)
			break ;
		is_sleeping(philo);
		if (!data->sim_running)
			break ;
		if (data->num_philos % 2 != 0)
			think_sleep(philo, data->time_to_eat);
	}
	return (NULL);
}
