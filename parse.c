/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 11:22:47 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/15 17:00:14 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

static int64_t	ft_atoi(const char *str)
{
	int64_t	n;
	int64_t	neg;

	n = 0;
	neg = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			neg = -1;
	while (*str >= '0' && *str <= '9')
	{
		n = (n * 10) + ((*str++) - 48);
	}
	return (n * neg);
}

static void	init_philos_and_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philos_n)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		table->philos[i].id = i + 1;
		table->philos[i].eat_n = 0;
		table->philos[i].last_eat_time = table->time_start;
		table->philos[i].state.mode = THINKING;
		table->philos[i].state.time = table->time_start;
		table->philos[i].thread = malloc(sizeof(pthread_t));
		i++;
	}
}

t_table	*init_table(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(*table));
	table->philos_n = ft_atoi(argv[1]);
	table->time_die = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	table->eat_each_n = -1;
	if (argc == 6)
		table->eat_each_n = ft_atoi(argv[5]);
	if (!is_table_data_valid(table, argc, argv))
		return (NULL);
	table->dead_philo = -1;
	table->each_ate_enough = 0;
	table->philos = malloc(table->philos_n * sizeof(t_philo));
	table->forks = malloc(table->philos_n * sizeof(pthread_mutex_t));
	pthread_mutex_init(&table->mx_phlio_state, NULL);
	pthread_mutex_init(&table->mx_each_ate_enough, NULL);
	pthread_mutex_init(&table->mx_dead_philo, NULL);
	pthread_mutex_init(&table->mx_print, NULL);
	table->time_start = get_time();
	init_philos_and_forks(table);
	return (table);
}

t_philo_ctx	*init_main_thread_ctx(t_table *table)
{
	t_philo_ctx	*ctx;

	ctx = malloc(sizeof(*ctx));
	ctx->i = -1;
	ctx->table = table;
	return (ctx);
}

void	free_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philos_n)
	{
		pthread_mutex_destroy(&table->forks[i]);
		free(table->philos[i++].thread);
	}
	free(table->philos);
	free(table->forks);
	pthread_mutex_destroy(&table->mx_phlio_state);
	pthread_mutex_destroy(&table->mx_each_ate_enough);
	pthread_mutex_destroy(&table->mx_dead_philo);
	pthread_mutex_destroy(&table->mx_print);
	free(table);
}
