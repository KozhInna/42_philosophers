#ifndef PHILO_H
# define PHILO_H

# include<stdio.h>
# include<unistd.h>
# include<pthread.h>
# include<stdbool.h>
# include<stdint.h>
# include<sys/time.h>

typedef enum s_state
{
    EATING,
    THINKING,
    SLEEPING,
    WAITING_FORK,
    DEAD
} t_state;


typedef struct s_philo
{
    int             id;
    pthread_t       tid;
    uint64_t        last_eat_time;
    int             num_eaten;
    t_state         state;
    struct s_data   *main_data;
} t_philo;

typedef struct s_waiter
{
    pthread_mutex_t mutex;
    bool            *forks_available;
} t_waiter;

typedef struct  s_data
{
    int         num_philos;
    int         time_to_die;
    int         time_to_eat;
    int         time_to_sleep;
    int         num_must_eat;
    uint64_t    start_time;     
    t_philo     *philos;
    t_waiter    water;
    pthread_t   print_mutex;
} t_data;

#endif