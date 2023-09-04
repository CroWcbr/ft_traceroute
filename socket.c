/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 21:36:12 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/04 22:53:02 by cdarrell         ###   ########.fr       */
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

	if (ai->ai_family != AF_INET)
	{
		printf("ft_traceroute: unknown address family\n");
		exit(1);
	}
	if (!inet_ntop(AF_INET, &((struct sockaddr_in *)ai->ai_addr)->sin_addr, tr->destination_ip, sizeof(tr->destination_ip)))
	{
		printf("ft_traceroute : error inet_ntop in check_destination\n");
		exit(1);
	}
	
	tr->sasend = ai->ai_addr;
	if (!(tr->sarecv = ft_calloc(1, ai->ai_addrlen)))
	{
		printf("ft_traceroute : ERROR : socket.c - ft_calloc\n");
		exit(1);
	}
	if (!(tr->salast = ft_calloc(1, ai->ai_addrlen)))
	{
		printf("ft_traceroute : ERROR : socket.c - ft_calloc\n");
		exit(1);
	}
	if (!(tr->sabind = ft_calloc(1, ai->ai_addrlen)))
	{
		printf("ft_traceroute : ERROR : socket.c - ft_calloc\n");
		exit(1);
	}
	tr->salen = ai->ai_addrlen;
	tr->ai = ai;

	if ((tr->recvfd = socket(tr->sasend->sa_family, SOCK_RAW, tr->icmpproto)) < 0)
	{
		printf("ft_traceroute : socket error recvfd\n");
		exit(1);
	}
	setuid(getuid());

	if ((tr->sendfd = socket(tr->sasend->sa_family, SOCK_DGRAM, 0)) < 0)
	{
		printf("ft_traceroute : socket error sendfd\n");
		exit(1);
	}

	tr->sabind->sa_family = tr->sasend->sa_family;
	tr->sport = (getpid() & 0xffff) | 0x8000;
	((struct sockaddr_in *)tr->sabind)->sin_port = htons(tr->sport);

	if (bind(tr->sendfd, tr->sabind, tr->salen) < 0)
	{
		printf("ft_traceroute : bind error\n");
		exit(1);
	}
}
