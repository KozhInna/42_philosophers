/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:17:08 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/23 11:17:23 by ikozhina         ###   ########.fr       */
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