/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 15:29:04 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/16 14:10:30 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_philo_dead(t_philo philo, int time_die_ms)
{
	int	dead;

	if (philo.last_eat_time == -1)
		return (0);
	dead = get_time() > philo.last_eat_time + time_die_ms;
	return (dead);
}

int	each_ate_enough(t_table *table)
{
	int	i;

	if (table->eat_each_n == -1)
		return (0);
	i = 0;
	pthread_mutex_lock(&table->mx_phlio_state);
	while (i < table->philos_n)
	{
		if (table->philos[i].eat_n < table->eat_each_n)
		{
			pthread_mutex_unlock(&table->mx_phlio_state);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&table->mx_phlio_state);
	return (1);
}

int	someone_died(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philos_n)
	{
		if (is_philo_dead(table->philos[i], table->time_die))
		{
			pthread_mutex_unlock(&table->mx_phlio_state);
			return (i);
		}
		pthread_mutex_unlock(&table->mx_phlio_state);
		i++;
	}
	return (-1);
}

int	check_termination(t_philo_ctx *ctx, int exit_thread)
{
	pthread_mutex_lock(&ctx->table->mx_dead_philo);
	pthread_mutex_lock(&ctx->table->mx_each_ate_enough);
	if (ctx->table->dead_philo > -1 || ctx->table->each_ate_enough)
	{
		pthread_mutex_unlock(&ctx->table->mx_each_ate_enough);
		pthread_mutex_unlock(&ctx->table->mx_dead_philo);
		if (exit_thread)
		{
			free(ctx);
			pthread_exit(NULL);
		}
		return (1);
	}
	pthread_mutex_unlock(&ctx->table->mx_each_ate_enough);
	pthread_mutex_unlock(&ctx->table->mx_dead_philo);
	return (0);
}

void	monitor_params(t_philo_ctx *ctx)
{
	int	i;

	i = 0;
	while (1)
	{
		i = someone_died(ctx->table);
		if (i > -1)
		{
			pthread_mutex_lock(&ctx->table->mx_dead_philo);
			ctx->table->dead_philo = i;
			pthread_mutex_unlock(&ctx->table->mx_dead_philo);
			print_state((t_philo_ctx){i, ctx->table}, DEAD);
			break ;
		}
		if (each_ate_enough(ctx->table))
		{
			pthread_mutex_lock(&ctx->table->mx_each_ate_enough);
			ctx->table->each_ate_enough = 1;
			pthread_mutex_unlock(&ctx->table->mx_each_ate_enough);
			break ;
		}
		usleep(1000);
	}
}
