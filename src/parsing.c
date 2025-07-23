/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:15:15 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/23 13:58:53 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	is_input_numeric(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
			{
				printf("❌ Arguments should be numeric.\n");
				print_usage_msg();
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	check_args_num(int argc)
{
	if (argc < 5 || argc > 6)
	{
		if (argc < 5)
			printf("❌ Not enough arguments.\n");
		else if (argc > 6)
			printf("❌ Too many arguments.\n");
		print_usage_msg();
		return (1);
	}
	return (0);
}

int	input_to_int(char *arg)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	while (arg[i] >= '0' && arg[i] <= '9')
	{
		res = res * 10 + (arg[i] - '0');
		i++;
	}
	return (res);
}

void	process_input(int argc, char **argv, t_data *data)
{
	data->num_philos = input_to_int(argv[1]);
	data->time_to_die = input_to_int(argv[2]);
	data->time_to_eat = input_to_int(argv[3]);
	data->time_to_sleep = input_to_int(argv[4]);
	if (data->time_to_eat > data->time_to_die)
		data->time_to_eat = data->time_to_die;
	if (data->time_to_sleep > data->time_to_die)
		data->time_to_sleep = data->time_to_die;
	if (argc == 6)
		data->num_must_eat = input_to_int(argv[5]);
}

int	parse_input(int argc, char **argv, t_data *data)
{
	if (check_args_num(argc) == 1 || is_input_numeric(argc, argv) == 1)
		return (1);
	process_input(argc, argv, data);
	printf("args - %d\n", argc);
	int i = 0;
	while (argv[i])
	{
		printf("args - \'%s\'\n", argv[i++]);
	}
	return (0);
}