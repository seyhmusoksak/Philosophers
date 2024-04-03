/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soksak <soksak@42istanbul.com.tr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:09:04 by soksak            #+#    #+#             */
/*   Updated: 2024/04/03 02:23:25 by soksak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	struct s_data	*p_data;
	int				philo_id;
	int				m_count;
	long long		last_eat;
	pthread_t		thread;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	*fork_l;
}					t_philo;

typedef struct s_data
{
	int				philo_eat;
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	int				die_flag;
	long int		t_start;
	int				full_philo_count;
	pthread_mutex_t	dead;
	pthread_mutex_t	meallock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printmutex;
	t_philo			*philo;
}					t_data;

int 	arg_control(int argc, char **argv);
int		my_atoi(char *str);
int 	numeric_control(char **argv);
void	start_simulation(t_data *data);
long long	timestamp(void);
int	ft_sleep(t_philo *philo, long long sleeptime);
int	ft_print_status(t_philo *philo, char *str);
int	sleep_and_think(t_philo *philo);
int	is_dead(t_philo *philo);
int eat(t_philo *philo);
void	*philo_life(void *philo);
void	unlock_forks(t_data *data);

#endif
