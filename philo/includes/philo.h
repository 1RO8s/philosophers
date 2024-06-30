/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:22:03 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 21:05:12 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>

# include <pthread.h>
# include <sys/time.h>

typedef struct timeval	t_timeval;
typedef struct s_config	t_config;
typedef struct s_philo	t_philo;

typedef struct s_config
{
	size_t				num_of_philo;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				required_eat_count;
	pthread_mutex_t		**forks;
	t_timeval			start;
	int					is_anyone_dead;
	pthread_mutex_t		is_anyone_dead_mutex;
	pthread_mutex_t		print_mutex;
	t_philo				*philos;
}						t_config;

typedef struct s_philo
{
	int					id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	size_t				eat_count;
	pthread_mutex_t		eat_count_mutex;
	t_timeval			last_eat_timeval;
	pthread_mutex_t		last_eat_time_mutex;
	t_config			*config;
}						t_philo;

typedef enum e_monitor_result
{
	CONTINUE,
	DIED,
	FULLFILLED
}						t_monitor_result;

typedef enum e_status
{
	DEAD,
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FORK,
	TEST
}						t_status;

// check.c
int						read_is_anyone_dead(t_config *config);
void					update_is_anyone_dead(t_config *config, int value);
int						philo_is_dead(t_philo *philo);
int						should_stop(t_philo *philo);

// config
t_config				*init_config(int argc, char **argv, t_config *config);
pthread_mutex_t			**free_forks(pthread_mutex_t **forks, size_t num);
void					free_config(t_config *config);

// debug.c
void					print_config(t_config *config);
void					print_philo(t_philo *data);
void					print_forks(pthread_mutex_t **forks);
void					print_philos_forks(t_philo *data, size_t num_of_philo);
void					print_result(t_config *config);

// forks
pthread_mutex_t			**init_forks(size_t num);
int						take_1st_fork(t_philo *philo, pthread_mutex_t *fork1);
int						take_2nd_fork(t_philo *philo, pthread_mutex_t *fork2);
void					put_forks(pthread_mutex_t *fork1,
							pthread_mutex_t *fork2);

// monitor.c
void					start_monitor(pthread_t *monitor_thread,
							t_config *config);

// philo.c
void					start_philos(pthread_t *pthreads, t_philo *data,
							size_t num_of_philo);

// print.c
void					mutex_message(t_config *config, char *message);
void					mutex_print(t_philo *philo, t_status status);

// timer.c
t_timeval				us2timeval(long usec);
void					ft_sleep(t_config *config, long msec);
long					get_elapsed_msec(t_timeval start);

// utils
int						mutex_init(pthread_mutex_t *mutex,
							const pthread_mutexattr_t *attr);
void					update_last_eat_time(t_philo *philo);

void update_eat_count(t_philo *philo);
size_t read_eat_count(t_philo *philo);
#endif