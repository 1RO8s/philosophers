/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:22:03 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/18 09:10:26 by hnagasak         ###   ########.fr       */
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
	// size_t				*eat_count;
	// long				*last_eat_time;
	pthread_mutex_t		*print_mutex;
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


void *monitor(void *args);
void start_monitor(pthread_t	*monitor_thread, t_config *config);

// timer.c
long	us2ms(long usec);
t_timeval	us2timeval(long usec);

// print.c
void					print_config(t_config *config);
void					print_philo(t_philo *data);
void					print_forks(pthread_mutex_t **forks);
void					print_philos_forks(t_philo *data, size_t num_of_philo);

void	mutex_message(t_config *config, char *message);

#endif