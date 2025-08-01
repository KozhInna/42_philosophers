/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:38:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/01 15:59:59 by ikozhina         ###   ########.fr       */
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
	if (run_simulation(&data) != 0)
		return (cleanup_data(&data, 1));
	return (cleanup_data(&data, 0));
}
