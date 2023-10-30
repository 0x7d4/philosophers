/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 18:27:04 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/15 17:04:06 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000L) + (time.tv_usec / 1000L));
}

void	print_state(t_philo_ctx ctx, enum e_philo_state_mode mode)
{
	long	now;

	pthread_mutex_lock(&ctx.table->mx_print);
	now = get_time();
	printf("%ld %d %s\n", now - ctx.table->time_start, ctx.i + 1,
		(char [5][20]){"has taken a fork", "is eating", "is sleeping",
		"is thinking", "died"}[mode]);
	pthread_mutex_unlock(&ctx.table->mx_print);
}

void	set_state(t_philo_ctx *ctx, enum e_philo_state_mode mode)
{
	long	now;

	check_termination(ctx, 1);
	now = get_time();
	ctx->table->philos[ctx->i].state = (t_philo_state){mode, now};
	print_state(*ctx, mode);
}

static int	is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_table_data_valid(t_table *table, int argc, char **argv)
{
	if (!is_digit(argv[1]) || !is_digit(argv[2]) || !is_digit(argv[3])
		|| !is_digit(argv[4]) || (argc == 6 && !is_digit(argv[5])))
		return (0);
	if (table->philos_n < 1)
		return (0);
	if (table->time_die < 0 || table->time_eat < 0 || table->time_sleep < 0
		|| table->eat_each_n < -1)
		return (0);
	return (1);
}
