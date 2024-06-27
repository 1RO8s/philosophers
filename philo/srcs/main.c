/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 10:21:06 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	read_is_anyone_dead(t_config *config)
{
	int	value;

	pthread_mutex_lock(&config->is_anyone_dead_mutex);
	value = config->is_anyone_dead;
	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
	return (value);
}

void	update_is_anyone_dead(t_config *config, int value)
{
	pthread_mutex_lock(&config->is_anyone_dead_mutex);
	config->is_anyone_dead = value;
	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
}

long	get_elapsed_usec(t_timeval start)
{
	t_timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000) - (start.tv_sec
			* 1000) - (start.tv_usec / 1000));
}

int	all_philos_eat_enough(t_config *config)
{
	size_t	i;
	size_t	num_of_philo;
	t_philo	*philos;

	philos = config->philos;
	num_of_philo = config->num_of_philo;
	i = 0;
	while (i < num_of_philo)
	{
		if (philos[i].eat_count < config->required_eat_count)
			return (0);
		i++;
	}
	return (1);
}

void	mutex_message(t_config *config, char *message)
{
	pthread_mutex_lock(&config->print_mutex);
	printf("%s", message);
	pthread_mutex_unlock(&config->print_mutex);
}

void	mutex_print(t_philo *philo, t_status status)
{
	long	elapsed_msec;

	pthread_mutex_lock(&philo->config->print_mutex);
	elapsed_msec = us2ms(get_elapsed_usec(philo->config->start));
	if (status == DEAD)
		printf("%ld\t%d died\n", elapsed_msec, philo->id);
	else if (status == EATING)
		printf("%ld\t%d is eating\n", elapsed_msec, philo->id);
	else if (status == SLEEPING)
		printf("%ld\t%d is sleeping\n", elapsed_msec, philo->id);
	else if (status == THINKING)
		printf("%ld\t%d is thinking\n", elapsed_msec, philo->id);
	else if (status == TAKE_FORK)
		printf("%ld\t%d has taken a fork\n", elapsed_msec, philo->id);
	else if (status == TEST)
		printf("%ld\t%d test\n", elapsed_msec, philo->id);
	pthread_mutex_unlock(&philo->config->print_mutex);
}

void	print_take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	long	elapsed_msec;

	pthread_mutex_lock(&philo->config->print_mutex);
	elapsed_msec = us2ms(get_elapsed_usec(philo->config->start));
	printf("%ld\t%d has taken a fork %p\n", elapsed_msec, philo->id, fork);
	pthread_mutex_unlock(&philo->config->print_mutex);
}

/**
 * @brief Checks if a philosopher has died.
 * @param philo A pointer to the philosopher structure.
 * @return 1 if the philosopher has died, 0 otherwise.
 */
int	philo_is_dead(t_philo *philo)
{
	t_timeval	last_eat_time;
	size_t		time_to_die;
	long		from_last_eat;

	last_eat_time = philo->last_eat_timeval;
	from_last_eat = get_elapsed_usec(last_eat_time);
	time_to_die = philo->config->time_to_die;
	if (from_last_eat > (long)time_to_die)
	{
		update_is_anyone_dead(philo->config, DIED);
		mutex_print(philo, DEAD);
		return (1);
	}
	return (0);
}

int	should_stop(t_philo *philo)
{
	t_config	*config;

	config = philo->config;
	if (read_is_anyone_dead(config) != CONTINUE)
		return (1);
	return (0);
}

void	waiting_for_forks(t_philo *philo, pthread_mutex_t *fork1,
		pthread_mutex_t *fork2)
{
	if (pthread_mutex_lock(fork1) != 0)
	{
		printf("Error: Failed to lock fork1[%p]\n", fork1);
		return ;
	}
	if (should_stop(philo))
	{
		pthread_mutex_unlock(fork1);
		return ;
	}
	mutex_print(philo, TAKE_FORK);
	if (pthread_mutex_lock(fork2) != 0)
	{
		printf("Error: Failed to lock fork2[%p]\n", fork2);
		pthread_mutex_unlock(fork1);
		return ;
	}
	if (should_stop(philo))
	{
		pthread_mutex_unlock(fork1);
		pthread_mutex_unlock(fork2);
		return ;
	}
	mutex_print(philo, TAKE_FORK);
}

void	wait_for_forks(t_philo *philo)
{
	t_config	*config;
	int			total_philos;

	config = philo->config;
	total_philos = config->num_of_philo;
	if (philo->id % 2 == 1)
	{
		if (get_elapsed_usec(philo->config->start) < 50)
			ft_sleep(*config, 10);
	}
	if (total_philos % 2 == 1 && philo->id == total_philos - 1)
	{
		if (get_elapsed_usec(philo->config->start) < 50)
			ft_sleep(*config, 10);
	}
	if (philo->id % 2 == 0)
		waiting_for_forks(philo, philo->left_fork, philo->right_fork);
	else
		waiting_for_forks(philo, philo->left_fork, philo->right_fork);
}

void	update_last_eat_time(t_philo *philo)
{
	gettimeofday(&philo->last_eat_timeval, NULL);
}

void	put_forks(pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
}

int	eat(t_philo *philo)
{
	wait_for_forks(philo);
	if (should_stop(philo))
	{
		put_forks(philo->right_fork, philo->left_fork);
		return (1);
	}
	mutex_print(philo, EATING);
	update_last_eat_time(philo);
	ft_sleep(*philo->config, philo->config->time_to_eat);
	put_forks(philo->right_fork, philo->left_fork);
	if (should_stop(philo))
		return (1);
	philo->eat_count++;
	return (0);
}

void	*handle_philo_actions(void *args)
{
	t_philo	*philo;
	size_t	i;

	philo = (t_philo *)args;
	update_last_eat_time(philo);
	i = 0;
	while (1 || i < 100)
	{
		if (eat(philo))
			break ;
		if (should_stop(philo))
			break ;
		mutex_print(philo, SLEEPING);
		ft_sleep(*philo->config, philo->config->time_to_sleep);
		if (should_stop(philo))
			break ;
		mutex_print(philo, THINKING);
		i++;
	}
	return (NULL);
}

pthread_mutex_t	*init_fork(pthread_mutex_t **forks, size_t i)
{
	forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (forks[i] == NULL)
	{
		printf("Error: Failed to allocate memory for mutex\n");
		free_forks(forks, i);
		return (NULL);
	}
	if (mutex_init(forks[i], NULL) != 0)
	{
		free(forks[i]);
		free_forks(forks, i);
		return (NULL);
	}
	return (forks[i]);
}

pthread_mutex_t	**init_forks(size_t num)
{
	pthread_mutex_t	**forks;
	size_t			i;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * num);
	if (forks == NULL)
	{
		printf("Error: Failed to allocate memory for forks array\n");
		return (NULL);
	}
	i = 0;
	while (i < num)
	{
		if (init_fork(forks, i) == NULL)
			return (NULL);
		printf("init_forks[%zu]: %p\n", i, (void *)forks[i]);
		i++;
	}
	return (forks);
}

void	set_args(int argc, char **argv, t_config *config)
{
	config->num_of_philo = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		config->required_eat_count = atoi(argv[5]);
	else
		config->required_eat_count = SIZE_MAX;
}

void	init_philos(t_philo *philos, t_config *config)
{
	size_t	i;

	i = 0;
	while (i < config->num_of_philo)
	{
		philos[i].id = i;
		philos[i].right_fork = config->forks[i];
		if (i + 1 != config->num_of_philo)
			philos[i].left_fork = config->forks[i + 1];
		else
			philos[i].left_fork = config->forks[0];
		philos[i].eat_count = 0;
		philos[i].last_eat_timeval = config->start;
		philos[i].config = config;
		i++;
	}
}

int	mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr) != 0)
	{
		perror("Failed to initialize mutex");
		return (1);
	}
	return (0);
}

t_config	*init_config(int argc, char **argv, t_config *config)
{
	pthread_mutex_t	**forks;
	t_philo			*philos;

	set_args(argc, argv, config);
	gettimeofday(&config->start, NULL);
	config->is_anyone_dead = 0;
	if (mutex_init(&config->print_mutex, NULL) != 0)
		return (NULL);
	if (mutex_init(&config->is_anyone_dead_mutex, NULL) != 0)
		return (NULL);
	print_config(config);
	forks = init_forks(config->num_of_philo);
	config->forks = forks;
	philos = (t_philo *)malloc(sizeof(t_philo) * config->num_of_philo);
	if (philos == NULL)
		return (NULL);
	init_philos(philos, config);
	config->philos = philos;
	print_philos_forks(philos, config->num_of_philo);
	return (config);
}

int	is_invalid_argument(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	if (atoi(argv[1]) < 2 || atoi(argv[1]) > 200)
	{
		printf("Error: Number of philosophers must be between 2 and 200\n");
		return (1);
	}
	if (atoi(argv[2]) < 60 || atoi(argv[3]) < 60 || atoi(argv[4]) < 60)
	{
		printf("Error: Time to die, eat, and sleep must be at least 60ms\n");
		return (1);
	}
	if (argc == 6 && atoi(argv[5]) < 1)
	{
		printf("Error: Required eat count must be at least 1\n");
		return (1);
	}
	return (0);
}

void	wait_for_threads(pthread_t *pthreads, size_t num_of_philo,
		pthread_t th_monitor)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_join(pthreads[i], NULL) != 0)
			printf("Error: Failed to join philo thread\n");
		i++;
	}
	if (pthread_join(th_monitor, NULL) != 0)
		printf("Error: Failed to join monitor thread\n");
}

void	start_philos(pthread_t *pthreads, t_philo *data, size_t num_of_philo)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_create(&pthreads[i], NULL, &handle_philo_actions,
				&data[i]) != 0)
			mutex_message(data[0].config, "Failed to create thread\n");
		i++;
	}
}

void	print_config_forks(t_config *config)
{
	size_t	i;

	i = 0;
	printf("--- print_config_forks ---\n");
	while (i < config->num_of_philo)
	{
		printf("config->forks[%zu]: %p\n", i, (void *)config->forks[i]);
		i++;
	}
	printf("\n");
}

pthread_mutex_t	**free_forks(pthread_mutex_t **forks, size_t num)
{
	size_t	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
		i++;
	}
	free(forks);
	return (NULL);
}

void	print_result(t_config *config)
{
	if (config->is_anyone_dead == DIED)
		printf("FAILURE: A philosopher died\n");
	else if (config->is_anyone_dead == FULLFILLED)
		printf("SUCCESS: All philosophers are fullfilled\n");
}

void	free_config(t_config *config)
{
	pthread_mutex_destroy(&config->print_mutex);
	pthread_mutex_destroy(&config->is_anyone_dead_mutex);
	free(config->philos);
	free_forks(config->forks, config->num_of_philo);
}

void	start_pthreads(pthread_t *pthreads, t_philo *philos,
		pthread_t *th_monitor, t_config *config)
{
	start_monitor(th_monitor, config);
	start_philos(pthreads, philos, config->num_of_philo);
}

int	main(int argc, char **argv)
{
	pthread_t	*pthreads;
	pthread_t	th_monitor;
	t_config	conf;

	if (is_invalid_argument(argc, argv))
		return (EXIT_FAILURE);
	init_config(argc, argv, &conf);
	pthreads = (pthread_t *)malloc(sizeof(pthread_t) * conf.num_of_philo);
	if (pthreads == NULL)
	{
		printf("Error: Failed to allocate memory for pthreads array\n");
		free(pthreads);
		free_config(&conf);
		return (EXIT_FAILURE);
	}
	start_pthreads(pthreads, conf.philos, &th_monitor, &conf);
	wait_for_threads(pthreads, conf.num_of_philo, th_monitor);
	free(pthreads);
	free_config(&conf);
	print_result(&conf);
	return (0);
}
