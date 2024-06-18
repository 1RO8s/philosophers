/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/18 09:18:17 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



long	get_elapsed_usec(t_timeval start)
{
	t_timeval	current;

	// long		elapsed_sec;
	// long		elapsed_usec;
	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000) - (start.tv_sec
			* 1000) - (start.tv_usec / 1000));
	// elapsed_sec = current.tv_sec - start.tv_sec;
	// elapsed_usec = current.tv_usec - start.tv_usec;
	// if (elapsed_usec < 0)
	// {
	// 	elapsed_sec--;
	// 	elapsed_usec += 1000000;
	// }
	// elapsed_usec = elapsed_sec * 1000000 + elapsed_usec;
	// return (elapsed_usec);
}

int	all_philos_eat_enough(t_config *config)
{
	size_t	i;
	size_t	num_of_philo;
	t_philo	*philos;

	philos = config->philos;
	// printf("-----  all_philos_eat_enough -----\n");
	num_of_philo = config->num_of_philo;
	i = 0;
	while (i < num_of_philo)
	{
		// printf("eat_count[%zu]: %zu, ", i, config->eat_count[i]);
		// if (config->eat_count[i] < config->required_eat_count)
		if (philos[i].eat_count < config->required_eat_count)
			return (0);
		i++;
	}
	// printf("\n");
	// printf("All philosophers have eaten enough\n");
	return (1);
}

void	mutex_message(t_config *config, char *message)
{
	pthread_mutex_lock(config->print_mutex);
	printf("%s", message);
	pthread_mutex_unlock(config->print_mutex);
}

void	mutex_print(t_philo *philo, t_status status)
{
	long	elapsed_msec;

	pthread_mutex_lock(philo->config->print_mutex);
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
	pthread_mutex_unlock(philo->config->print_mutex);
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

	last_eat_time = philo->last_eat_timeval;
	time_to_die = philo->config->time_to_die;
	// printf("\t\t\t\t %d last_eat_time: %ld\n", philo->id,
	// 	get_elapsed_usec(last_eat_time));
	if (get_elapsed_usec(last_eat_time) >= (long)time_to_die * 1000)
		return (1);
	return (0);
}

int	should_stop(t_philo *philo)
{
	// mutex_print(philo, TEST);
	if (all_philos_eat_enough(philo->config) || philo->config->is_anyone_dead)
		return (1);
	if (philo_is_dead(philo))
	{
		philo->config->is_anyone_dead = 1;
		mutex_print(philo, DEAD);
		return (1);
	}
	return (0);
}

void	waiting_for_forks(t_philo *philo, pthread_mutex_t *fork1,
		pthread_mutex_t *fork2)
{
	if (pthread_mutex_lock(fork1) != 0)
		printf("Error: Failed to lock fork1[%p]\n", fork1);
	mutex_print(philo, TAKE_FORK);
	if (pthread_mutex_lock(fork2) != 0)
	{
		pthread_mutex_unlock(fork1);
		printf("Error: Failed to lock fork2[%p]\n", fork2);
	}
	mutex_print(philo, TAKE_FORK);
}

//
int	eat(t_philo *philo)
{
	waiting_for_forks(philo, philo->left_fork, philo->right_fork);
	// 食事前に死んでいないか確認
	if (should_stop(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	mutex_print(philo, EATING);
	usleep(philo->config->time_to_eat * 1000);
	// printf("%ld\t%d [test]\n", us2ms(get_elapsed_usec(philo->config->start)),
	// philo->id);
	// mutex_print(philo, TEST);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	mutex_print(philo, TEST);
	// 食事後に死んでいないか確認
	if (should_stop(philo))
		return (1);
	philo->eat_count++;
	gettimeofday(&philo->last_eat_timeval, NULL);
	return (0);
}

void	*handle_philo_actions(void *args)
{
	t_philo		*philo;
	t_config	*config;
	size_t		i;

	philo = (t_philo *)args;
	config = philo->config;
	// config->last_eat_time[philo->id] = get_elapsed_usec(config->start);
	gettimeofday(&philo->last_eat_timeval, NULL);
	i = 0;
	while (1 || i < 5)
	{
		// mutex_print(philo, TEST);
		// 食事
		if (eat(philo))
			break ;
		break ;
		// config->eat_count[philo->id]++;
		// config->last_eat_time[philo->id] = get_elapsed_usec(us2timeval(config->last_eat_time[philo->id]));
		// 睡眠
		if (philo_is_dead(philo))
			config->is_anyone_dead = 1;
		if (all_philos_eat_enough(config) || config->is_anyone_dead)
			break ;
		// philo->eat_count++;
		// gettimeofday(&philo->last_eat_timeval, NULL);
		mutex_print(philo, SLEEPING);
		usleep(philo->config->time_to_sleep * 1000);
		// 思考して次の食事を待つ
		if (philo_is_dead(philo))
			config->is_anyone_dead = 1;
		if (all_philos_eat_enough(config) || config->is_anyone_dead)
			break ;
		mutex_print(philo, THINKING);
		i++;
	}
	return (NULL);
}

t_config	*init_config(int argc, char **argv, t_config *config)
{
	config->num_of_philo = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		config->required_eat_count = atoi(argv[5]);
	else
		config->required_eat_count = SIZE_MAX;
	gettimeofday(&config->start, NULL);
	config->is_anyone_dead = 0;
	config->print_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(config->print_mutex, NULL);
	print_config(config);
	return (config);
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
	for (i = 0; i < num; i++)
	{
		forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (forks[i] == NULL)
		{
			printf("Error: Failed to allocate memory for mutex\n");
			while (i > 0)
				free(forks[--i]);
			free(forks);
			return (NULL);
		}
		printf("# forks[%zu]: %p\n", i, forks[i]);
		if (pthread_mutex_init(forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize mutex\n");
			while (i > 0)
			{
				pthread_mutex_destroy(forks[--i]);
				free(forks[i]);
			}
			free(forks[i]); // The last allocated mutex also needs to be freed
			free(forks);
			return (NULL);
		}
	}
	return (forks);
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

void	wait_for_threads(pthread_t *pthreads, size_t num_of_philo)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_join(pthreads[i], NULL) != 0)
			printf("Error: Failed to join philo thread\n");
		i++;
	}
}

void	start_philos(pthread_t *pthreads, t_philo *data, size_t num_of_philo)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		// mutex_message(data[0].config, "### create philo thread!!\n");
		if (pthread_create(&pthreads[i], NULL, &handle_philo_actions,
				&data[i]) != 0)
			mutex_message(data[0].config, "Failed to create thread\n");
		// perror("Failed to create thread");
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

void	init_philos(t_philo *philos, t_config *config)
{
	size_t	i;

	i = 0;
	while (i < config->num_of_philo)
	{
		printf("config->forks @philos[%zu] 0\n", i);
		print_config_forks(config);
		// philos[i].left_fork = NULL;
		// philos[i].right_fork = NULL;
		philos[i].id = i;
		philos[i].left_fork = config->forks[i];
		if (i + 1 != config->num_of_philo)
		{
			printf("config->forks @philos[%zu] 01\n", i);
			print_config_forks(config);
			philos[i].right_fork = config->forks[i + 1];
			printf("config->forks @philos[%zu] 02\n", i);
			print_config_forks(config);
		}
		else
		{
			philos[i].right_fork = config->forks[0];
		}

		printf("config->forks @philos[%zu] 03\n", i);
		print_config_forks(config);
		philos[i].eat_count = 0;
		printf("config->forks @philos[%zu] 04\n", i);
		print_config_forks(config);

		printf("config->forks @philos[%zu] 1\n", i);
		print_config_forks(config);
		philos[i].last_eat_timeval = config->start;
		printf("config->forks @philos[%zu] 2\n", i);
		print_config_forks(config);
		philos[i].config = config;
		printf("config->forks @philos[%zu] 3\n", i);
		print_config_forks(config);
		i++;
	}
}

pthread_mutex_t	**free_forks(pthread_mutex_t **forks)
{
	size_t	i;

	i = 0;
	while (forks[i] != NULL)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
		i++;
	}
	free(forks);
	return (NULL);
}

// void *monitor(void *args)
// {
// 	(void) args;
// 	// fprintf(stderr, "Monitor\n");
// 	write(2,"--- Monitor thread ---\n", 23);
// 	return (NULL);
// }

int	main(int argc, char **argv)
{
	pthread_t *pthreads;
	pthread_t *th_monitor;
	t_config conf;
	t_philo *philos;
	// t_philo			*philo_args;
	pthread_mutex_t **forks;

	// pthread_t		*pthreads;
	if (is_invalid_argument(argc, argv))
		return (EXIT_FAILURE);

	// initialize
	philos = (t_philo *)malloc(sizeof(t_philo) * conf.num_of_philo);

	init_config(argc, argv, &conf);
	forks = init_forks(conf.num_of_philo);
	printf("forks[2]: %p\n", forks[2]);
	printf("forks[3]: %p\n", forks[3]);
	conf.forks = forks;
	printf("conf.forks[2]: %p\n", conf.forks[2]);
	printf("conf.forks[3]: %p\n", conf.forks[3]);
	init_philos(philos, &conf);
	conf.philos = philos;
	print_philos_forks(philos, conf.num_of_philo);

	// start threads
	pthreads = (pthread_t *)malloc(sizeof(pthread_t) * conf.num_of_philo);
	start_philos(pthreads, philos, conf.num_of_philo);

	
	th_monitor = (pthread_t *)malloc(sizeof(pthread_t));
	start_monitor(th_monitor, &conf);
	// pthread_create(&th_monitor, NULL, &monitor, NULL);
	// if (pthread_create(&th_monitor, NULL, &monitor, &conf) != 0)
	// 	mutex_message(&conf, "Failed to create monitor thread\n");
	// else
	// 	mutex_message(&conf, "### SUCCESS create monitor thread!!\n");
	// if (pthread_create(&th_monitor, NULL, &monitor, &conf) != 0)
	// 	perror("Failed to create monitor thread\n");
	// else
	// 	perror("### SUCCESS create monitor thread!!\n");

	// wait for threads
	mutex_message(&conf, "### waiting for threads!!!!\n");
	// wait_for_threads(pthreads, conf.num_of_philo);
	// if (pthread_join(th_monitor, NULL) != 0)
	// 	perror("Failed to join monitor thread");

	// free
	free(pthreads);
	free(philos);
	forks = free_forks(forks);
	printf("complete\n");
	return (0);
}