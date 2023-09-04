/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:28:51 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/04 20:08:16 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#define IS_DEBUG 1

#include <stdbool.h>	// for bool
#include <stddef.h>		// for size_t

typedef struct s_tr
{
	char				*destination;
	size_t				first_ttl;	// -f
	size_t				max_ttl;	// -m
	bool				no_dns;		// -n
	size_t				dport;		// -p
	size_t				nprobes;	// -q
	bool				verbose;	// -v?
}	t_tr;


void	parse(int argc, char **argv, t_tr* tr);

void	ft_exit(char *str_err);
void	ft_exit_add_info(char *str_info, char *str_err);

#endif
