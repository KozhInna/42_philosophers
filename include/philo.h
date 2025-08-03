/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:58:04 by ikozhina          #+#    #+#             */
/*   Updated: 2025/08/03 14:57:50 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
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

// typedef struct s_waiter
// {
// 	pthread_mutex_t	waiter_mutex;
// 	bool			waiter_mutex_init;
// 	t_fork			*forks;
// }					t_waiter;

typedef struct s_data
{
	int				sim_running;
	pthread_mutex_t	sim_mutex;
	bool			sim_mutex_init;
    
    pthread_mutex_t	state_mutex;
	bool			state_mutex_init;
	t_fork			*forks;

    pthread_mutex_t	print_mutex;
	bool			print_mutex_init;
	bool			num_fork_mutex_init;

	int				num_philos;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				num_must_eat;
	uint64_t		start_time;
	t_philo			*philos;
	// t_waiter		waiter;

}					t_data;

int					parse_input(int argc, char **argv, t_data *data);
int					init_simulation(t_data *data);
int					run_simulation(t_data *data);
void				*routine(void *arg);
void				*monitor(void *arg);
int					is_eating(t_philo *philo);
int	                take_forks(t_philo *philo, int left, int right, t_data *data);
void				print_state(t_philo *philo, char *state);
// int					all_eaten_enough(t_data *data);
void				print_usage_msg(char *msg);
void				print_error_msg(char *s);
uint64_t			get_curr_time(void);
uint64_t			time_since_sim_start(t_data *data);
void				ft_usleep(uint64_t sleep_time);
void				ft_usleep_interupt(uint64_t sleep_time, t_data *data);
int					cleanup_data(t_data *data, int exit_code);
int					is_sim_running(t_data *data);
void				stop_sim(t_data *data);

#endif