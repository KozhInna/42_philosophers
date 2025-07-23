/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:38:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/23 13:58:49 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	create_philos(t_data *data)
{
	t_philo	*arr_of_philos;
	int		i;

	i = 0;
	arr_of_philos = malloc(data->num_philos * sizeof(t_philo));
	if (!arr_of_philos)
	{
		perror("malloc");
		return (0);
	}
	while (i < data->num_philos)
	{
		arr_of_philos[i].id = i + 1;
		arr_of_philos[i].main_data = data;
		i++;
	}
	data->philos = arr_of_philos;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_input(argc, argv, &data) != 0)
		return (1);
	if (create_philos(&data) != 0)
		return (1);
	return (0);
}
// philo num, time_to_die, time_to_eat, time_to_sleep, option
