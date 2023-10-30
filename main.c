/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 19:22:06 by mmomeni           #+#    #+#             */
/*   Updated: 2023/09/15 16:58:47 by mmomeni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	main(int argc, char **argv)
{
	t_table		*table;
	t_philo_ctx	*ctx;
	int			i;

	if (argc != 6 && argc != 5)
		return (printf("Invalid number of arguments\n"));
	i = 0;
	table = init_table(argc, argv);
	if (!table)
		return (printf("Invalid arguments\n"));
	ctx = init_main_thread_ctx(table);
	create_threads(ctx->table);
	monitor_params(ctx);
	while (i < ctx->table->philos_n)
		pthread_join(*ctx->table->philos[i++].thread, NULL);
	free_table(ctx->table);
	free(ctx);
	return (EXIT_SUCCESS);
}
