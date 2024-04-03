/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_live.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 04:28:44 by soksak            #+#    #+#             */
/*   Updated: 2024/04/03 04:39:16 by soksak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	int	eat_finish(t_data *data)
{
	pthread_mutex_lock(&data->meallock);
	if (data->full_philo_count == data->n_philo)
	{
		pthread_mutex_unlock(&data->meallock);
		return (1);
	}
	pthread_mutex_unlock(&data->meallock);
	return (0);
}

static void	finish_control(t_data *data)
{
	int	loop;

	loop = 0;
	while (1)
	{
		if (loop == data->n_philo)
			loop = 0;
		pthread_mutex_lock(&data->dead);
		if ((timestamp() - (data->philo[loop]).last_eat) > data->t_die)
		{
			if (data->die_flag == 0)
				printf("%llu %d %s", timestamp() - data->t_start,
					data->philo[loop].philo_id, "is dead\n");
			data->die_flag = 1;
			unlock_forks(data);
			pthread_mutex_unlock(&data->dead);
			break ;
		}
		pthread_mutex_unlock(&data->dead);
		if (eat_finish(data) == 1)
			break ;
		loop++;
	}
}

void	*philo_life(void *philo)
{
	t_philo	*tmp_philo;

	tmp_philo = (t_philo *)philo;
	tmp_philo->last_eat = timestamp();
	while (1)
	{
		pthread_mutex_lock(&tmp_philo->p_data->meallock);
		if (tmp_philo->p_data->full_philo_count == tmp_philo->p_data->n_philo)
		{
			pthread_mutex_unlock(&tmp_philo->p_data->meallock);
			return (0);
		}
		pthread_mutex_unlock(&tmp_philo->p_data->meallock);
		if (is_dead(tmp_philo))
			return (0);
		if (eat(tmp_philo))
			return (0);
		if (sleep_and_think(tmp_philo))
			return (0);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->t_start = timestamp();
	while (i < data->n_philo)
	{
		pthread_create(&data->philo[i].thread, NULL,
			&philo_life, &data->philo[i]);
		i++;
		usleep(50);
	}
	finish_control(data);
	i = 0;
	while (i < data->n_philo)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
}
