/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:15:15 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/27 00:57:30 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	is_input_numeric(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
        if (argv[i][j] == '+' || argv[i][j] == '-')
            j++;
        if (!argv[i][j])
            return(print_usage_msg("❌ Arguments should be numeric."), 1);
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return(print_usage_msg("❌ Arguments should be numeric."), 1);
            j++;
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
            print_usage_msg("❌ Not enough arguments.");
		else if (argc > 6)
            print_usage_msg("❌ Too many arguments.");
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

long int input_to_long(char *arg, int *err)
{
    long	res;

	res = 0;
	while (*arg == ' ' || (*arg >= 9 && *arg <= 13))
		arg++;
	if (*arg == '-' || *arg == '+')
	{
		if (*arg == '-')
        {
            *err = -2;
            return (-1);
        }
        arg++;
	}
	while (*arg >= '0' && *arg <= '9')
	{
		res = res * 10 + (*arg - '0');
        if (res > INT_MAX)
        {
            *err = -3;
            return (-1);
        }
		arg++;
	}
	return (res);
}

int process_input(int argc, char **argv, t_data *data)
{
    int err;
    
    err = 0;
	data->num_philos = input_to_long(argv[1], &err);
    if (err == 0)
	    data->time_to_die = input_to_long(argv[2], &err);
    if (err == 0)
	    data->time_to_eat = input_to_long(argv[3], &err);
    if (err == 0)
	    data->time_to_sleep = input_to_long(argv[4], &err);
	if (argc == 6 && err == 0)
		data->num_must_eat = input_to_long(argv[5], &err);
    else
        data->num_must_eat = -1;
    if (err == -2)
        return (print_usage_msg("❌ Arguments can't be negative."), 1);
    else if (err == -3)
        return (print_usage_msg("❌ Arguments value is too big."), 1);
    if (data->time_to_eat > data->time_to_die)
		data->time_to_eat = data->time_to_die;
	if (data->time_to_sleep > data->time_to_die)
		data->time_to_sleep = data->time_to_die;
    return (0);
}

int	parse_input(int argc, char **argv, t_data *data)
{
	if (check_args_num(argc) == 1 || is_input_numeric(argc, argv) == 1)
		return (1);
	if (process_input(argc, argv, data) == 1)
        return (1);
	return (0);
}