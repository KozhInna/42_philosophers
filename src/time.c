/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:18:38 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 16:46:49 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_curr_time(void)
{
	struct timeval	tv;
	uint64_t		time_in_ms;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	time_in_ms = (uint64_t)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

uint64_t	time_since_sim_start(t_data *data)
{
	uint64_t	current_time;

	current_time = get_curr_time();
	return (current_time - data->start_time);
}

void	ft_usleep(uint64_t sleep_time)
{
	uint64_t	start;

	start = get_curr_time();
	while ((get_curr_time() - start) < sleep_time)
		usleep(500);
}
