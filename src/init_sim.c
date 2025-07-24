/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:15:17 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/24 13:15:30 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int get_curr_time()
{
    return (0);
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
        arr_of_philos[i].last_eat_time = get_curr_time();
		i++;
	}
	data->philos = arr_of_philos;
	return (0);
}


int init_fork(t_fork *fork, int i)
{
    fork->id = i + 1;
    fork->is_available = true;
    if (pthread_mutex_init(&fork->mutex, NULL) != 0)
        return (1);
    return (0);
}

int init_simulation(t_data *data)
{
    t_fork      *forks;
    int         i;
    
    i = 0;
    forks = malloc(data->num_philos * sizeof(t_fork));
    if(!forks)
        return (1);
    data->waiter.forks = forks;
    data->num_fork_mutex_init = 0;
    while (i < data->num_philos)
    {
        if (init_fork(&forks[i], i) != 0)
            return (1);
        data->num_fork_mutex_init++;
        i++;
    }
    if (pthread_mutex_init(&data->waiter.waiter_mutex, NULL) != 0)
        return (1);
    data->waiter.waiter_mutex_init = true;
    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return (1);
    data->print_mutex_init = true;
    if (create_philos(data) != 0)
        return(1);
    return (0);
}