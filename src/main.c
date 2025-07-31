/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:38:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/31 12:41:02 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print_state(t_philo *philo, char *state)
{
	uint64_t	time;
	t_data		*data;

	data = philo->main_data;
	time = time_since_sim_start(data);
	pthread_mutex_lock(&data->print_mutex);
	if (!data->sim_running)
		return ;
	if (data->sim_running || (philo->state == DEAD))
		printf("%lu %d %s\n", time, philo->id, state);
	pthread_mutex_unlock(&data->print_mutex);
}

void	take_forks(t_philo *philo, int left, int right)
{
	t_fork	*forks;

	forks = philo->main_data->waiter.forks;
	pthread_mutex_lock(&forks[left].mutex);
	forks[left].is_available = false;
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&forks[right].mutex);
	forks[right].is_available = false;
	print_state(philo, "has taken a fork");
}

void	release_forks(t_philo *philo, int left, int right)
{
	t_fork	*forks;

	forks = philo->main_data->waiter.forks;
	forks[left].is_available = true;
	pthread_mutex_unlock(&forks[left].mutex);
	forks[right].is_available = true;
	pthread_mutex_unlock(&forks[right].mutex);
}
void	handle_one_philo(t_philo *philo, t_data *data)
{
	t_fork	*forks;

	forks = data->waiter.forks;
	pthread_mutex_lock(&forks[0].mutex);
	forks[0].is_available = false;
	philo->state = WAITING_FORK;
	print_state(philo, "has taken a fork");
	while (data->sim_running)
		ft_usleep(1000);
	forks[0].is_available = true;
	pthread_mutex_unlock(&forks[0].mutex);
}

void	is_eating(t_philo *philo)
{
	t_data	*data;
	int		left;
	int		right;
	int		philo_index;

	data = philo->main_data;
	philo_index = philo->id - 1;
	left = (philo_index + 1 + data->num_philos) % data->num_philos;
	right = philo_index;
	if (data->num_philos == 1)
	{
		handle_one_philo(philo, data);
		return ;
	}
	take_forks(philo, left, right);
	if (!data->sim_running)
	{
		release_forks(philo, left, right);
		return ;
	}
	philo->last_eat_time = time_since_sim_start(data);
	philo->state = EATING;
	print_state(philo, "is eating");
	ft_usleep(data->time_to_eat);
	philo->num_eaten++;
	if (data->num_must_eat > 0 && all_eaten_enough(data))
		data->sim_running = 0;
	release_forks(philo, left, right);
}

void	is_sleeping(t_philo *philo)
{
	t_data	*data;

	data = philo->main_data;
	philo->state = SLEEPING;
	print_state(philo, "is sleeping");
	ft_usleep(data->time_to_sleep);
}
void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	bool	is_even;
	bool	is_odd;

	philo = (t_philo *)arg;
	is_even = (philo->id % 2 == 0);
	is_odd = (philo->id % 2 != 0);
	data = philo->main_data;
	while (get_curr_time() < data->start_time)
		ft_usleep(50);
	if (is_odd && philo->id == data->num_philos && data->num_philos != 1)
	{
		philo->state = THINKING;
		print_state(philo, "is thinking");
		ft_usleep(data->time_to_eat);
		ft_usleep(data->time_to_sleep);
	}
	else if (is_even)
	{
		philo->state = THINKING;
		print_state(philo, "is thinking");
		ft_usleep(data->time_to_eat);
	}
	while (data->sim_running)
	{
		if (!data->sim_running)
			break ;
		is_eating(philo);
		if (!data->sim_running)
			break ;
		is_sleeping(philo);
		if (!data->sim_running)
			break ;
		if (data->num_philos % 2 != 0)
		{
			philo->state = THINKING;
			print_state(philo, "is thinking");
			ft_usleep(data->time_to_eat);
		}
	}
	return (NULL);
}

int	all_eaten_enough(t_data *data)
{
	int		i;
	int		count;
	t_philo	*philos;

	philos = data->philos;
	count = 0;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->waiter.waiter_mutex);
		if (philos[i].num_eaten >= data->num_must_eat)
			count++;
		i++;
		pthread_mutex_lock(&data->waiter.waiter_mutex);
	}
	if (count == data->num_philos)
		return (1);
	return (0);
}

int	is_smb_dead(t_data *data, t_philo *philo)
{
	uint64_t	now;

	pthread_mutex_lock(&data->waiter.waiter_mutex);
	now = time_since_sim_start(data);
	if ((now - philo->last_eat_time) > data->time_to_die
		&& philo->state != EATING)
	{
		philo->state = DEAD;
		pthread_mutex_unlock(&data->waiter.waiter_mutex);
		print_state(philo, "died");
		return (1);
	}
	pthread_mutex_unlock(&data->waiter.waiter_mutex);
	return (0);
}

void	*monitor(void *arg)
{
	int		i;
	t_data	*data;
	t_philo	*philos;

	data = (t_data *)arg;
	philos = data->philos;
	while (get_curr_time() < data->start_time)
		ft_usleep(50);
	while (data->sim_running)
	{
		i = 0;
		while (i < data->num_philos && data->sim_running)
		{
			if (is_smb_dead(data, &philos[i]))
			{
				data->sim_running = 0;
				return (NULL);
			}
			i++;
		}
		if (data->num_must_eat > 0 && data->sim_running)
		{
			if (all_eaten_enough(data))
			{
				data->sim_running = 0;
				return (NULL);
			}
		}
		ft_usleep(1);
	}
	return (NULL);
}

int	run_simulation(t_data *data)
{
	int			i;
	t_philo		*philos;
	pthread_t	tid;

	i = 0;
	philos = data->philos;
	data->start_time = get_curr_time() + 200;
	while (i < data->num_philos)
	{
		if (pthread_create(&philos[i].tid, NULL, routine,
				(void *)&philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&tid, NULL, monitor, (void *)data) != 0)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(philos[i].tid, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(tid, NULL) != 0)
		return (1);
	return (0);
}

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
	return (0);
}
