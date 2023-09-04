/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:28:54 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/04 20:08:57 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// static void	ft_exit(char *str_err)
// {
// 	printf("%s\n", str_err);
// 	exit(1);
// }

// static void	ft_exit_add_info(char *str_info, char *str_err)
// {
// 	printf("ft_traceroute: %s: %s\n", str_info, str_err);
// 	exit(1);
// }

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
	// if (getuid() != 0)
	// 	ft_exit("ft_traceroute: usage error: You must be root to run this program");

	t_tr	tr;
	parse(argc, argv, &tr);

	signal(SIGINT, _signal_handler);
	signal(SIGQUIT, _signal_handler);
	signal(SIGTSTP, _signal_handler);

	// while(1)
	// {
		
	// }

	return 0;
}
