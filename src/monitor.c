/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:33:21 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 16:49:17 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_eaten_enough(t_data *data)
{
	int		i;
	int		count;
	t_philo	*philos;

	philos = data->philos;
	count = 0;
	i = 0;
	pthread_mutex_lock(&data->waiter.waiter_mutex);
	while (i < data->num_philos)
	{
		if (philos[i].num_eaten >= data->num_must_eat)
			count++;
		i++;
	}
	pthread_mutex_unlock(&data->waiter.waiter_mutex);
	if (count == data->num_philos)
		return (1);
	return (0);
}

int	is_smb_dead(t_data *data)
{
	uint64_t	now;
	t_philo		*philos;
	int			i;

	i = 0;
	philos = data->philos;
	while (i < data->num_philos && data->sim_running)
	{
		pthread_mutex_lock(&data->waiter.waiter_mutex);
		now = time_since_sim_start(data);
		if ((now - philos[i].last_eat_time) > data->time_to_die
			&& philos[i].state != EATING)
		{
			philos[i].state = DEAD;
			pthread_mutex_unlock(&data->waiter.waiter_mutex);
			print_state(&philos[i], "died");
			data->sim_running = 0;
			return (1);
		}
		pthread_mutex_unlock(&data->waiter.waiter_mutex);
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (get_curr_time() < data->start_time)
		ft_usleep(50);
	while (data->sim_running)
	{
		if (is_smb_dead(data))
			return (NULL);
		if (data->num_must_eat > 0 && data->sim_running)
		{
			if (all_eaten_enough(data))
			{
				data->sim_running = 0;
				return (NULL);
			}
		}
		ft_usleep(1);
	}
	return (NULL);
}
