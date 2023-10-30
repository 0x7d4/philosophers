/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 19:20:57 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/15 16:58:13 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

enum						e_philo_state_mode
{
	HAS_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DEAD
};

typedef struct s_philo_state
{
	enum e_philo_state_mode	mode;
	long					time;
}							t_philo_state;

typedef struct s_philo
{
	int						id;
	pthread_t				*thread;
	t_philo_state			state;
	int						eat_n;
	long					last_eat_time;

}							t_philo;

typedef struct s_table
{
	int						philos_n;
	int						time_die;
	int						time_eat;
	int						time_sleep;
	int						eat_each_n;
	int						dead_philo;
	int						each_ate_enough;
	long					time_start;
	t_philo					*philos;
	pthread_mutex_t			*forks;
	pthread_mutex_t			mx_phlio_state;
	pthread_mutex_t			mx_each_ate_enough;
	pthread_mutex_t			mx_dead_philo;
	pthread_mutex_t			mx_print;
}							t_table;

/**
 * This context holds the info about the table and the -
 * current philo (thread) ID to be used in the thread function.
 */
typedef struct s_philo_ctx
{
	int						i;
	t_table					*table;
}							t_philo_ctx;

/* --- Threads --- */

void						create_threads(t_table *table);

/* --- Monitor --- */

int							check_termination(t_philo_ctx *ctx,
								int exit_thread);
int							is_philo_dead(t_philo philo, int time_die_ms);
int							each_ate_enough(t_table *table);
int							someone_died(t_table *table);
void						monitor_params(t_philo_ctx *ctx);

/* --- Parse --- */

t_table						*init_table(int argc, char **argv);
void						free_table(t_table *table);
t_philo_ctx					*init_main_thread_ctx(t_table *table);
int							is_table_data_valid(t_table *table, int argc,
								char **argv);

/* --- Utils --- */

long						get_time(void);
void						set_state(t_philo_ctx *ctx,
								enum e_philo_state_mode mode);
void						print_state(t_philo_ctx ctx,
								enum e_philo_state_mode mode);

#endif
