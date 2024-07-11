/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:47:32 by hnagasak          #+#    #+#             */
/*   Updated: 2024/07/12 02:33:31 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

static int	is_space(char c)
{
	if (('\t' <= c && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	nbr;

	sign = 1;
	nbr = 0;
	while (is_space(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		if ((sign == 1 && sign * nbr > (LONG_MAX - (*str - '0')) / 10))
			return (-1);
		if (sign == -1 && sign * nbr < (LONG_MIN + (*str - '0')) / 10)
			return (-1);
		nbr = nbr * 10 + (*str - '0');
		str++;
	}
	if (nbr * sign > INT_MAX || nbr * sign < INT_MIN)
		return (-1);
	return (sign * (int)nbr);
}
