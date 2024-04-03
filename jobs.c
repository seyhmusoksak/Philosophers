/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 00:21:36 by soksak            #+#    #+#             */
/*   Updated: 2024/04/03 04:42:11 by soksak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_sleep(t_philo *philo, long long sleeptime)
{
	long long	loop;

	loop = timestamp() + sleeptime;
	while (loop > timestamp())
	{
		usleep(200);
		if (is_dead(philo) == 1)
			return (1);
	}
	return (0);
}

int	sleep_and_think(t_philo *philo)
{
	if (ft_print_status(philo, "is sleeping"))
		return (1);
	if (ft_sleep(philo, philo->p_data->t_sleep))
		return (1);
	if (ft_print_status(philo, "is thinking"))
		return (1);
	return (0);
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
