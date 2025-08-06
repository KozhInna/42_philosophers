/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:33:21 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/06 10:12:14 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	all_eaten_enough(t_data *data)
{
	int		i;
	int		count;
	t_philo	*philos;

	philos = data->philos;
	count = 0;
	i = 0;
	pthread_mutex_lock(&data->state_mutex);
	while (i < data->num_philos)
	{
		if (philos[i].num_eaten >= data->num_must_eat)
			count++;
		i++;
	}
	pthread_mutex_unlock(&data->state_mutex);
	if (count == data->num_philos)
		return (1);
	return (0);
}

static int	is_smb_dead(t_data *data)
{
	uint64_t	now;
	uint64_t	time;
	t_philo		*philos;
	int			i;

	i = 0;
	philos = data->philos;
	while (i < data->num_philos && is_sim_running(data))
	{
		pthread_mutex_lock(&data->state_mutex);
		now = time_since_sim_start(data);
		if ((now - philos[i].last_eat_time) > data->time_to_die)
		{
			pthread_mutex_unlock(&data->state_mutex);
			stop_sim(data);
			pthread_mutex_lock(&data->print_mutex);
			time = time_since_sim_start(data);
			printf("%lu %d %s\n", time, philos[i].id, "died");
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->state_mutex);
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
	while (is_sim_running(data))
	{
		if (is_smb_dead(data))
			return (NULL);
		if (data->num_must_eat > 0 && is_sim_running(data))
		{
			if (all_eaten_enough(data))
			{
				stop_sim(data);
				return (NULL);
			}
		}
		ft_usleep(1);
	}
	return (NULL);
}
