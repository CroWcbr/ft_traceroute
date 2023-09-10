/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:28:54 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/10 18:41:55 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void _signal_handler(int signal)
{
	if (signal == 2)
		printf("\033[93m\rinterapt signal Ctrl + C from user\n\033[0m");
	else if (signal == 3)
		printf("\033[93m\rinterapt signal Ctrl + / from user\n\033[0m");
	else if (signal == 20)
		printf("\033[93m\rinterapt signal Ctrl + Z from user\n\033[0m");
	else
		printf("\033[93m\runknown signal\n\033[0m");
	exit(signal);
}

int main(int argc, char **argv)
{
	if (getuid() != 0)
		ft_exit("ft_traceroute: usage error: You must be root to run this program\n");

	t_tr	tr;
	parse(argc, argv, &tr);
	create_socket(&tr);

	signal(SIGINT, _signal_handler);
	signal(SIGQUIT, _signal_handler);
	signal(SIGTSTP, _signal_handler);

	traceroute_loop(&tr);

	freeaddrinfo(tr.ai);
	free(tr.sarecv);
	free(tr.salast);
	free(tr.sabind);
	return 0;
}
