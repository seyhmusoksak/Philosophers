/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 00:21:36 by soksak            #+#    #+#             */
/*   Updated: 2024/04/02 02:01:03 by soksak           ###   ########.fr       */
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

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_data->meallock);
	if (philo->p_data->full_philo_count == philo->p_data->n_philo)
	{
		pthread_mutex_unlock(&philo->p_data->meallock);
		return (2);
	}
	pthread_mutex_unlock(&philo->p_data->meallock);
	pthread_mutex_lock(&philo->p_data->dead);
	if (philo->p_data->die_flag == 1)
	{
		pthread_mutex_unlock(&philo->p_data->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->p_data->dead);
	return (0);
}


int	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_l);
	ft_print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->fork_r);
	ft_print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->p_data->dead);
	philo->last_eat = timestamp();
	pthread_mutex_unlock(&philo->p_data->dead);
	ft_print_status(philo, "is eating");
	if (ft_sleep(philo, philo->p_data->t_eat))
		return (1);
	pthread_mutex_lock(&philo->p_data->meallock);
	philo->m_count--;
	if (philo->m_count == 0)
	{
		philo->p_data->full_philo_count++;
		pthread_mutex_unlock(&philo->p_data->meallock);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_unlock(philo->fork_r);
		return (1);
	}
	pthread_mutex_unlock(&philo->p_data->meallock);
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
	return (0);
}

void	*philo_life(void *philo)
{
	t_philo *tmp_philo;

	tmp_philo = (t_philo *)philo;
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
		pthread_create(&data->philo[i].thread, NULL, &philo_life, &data->philo[i]);
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
