/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:17:08 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/24 13:07:52 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print_usage_msg(void)
{
	printf("✅ Usage: \n");
	printf("./philo <num_philos> <time_die> <time_eat> <time_sleep> ");
	printf("[num_must_eat]\n");
	printf("Arguments:\n");
	printf("1. number of philosophers,\n");
	printf("2. time (in ms) to die without eating,\n");
	printf("3. time (in ms) to eat,\n");
	printf("4. time (in ms) to sleep,\n");
	printf("5. optionally, how many times each philosopher must eat ");
	printf("before simulation ends.\n");
}
int cleanup_data(t_data *data, int exit_code)
{
    t_fork  *forks;
    int     i;

    i = 0;
    forks = data->waiter.forks;
    if (data->philos)
    {
        free(data->philos);
        data->philos = NULL;
    }
    if (data->print_mutex_init)
        pthread_mutex_destroy(&data->print_mutex);
    if (data->waiter.waiter_mutex_init)
        pthread_mutex_destroy(&data->waiter.waiter_mutex);
    if (forks)
    {
        while (i < data->num_fork_mutex_init)
            pthread_mutex_destroy(&forks[i++].mutex);
        free(forks);
        data->waiter.forks = NULL;
    }
    return (exit_code);
}