#include<philo.h>

void    print_usage_msg(void)
{
    printf("✅ Usage: \n");
    printf("./philo <num_philos> <time_die> <time_eat> <time_sleep> ");
    printf("[num_must_eat]\n");
    printf("Arguments:\n");
    printf("1. number of philosophers,\n");
    printf("2. time (in ms) to die without eating,\n");
    printf("3. time (in ms) to eat,\n");
    printf("4. time (in ms) to sleep,\n");
    printf("5. optionally, how many times each philosopher must eat ");
    printf("before simulation ends.\n");
}

int parse_input(int argc, char **argv)
{
    (void)argv;
    if (argc < 5 || argc > 6)
    {
        if (argc < 5)
            printf("❌ Not enough arguments.\n");
        else if (argc > 6)
            printf("❌ Too many arguments.\n");
        print_usage_msg();
        return (1);
    }
    printf("args - %d\n", argc);
    return(0);
}

int main(int argc, char **argv)
{
    parse_input(argc, argv); 
    return (0);
}

//philo num, time_to_die, time_to_eat, time_to_sleep, option
