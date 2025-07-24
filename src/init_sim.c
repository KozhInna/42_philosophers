/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:15:17 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/24 14:30:46 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int get_curr_time()
{
    struct timeval  tv;
    uint64_t       time_in_ms;
    
    if(gettimeofday(&tv, NULL) != 0)
        return (0);
    time_in_ms = (uint16_t)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time_in_ms);
}

int	create_philos(t_data *data)
{
	t_philo	*arr_of_philos;
	int		i;

	i = 0;
	arr_of_philos = malloc(data->num_philos * sizeof(t_philo));
	if (!arr_of_philos)
	{
		perror("malloc");
		return (1);
	}
	while (i < data->num_philos)
	{
		arr_of_philos[i].id = i + 1;
		arr_of_philos[i].main_data = data;
        arr_of_philos[i].num_eaten = 0;
        arr_of_philos[i].state = THINKING;
		i++;
	}
	data->philos = arr_of_philos;
	return (0);
}

int init_forks(t_data *data)
{
    int     i;
    t_fork  *forks;

    i = 0;
    forks = data->waiter.forks;
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

int init_simulation(t_data *data)
{
    t_fork      *forks;
    uint64_t    time_in_ms;

    forks = malloc(data->num_philos * sizeof(t_fork));
    if(!forks)
        return (1);
    data->waiter.forks = forks;
    data->num_fork_mutex_init = 0;
    if (init_forks(data) != 0)
        return (1);
    if (pthread_mutex_init(&data->waiter.waiter_mutex, NULL) != 0)
        return (1);
    data->waiter.waiter_mutex_init = true;
    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return (1);
    data->print_mutex_init = true;
    if (create_philos(data) != 0)
        return(1);
    time_in_ms = get_curr_time();
    if (!time_in_ms)
        return (1);
    data->start_time = time_in_ms;
    return (0);
}
