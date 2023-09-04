/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:28:51 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/05 00:06:54 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#define IS_DEBUG 1

#include <stdbool.h>	// for bool
#include <stddef.h>		// for size_t
#include <netdb.h>		// for addrinfo

typedef struct s_tr
{
	char				*destination;
	char				destination_ip[16];
	struct addrinfo		*ai;
	struct sockaddr		*sasend;	// sockaddr{} for send, from getaddrinfo
	struct sockaddr		*sarecv;	// sockaddr{} for receiving
	struct sockaddr		*salast;	// last sockaddr{} for receiving
	struct sockaddr		*sabind;	// sockaddr{} for binding source port
	socklen_t			salen;		// length of sockaddr{}
	int					icmpproto;	// IPPROTO_xxx value for ICMP
	int					ttllevel;	// setsockopt() level to set TTL
	int					ttloptname;	// setsockopt() name to set TTL

	int					sendfd;		// send on UDP sock
	int 				recvfd;		// read on raw ICMP sock

	u_short				sport;		// UDP send port
	size_t				first_ttl;	// -f
	size_t				max_ttl;	// -m
	bool				no_dns;		// -n
	u_short				dport;		// -p
	size_t				nprobes;	// -q
	bool				verbose;	// -v?
}	t_tr;

void	parse(int argc, char **argv, t_tr* tr);
void	create_socket(t_tr* tr);
void	traceroute_loop(t_tr *tr);

#endif
