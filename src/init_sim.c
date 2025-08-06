/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:15:17 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/06 10:12:28 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_philos(t_data *data)
{
	t_philo	*arr_of_philos;
	int		i;

	i = 0;
	arr_of_philos = malloc(data->num_philos * sizeof(t_philo));
	if (!arr_of_philos)
	{
		print_error_msg("Malloc failed");
		return (1);
	}
	while (i < data->num_philos)
	{
		arr_of_philos[i].id = i + 1;
		arr_of_philos[i].main_data = data;
		arr_of_philos[i].num_eaten = 0;
		arr_of_philos[i].last_eat_time = 0;
		i++;
	}
	data->philos = arr_of_philos;
	return (0);
}

static int	init_forks(t_data *data)
{
	int		i;
	t_fork	*forks;

	i = 0;
	forks = data->forks;
	while (i < data->num_philos)
	{
		forks[i].id = i + 1;
		forks[i].is_available = true;
		if (pthread_mutex_init(&forks[i].mutex, NULL) != 0)
			return (1);
		data->num_fork_mutex_init++;
		i++;
	}
	return (0);
}

int	init_simulation(t_data *data)
{
	t_fork	*forks;

	data->sim_running = 1;
	forks = malloc(data->num_philos * sizeof(t_fork));
	if (!forks)
		return (print_error_msg("Malloc failed"), 1);
	data->forks = forks;
	data->num_fork_mutex_init = 0;
	if (init_forks(data) != 0)
		return (1);
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
		return (1);
	data->state_mutex_init = true;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	data->print_mutex_init = true;
	if (pthread_mutex_init(&data->sim_mutex, NULL) != 0)
		return (1);
	data->sim_mutex_init = true;
	if (create_philos(data) != 0)
		return (1);
	data->start_time = 0;
	return (0);
}
