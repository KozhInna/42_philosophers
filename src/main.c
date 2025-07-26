/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozhina <ikozhina@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:38:39 by ikozhina          #+#    #+#             */
/*   Updated: 2025/07/26 14:10:22 by ikozhina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


void    print_state(t_philo *philo, char *state)
{
    uint64_t    time;
    t_data      *data;

    data = philo->main_data;
    time = time_since_sim_start(data);
    pthread_mutex_lock(&data->print_mutex);
    if (data->sim_running || (philo->state == DEAD))
        printf("%llu %d %s\n", time, philo->id, state);
    pthread_mutex_unlock(&data->print_mutex);
}

void    take_forks(t_philo *philo, int left, int right)
{
    t_fork  *forks;

    forks = philo->main_data->waiter.forks;
    pthread_mutex_lock(&forks[left].mutex);
    forks[left].is_available = false;
    print_state(philo, "has taken a fork");
    pthread_mutex_lock(&forks[right].mutex);
    forks[right].is_available = false;
    print_state(philo, "has taken a fork");
}

void    release_forks(t_philo *philo, int left, int right)
{
    t_fork  *forks;

    forks = philo->main_data->waiter.forks;
    forks[left].is_available = true;
    pthread_mutex_unlock(&forks[left].mutex);
    forks[right].is_available = true;
    pthread_mutex_unlock(&forks[right].mutex);
}

bool    is_eating( t_philo *philo)
{
    t_data      *data;
    t_fork      *forks;
    int         left;
    int         right;
    uint64_t    since_last_meal;
    bool        near_starving;

    data = philo->main_data;
    forks = data->waiter.forks;
    left = philo->id - 1;
    right = philo->id % data->num_philos;
    pthread_mutex_lock(&data->waiter.waiter_mutex);
    if (left == right)
    {
        if (forks[left].is_available)
        {
            pthread_mutex_lock(&forks[left].mutex);
            forks[left].is_available = false;
            pthread_mutex_unlock(&data->waiter.waiter_mutex);
            print_state(philo, "has taken a fork");
            while (data->sim_running)
                usleep(10000);
            pthread_mutex_lock(&data->waiter.waiter_mutex);
            forks[left].is_available = true;
            pthread_mutex_unlock(&data->waiter.waiter_mutex);
            pthread_mutex_unlock(&forks[left].mutex);
        }
        else
        {
            pthread_mutex_unlock(&data->waiter.waiter_mutex);
        }
        return (false);
    }
    else if (forks[left].is_available && forks[right].is_available)
    {
        since_last_meal = time_since_sim_start(data) - philo->last_eat_time;
        near_starving = since_last_meal > (data->time_to_die * 0.8);
        if (philo->num_eaten == 0 || near_starving)
        {
            take_forks(philo, left, right);
            philo->last_eat_time = time_since_sim_start(data);
            philo->state = EATING;
            pthread_mutex_unlock(&philo->main_data->waiter.waiter_mutex);
            print_state(philo, "is eating");
            usleep(data->time_to_eat * 1000);
            pthread_mutex_lock(&data->waiter.waiter_mutex);
            philo->num_eaten++;
            if (data->num_must_eat > 0 && all_eaten_enough(data))
                data->sim_running = 0;
            pthread_mutex_unlock(&data->waiter.waiter_mutex);
            release_forks(philo, left, right);
            return (true);
        }
    }
    pthread_mutex_unlock(&philo->main_data->waiter.waiter_mutex);
    return (false);
}

void    is_sleeping(t_philo *philo)
{
    t_data *data;

    data = philo->main_data;
    philo->state = SLEEPING;
    print_state(philo, "is sleeping");
    usleep(data->time_to_sleep * 1000);
}

void    *routine(void *arg)
{
    t_philo *philo;
    t_data  *data;

    philo = (t_philo *)arg;
    data = philo->main_data;
    while (data->sim_running)
    {
        if (!data->sim_running)
            break ;
        if (is_eating(philo))
        {
            if (!data->sim_running)
                return NULL;
            is_sleeping(philo);
            if (!data->sim_running)
                return NULL;
            philo->state = THINKING;
            print_state(philo, "is thinking");
        }
        else
            usleep(1000);
    }
    return (NULL);
}

int all_eaten_enough(t_data *data)
{
    int     i;
    int     count;
    t_philo *philos;
    
    philos = data->philos;
    count = 0;
    i = 0;
    while (i < data->num_philos)
    {
        if (philos[i].num_eaten >= data->num_must_eat)
            count++;
        i++;
    }
    if (count == data->num_philos)
    {
        printf("here - %d\n", count);   
        return (1);
    }
    return (0);
}

int is_smb_dead(t_data *data, t_philo *philo)
{
    uint64_t    now;

    pthread_mutex_lock(&data->waiter.waiter_mutex);
    now = time_since_sim_start(data);
    if ((now - philo->last_eat_time) > data->time_to_die && philo->state != EATING)
    {
        philo->state = DEAD;
        pthread_mutex_unlock(&data->waiter.waiter_mutex);
        print_state(philo, "died");
        return (1);
    }
    pthread_mutex_unlock(&data->waiter.waiter_mutex);
    return (0);
}

void    *monitor(void *arg)
{
    int     i;
    t_data  *data;
    t_philo *philos;
    
    data = (t_data *)arg;
    philos = data->philos;
    while (data->sim_running)
    {
        i = 0;
        while (i < data->num_philos)
        {
            if (is_smb_dead(data, &philos[i]))
            {
                data->sim_running = 0;
                // usleep(1000);
                return (NULL);
            }
            i++;
        }
        if (data->num_must_eat > 0)
        {
            pthread_mutex_lock(&data->waiter.waiter_mutex);
            if (all_eaten_enough(data))
            {
                data->sim_running = 0;
                pthread_mutex_unlock(&data->waiter.waiter_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->waiter.waiter_mutex);
        }
        // if (data->num_must_eat > 0 && all_eaten_enough(data))
        // {
        //     data->sim_running = 0;
        //     return (NULL);
        // }
        // usleep(1000);
        usleep(100);
    }
    return (NULL);
}

int run_simulation(t_data *data)
{
    int         i;
    t_philo     *philos;
    pthread_t   tid;

    i = 0;
    philos = data->philos; 
    while (i < data->num_philos)
    {
        if (pthread_create(&philos[i].tid, NULL, routine, (void *)&philos[i]) != 0)
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
    
    // printf("time %llu\n", data.start_time);
    // usleep(200000);
    // printf("time %llu\n", get_time_elapsed(&data));
	return (0);
}
// philo num, time_to_die, time_to_eat, time_to_sleep, option
