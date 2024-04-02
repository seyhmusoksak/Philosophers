/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:11:03 by soksak            #+#    #+#             */
/*   Updated: 2024/04/02 01:19:46 by soksak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		data->philo[i].p_data = data;
		data->philo[i].fork_l = &data->forks[i];
		data->philo[i].fork_r = &data->forks[(i + 1) % data->n_philo];
		pthread_mutex_init(data->philo[i].fork_l, NULL);
		pthread_mutex_init(data->philo[i].fork_r, NULL);
		data->philo[i].last_eat = 0;
		data->philo[i].m_count = data->n_eat;
		data->philo[i].philo_id = i + 1;
		i++;
	}
}
static int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	data->n_philo = my_atoi(argv[1]);
	data->t_die = my_atoi(argv[2]);
	data->t_eat = my_atoi(argv[3]);
	data->t_sleep = my_atoi(argv[4]);
	data->die_flag = 0;
	data->full_philo_count = 0;
	data->t_start = 0;
	data->n_eat = -1;
	if (argc == 6)
		data->n_eat = my_atoi(argv[5]);
	pthread_mutex_init(&data->dead, NULL);
	pthread_mutex_init(&data->printmutex, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * (data->n_philo));
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->n_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->meallock, NULL);
	data->philo = (t_philo *)malloc(sizeof(t_philo) * (data->n_philo));
	if (!data->philo)
		return (2);
	return (0);
}


int main(int argc, char **argv)
{
	t_data	*data;
	int		error_code;

	error_code = arg_control(argc, argv);
	if (error_code != 0)
	{
		if (error_code == 3)
			return (3);
		write(2, "Error\n", 6);
		return (error_code);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (5);
	if (init_data(data, argc, argv))
		return (5);
	init_philo(data);
	start_simulation(data);
}
