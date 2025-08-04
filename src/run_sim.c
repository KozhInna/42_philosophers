/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_sim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:37:59 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/04 12:31:59 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_sim_running(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->sim_mutex);
	status = data->sim_running;
	pthread_mutex_unlock(&data->sim_mutex);
	return (status);
}

void	stop_sim(t_data *data)
{
	pthread_mutex_lock(&data->sim_mutex);
	data->sim_running = 0;
	pthread_mutex_unlock(&data->sim_mutex);
}

static int	join_threads(t_data *data, int num)
{
	t_philo	*threads;
	int		i;

	i = 0;
	threads = data->philos;
	while (i < num)
	{
		if (pthread_join(threads[i].tid, NULL) != 0)
		{
			print_error_msg("Pthread_join failed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	create_threads(t_data *data)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = data->philos;
	while (i < data->num_philos)
	{
		if (pthread_create(&philos[i].tid, NULL, routine,
				(void *)&philos[i]) != 0)
		{
			stop_sim(data);
			join_threads(data, i);
			printf("Thread creation failed.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	run_simulation(t_data *data)
{
	pthread_t	monitor_tid;

	data->start_time = get_curr_time() + 200;
	if (create_threads(data) != 0)
		return (1);
	if (pthread_create(&monitor_tid, NULL, monitor, (void *)data) != 0)
	{
		stop_sim(data);
		join_threads(data, data->num_philos);
		return (1);
	}
	if (join_threads(data, data->num_philos) != 0)
		return (1);
	if (pthread_join(monitor_tid, NULL) != 0)
		return (1);
	return (0);
}
