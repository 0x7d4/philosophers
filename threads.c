/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 14:45:08 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/14 20:08:59 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

/**
 * This function is used to check if a philosopher is dead while
 * he has taken a fork or is eating. It unlocks the mutex of the fork
 * to avoid deadlock.
 */
static void	check_termination_fork(t_philo_ctx *ctx, int rf, int lf, int both)
{
	if (check_termination(ctx, 0))
	{
		pthread_mutex_unlock(&ctx->table->forks[lf]);
		if (both)
			pthread_mutex_unlock(&ctx->table->forks[rf]);
		pthread_exit(NULL);
	}
}

static void	eat(t_philo_ctx *ctx, enum e_philo_state_mode mode)
{
	int	rf;
	int	lf;

	rf = (ctx->i + (ctx->i % 2)) % ctx->table->philos_n;
	lf = rf - 1;
	pthread_mutex_lock(&ctx->table->forks[rf]);
	set_state(ctx, HAS_FORK);
	if (ctx->table->philos_n == 1)
	{
		free(ctx);
		pthread_exit(NULL);
	}
	check_termination_fork(ctx, rf, lf, 0);
	pthread_mutex_lock(&ctx->table->forks[lf]);
	check_termination_fork(ctx, rf, lf, 1);
	set_state(ctx, HAS_FORK);
	check_termination_fork(ctx, rf, lf, 1);
	set_state(ctx, mode);
	pthread_mutex_lock(&ctx->table->mx_phlio_state);
	ctx->table->philos[ctx->i].last_eat_time = ctx->table->philos[ctx->i]
		.state.time;
	pthread_mutex_unlock(&ctx->table->mx_phlio_state);
	usleep(ctx->table->time_eat * 1000);
	pthread_mutex_unlock(&ctx->table->forks[lf]);
	pthread_mutex_unlock(&ctx->table->forks[rf]);
}

void	philo_do(t_philo_ctx *ctx, enum e_philo_state_mode mode)
{
	if (mode == EATING)
		eat(ctx, mode);
	if (mode == SLEEPING)
	{
		set_state(ctx, mode);
		pthread_mutex_lock(&ctx->table->mx_phlio_state);
		ctx->table->philos[ctx->i].eat_n++;
		pthread_mutex_unlock(&ctx->table->mx_phlio_state);
		if (ctx->table->time_sleep)
			usleep(ctx->table->time_sleep * 1000);
	}
	if (mode == THINKING)
		set_state(ctx, THINKING);
}

static void	*philo_thread(void *param)
{
	t_philo_ctx	*ctx;

	ctx = (t_philo_ctx *)param;
	while (!check_termination(ctx, 0))
	{
		philo_do(ctx, THINKING);
		philo_do(ctx, EATING);
		philo_do(ctx, SLEEPING);
	}
	free(ctx);
	pthread_exit(NULL);
}

void	create_threads(t_table *table)
{
	int			i;
	t_philo_ctx	*ctx;

	i = 0;
	while (i < table->philos_n)
	{
		ctx = malloc(sizeof(*ctx));
		ctx->i = i;
		ctx->table = table;
		pthread_create(table->philos[i].thread, NULL, philo_thread,
			(void *)ctx);
		i++;
	}
}
