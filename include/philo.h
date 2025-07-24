/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:58:04 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/24 13:15:42 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum s_state
{
	EATING,
	THINKING,
	SLEEPING,
	WAITING_FORK,
	DEAD
}					t_state;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	uint64_t		last_eat_time;
	int				num_eaten;
	t_state			state;
	struct s_data	*main_data;
}					t_philo;

typedef struct s_fork
{
	int				id;
	bool			is_available;
	pthread_mutex_t	mutex;
}					t_fork;

typedef struct s_waiter
{
	pthread_mutex_t	waiter_mutex;
    bool            waiter_mutex_init;
	t_fork			*forks;
}					t_waiter;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	uint64_t		start_time;                    
	t_philo			*philos;
	t_waiter		waiter;
	pthread_mutex_t print_mutex;
    bool            print_mutex_init;
    int             num_fork_mutex_init;
}					t_data;

int					parse_input(int argc, char **argv, t_data *data);
void				print_usage_msg(void);
int                 cleanup_data(t_data *data, int exit_code);
int                 init_simulation(t_data *data);

#endif