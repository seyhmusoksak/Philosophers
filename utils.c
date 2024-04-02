/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 20:53:41 by soksak            #+#    #+#             */
/*   Updated: 2024/04/02 01:49:02 by soksak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	my_atoi(char *str)
{
	int		i;
	long	n;
	int		sign;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	if (n * sign > 2147483647 || n * sign < -2147483648)
		exit(-1);
	return (n * sign);
}


int arg_control(int argc, char **argv)
{
	int	i;
	i = 0;
	if (argc == 5 || argc == 6)
	{
		if(numeric_control(argv))
			return (1);
		while (argv[i])
		{
			if (my_atoi(argv[i]) < 0)
				return (2);
			i++;
		}
		if (argc == 6 && my_atoi(argv[5]) == 0)
			return (3);
	}
	else
	{
		return (4);
	}
	return (0);
}

int numeric_control(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_sleep(t_philo *philo, long long sleeptime)
{
	long long	time;

	time = timestamp() + sleeptime;
	while (time > timestamp())
	{
		usleep(200);
		if (is_dead(philo) == 1)
			return (1);
	}
	return (0);
}

int	ft_print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->p_data->printmutex);
	if (is_dead(philo) != 0)
	{
		pthread_mutex_unlock(&philo->p_data->printmutex);
		return (1);
	}
	printf("%llu %d %s\n", (timestamp() - philo->p_data->t_start), philo->philo_id, str);
	pthread_mutex_unlock(&philo->p_data->printmutex);
	return (0);
}

int	sleep_and_think(t_philo *philo)
{
	if (ft_print_status(philo, "is sleeping"))
		return (1);
	if (ft_sleep(philo, philo->p_data->t_start))
		return (1);
	if (ft_print_status(philo, "is thinking"))
		return (1);
	return(0);
}

void	unlock_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_unlock(&data->forks[i]);
		i++;
	}
}
