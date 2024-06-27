/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:22:03 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 20:09:39 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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
	t_timeval			last_eat_timeval;
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

long					get_elapsed_usec(t_timeval start);
long					us2ms(long usec);
int						philo_is_dead(t_philo *philo);
void					mutex_print(t_philo *philo, t_status status);
void					mutex_message(t_config *config, char *message);

void					*monitor(void *args);
void					start_monitor(pthread_t *monitor_thread,
							t_config *config);

void					update_is_anyone_dead(t_config *config, int value);
int						read_is_anyone_dead(t_config *config);

pthread_mutex_t			**free_forks(pthread_mutex_t **forks, size_t num);

// timer.c
long					us2ms(long usec);
t_timeval				us2timeval(long usec);
void					ft_sleep(t_config conf, long msec);

// print.c
void					print_config(t_config *config);
void					print_philo(t_philo *data);
void					print_forks(pthread_mutex_t **forks);
void					print_philos_forks(t_philo *data, size_t num_of_philo);
void					print_result(t_config *config);
void					print_take_fork(t_philo *philo, pthread_mutex_t *fork);

void					mutex_message(t_config *config, char *message);

int						mutex_init(pthread_mutex_t *mutex,
							const pthread_mutexattr_t *attr);

// config
t_config				*init_config(int argc, char **argv, t_config *config);
void					free_config(t_config *config);

// forks
pthread_mutex_t			**init_forks(size_t num);
void					wait_for_forks(t_philo *philo);
void					put_forks(pthread_mutex_t *fork1,
							pthread_mutex_t *fork2);

int						should_stop(t_philo *philo);

// philo
void					*handle_philo_actions(void *args);
void					start_philos(pthread_t *pthreads, t_philo *data,
							size_t num_of_philo);

#endif