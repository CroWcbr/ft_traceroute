/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 21:36:12 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/10 18:41:48 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>
#include "./external/libft/include/libft.h"
#include <arpa/inet.h>		// inet_ntop


void create_socket(t_tr* tr)
{
	struct addrinfo	*ai;

	{
		struct addrinfo hints;
		ft_bzero(&hints, sizeof(struct addrinfo));
		hints.ai_flags = AI_CANONNAME;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_RAW;
		hints.ai_protocol = IPPROTO_ICMP;

		int	gai_err;
		if ( (gai_err = getaddrinfo(tr->destination, NULL, &hints, &ai)) != 0)
		{
			printf("ft_traceroute: %s: %s\n", tr->destination, (char *)gai_strerror(gai_err));
			exit(1);
		}
	}

	tr->ai = ai;
	if (ai->ai_family != AF_INET)
		ft_exit("ft_traceroute: unknown address family\n");
	if (!inet_ntop(AF_INET, &((struct sockaddr_in *)ai->ai_addr)->sin_addr, tr->destination_ip, sizeof(tr->destination_ip)))
		ft_exit("ft_traceroute : error inet_ntop in check_destination\n");
	
	tr->sasend = ai->ai_addr;
	if (!(tr->sarecv = ft_calloc(1, ai->ai_addrlen)))
		ft_exit("ft_traceroute : ERROR : socket.c - ft_calloc\n");
	if (!(tr->salast = ft_calloc(1, ai->ai_addrlen)))
		ft_exit("ft_traceroute : ERROR : socket.c - ft_calloc\n");
	if (!(tr->sabind = ft_calloc(1, ai->ai_addrlen)))
		ft_exit("ft_traceroute : ERROR : socket.c - ft_calloc\n");
	tr->salen = ai->ai_addrlen;

	if ((tr->recvfd = socket(tr->sasend->sa_family, SOCK_RAW, tr->icmpproto)) < 0)
		ft_exit("ft_traceroute : socket error recvfd\n");
	setuid(getuid());

	if (setsockopt(tr->recvfd, SOL_SOCKET, SO_RCVTIMEO, &tr->timeout, sizeof(tr->timeout)) < 0)
		ft_exit("ft_traceroute : setsockopt");

	if ((tr->sendfd = socket(tr->sasend->sa_family, SOCK_DGRAM, 0)) < 0)
		ft_exit("ft_traceroute : socket error sendfd\n");

	tr->sabind->sa_family = tr->sasend->sa_family;
	tr->sport = (getpid() & 0xffff) | 0x8000;
	((struct sockaddr_in *)tr->sabind)->sin_port = htons(tr->sport);

	if (bind(tr->sendfd, tr->sabind, tr->salen) < 0)
		ft_exit("ft_traceroute : bind error\n");
}
