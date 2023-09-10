/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieve_package.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 00:34:19 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/10 22:19:54 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <unistd.h>
#include <arpa/inet.h>		// inet_ntop

int		receive_the_package(t_tr* tr, int seq, struct timeval *tv, struct timeval *tvstart)
{
	int				ret;
	int				hlen1;
	int				hlen2;
	int				icmplen;
	ssize_t			n;
	char			recvbuf[1500];
	struct ip		*ip;
	struct ip		*hip;
	struct icmp		*icmp;
	struct udphdr	*udp;

	while(true)
	{
		n = recvfrom(tr->recvfd, recvbuf, sizeof(recvbuf), 0, tr->sarecv, &tr->salen);
		if (gettimeofday(tv, NULL) == -1)
			ft_exit("ft_traceroute : gettimeofday error\n");
		if (tv->tv_sec - tvstart->tv_sec >= tr->timeout.tv_sec)		// check alarm
			return -3;
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			else
				ft_exit("ft_traceroute : recvfrom error\n");
		}

		ip = (struct ip *) recvbuf;									// start of IP header
		hlen1 = ip->ip_hl << 2;										// length of IP header

		icmp = (struct icmp *) (recvbuf + hlen1);					// start of ICMP header
		if ( (icmplen = n - hlen1) < 8)
			continue;												// not enough to look at ICMP header

		if (icmp->icmp_type == ICMP_TIMXCEED && icmp->icmp_code == ICMP_TIMXCEED_INTRANS)
		{
			if (icmplen < (int)(8 + sizeof(struct ip)))
				continue;											// not enough data to look at inner IP

			hip = (struct ip *)(recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;											// not enough data to look at UDP ports

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP && udp->uh_sport == htons(tr->sport) && udp->uh_dport == htons(tr->dport + seq))
			{
				ret = -2;											// hit an intermediate router
				break;
			}
		}
		else if (icmp->icmp_type == ICMP_UNREACH)
		{
			if (icmplen < (int)(8 + sizeof(struct ip)))
				continue;											// not enough data to look at inner IP

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;											// not enough data to look at UDP ports

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP &&
				udp->uh_sport == htons(tr->sport) &&
				udp->uh_dport == htons(tr->dport + seq))
			{
				if (icmp->icmp_code == ICMP_UNREACH_PORT)
					ret = -1;										// have reached destination */
				else
					ret = icmp->icmp_code;
				break;
			}
		}
	}
	if (gettimeofday(tv, NULL) == -1)
		ft_exit("ft_traceroute : gettimeofday error\n");
	return ret;
}
