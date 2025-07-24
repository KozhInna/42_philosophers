/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:38:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/24 13:15:12 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_input(argc, argv, &data) != 0)
		return (1);
    if (init_simulation(&data) != 0)
        return (cleanup_data(&data, 1));

    
	return (0);
}
// philo num, time_to_die, time_to_eat, time_to_sleep, option
