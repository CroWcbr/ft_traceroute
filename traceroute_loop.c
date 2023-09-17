/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 00:04:52 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/17 22:48:48 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include "./external/libft/include/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>		// inet_ntop

static void	icmpcode(int code)
{
	switch (code)
	{
		case  0:	printf("network unreachable"); break;
		case  1:	printf("host unreachable"); break;
		case  2:	printf("protocol unreachable"); break;
		case  3:	printf("port unreachable"); break;
		case  4:	printf("fragmentation required but DF bit set"); break;
		case  5:	printf("source route failed"); break;
		case  6:	printf("destination network unknown"); break;
		case  7:	printf("destination host unknown"); break;
		case  8:	printf("source host isolated (obsolete)"); break;
		case  9:	printf("destination network administratively prohibited"); break;
		case 10:	printf("destination host administratively prohibited"); break;
		case 11:	printf("network unreachable for TOS"); break;
		case 12:	printf("host unreachable for TOS"); break;
		case 13:	printf("communication administratively prohibited by filtering"); break;
		case 14:	printf("host recedence violation"); break;
		case 15:	printf("precedence cutoff in effect"); break;
		default:	printf("[unknown code %d]", code); break;
	}
}

static void	tv_sub(struct timeval *out, struct timeval *in)
{
	if ( (out->tv_usec -= in->tv_usec) < 0) {	/* out -= in */
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void traceroute_loop(t_tr *tr)
{
	int					seq = 0;
	int					code = 0;
	int					done = 0;
	double				rtt;
	char				sendbuf[sizeof(t_rec)];
	t_rec				*rec = (t_rec*)sendbuf;
	struct timeval		tvrecv;

	printf("traceroute to %s (%s): %ld hops max, %ld data bytes\n",
			tr->ai->ai_canonname ? tr->ai->ai_canonname : tr->destination_ip,
			tr->destination_ip, tr->max_ttl, sizeof(t_rec));

	for (size_t ttl = tr->first_ttl; ttl <= tr->max_ttl && done == 0; ++ttl)
	{
		if (setsockopt(tr->sendfd, tr->ttllevel, tr->ttloptname, &ttl, sizeof(int)) < 0)
			ft_exit("ft_traceroute : setsockopt error\n");

		ft_bzero(tr->salast, tr->salen);
		printf("%2ld ", ttl);

		for (size_t probe = 0; probe < tr->nprobes; ++probe)
		{
			rec->rec_seq = ++seq;
			rec->rec_ttl = ttl;
			if (gettimeofday(&rec->rec_tv, NULL) == -1)
				ft_exit("ft_traceroute : gettimeofday error\n");

			((struct sockaddr_in *)tr->sasend)->sin_port = htons(tr->dport + seq);
			if (sendto(tr->sendfd, sendbuf, sizeof(t_rec), 0, tr->sasend, tr->salen) != sizeof(t_rec))
				ft_exit("ft_traceroute : sendto error\n");

// -3 on timeout
// -2 on ICMP time exceeded in transit (caller keeps going)
// -1 on ICMP port unreachable (caller is done)
// >= 0 return value is some other ICMP unreachable code
			if ((code = receive_the_package(tr, seq, &tvrecv, &rec->rec_tv)) == -3)
				printf(" *");
			else
			{

				if (tr->sarecv->sa_family != tr->salast->sa_family || \
						ft_memcmp( &((struct sockaddr_in *) tr->sarecv)->sin_addr, &((struct sockaddr_in *) tr->salast)->sin_addr, sizeof(struct in_addr)))
				{
					char str_dest[128];
					if (!inet_ntop(AF_INET, &((struct sockaddr_in *)tr->sarecv)->sin_addr, str_dest, sizeof(str_dest)))
						ft_exit("ft_traceroute : error inet_ntop in middle_destination\n");

					if (!tr->no_dns)
					{
						char	str[NI_MAXHOST];
						if (getnameinfo(tr->sarecv, tr->salen, str, sizeof(str), NULL, 0, 0) == 0)
							printf(" %s (%s)", str, str_dest);
						else
							printf(" %s (%s)", str_dest, str_dest);
					}
					else
						printf(" %s", str_dest);
					ft_memcpy(tr->salast, tr->sarecv, tr->salen);
				}
				tv_sub(&tvrecv, &rec->rec_tv);
				rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
				printf("  %.3f ms", rtt);

				if (code == -1)
					done++;
				else if (code >= 0)
					icmpcode(code);
			}
		}
		printf("\n");
	}
}
