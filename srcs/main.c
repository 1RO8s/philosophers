#include "libft2.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	size_t	num_of_philo;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	required_eat_count;

	if (argc != 5 && argc != 6)
		return (printf("Error: Wrong number of arguments\n"), 1);
	num_of_philo = ft_atoi(argv[1]);
	time_to_die = ft_atoi(argv[2]);
	time_to_eat = ft_atoi(argv[3]);
	time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		required_eat_count = ft_atoi(argv[5]);
	printf("Philosopers!!\n");
	printf("num_of_philo: %zu\n", num_of_philo);
	printf("time_to_die: %zu\n", time_to_die);
	printf("time_to_eat: %zu\n", time_to_eat);
	printf("time_to_sleep: %zu\n", time_to_sleep);
	if (argc == 6)
		printf("required_eat_count: %zu\n", required_eat_count);

  // 哲学者の初期化
	// init_philo(num_of_philo, time_to_die, time_to_eat, time_to_sleep, required_eat_count);

  // 食事の開始
	while(1)
	{
		if(has_dead_philo())
			break;
		if (has_eat_enough())
			break;
	}
	printf("complete\n");
}
