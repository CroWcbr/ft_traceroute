/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:29:13 by cdarrell          #+#    #+#             */
/*   Updated: 2023/09/04 20:06:28 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./external/libft/include/libft.h"
#include "ft_traceroute.h"

#include <stdio.h>

static void	print_help(void)
{
	printf("Usage\n");
	printf("  ft_traceroute [options] <destination>\n");
	printf("\n");
	printf("Options:\n");
	printf("  --help             help info\n");
	printf("  --version          print version\n");
	printf("  -f <count>         Start from the first_ttl hop (instead from 1)\n");
	printf("  -m <count>         Set the max number of hops (max TTL to bereached). Default is 30\n");
	printf("  -n                 Do not resolve IP addresses to their domain names\n");
	printf("  -p <count>         Set the destination port to use.\n");
	printf("  -q <count>         Set the number of probes per each hop. Default is 3\n");
	printf("  -v                 verbose output\n");
	printf("  <destination>      dns name or ip address\n");
}

static void	init(t_tr* tr)
{
	tr->destination == NULL;
	tr->first_ttl = 1;
	tr->max_ttl = 30;
	tr->no_dns = false;
	tr->dport = 33434;
	tr->nprobes = 3;
	tr->verbose = false;
}

static void	parse_print(t_tr* tr)
{
	printf("---------------PARSRER-------------------\n");
	printf("destination    = \t%s\n", tr->destination);
	printf("first_ttl      = \t%ld\n", tr->first_ttl);
	printf("max_ttl        = \t%ld\n", tr->max_ttl);
	printf("no_dns         = \t%d\n", tr->no_dns);
	printf("dport          = \t%ld\n", tr->dport);
	printf("nprobes        = \t%ld\n", tr->nprobes);
	printf("verbose        = \t%d\n", tr->verbose);
	printf("-----------------------------------------\n");
}

static size_t	find_int(char *opt, char flag)
{
	for (size_t i = 0; i < ft_strlen(opt); ++i)
	{
		if (!ft_isdigit(opt[i]))
		{
			printf("ft_traceroute : Cannot handle `-%c' option with arg `%s'\n", flag, opt);
			exit(1);
		}
	}

	size_t tmp = 0;
	for (size_t i = 0; i < ft_strlen(opt); ++i)
	{
		tmp = tmp * 10 + opt[i] - '0';
	}
	return (tmp);
}

static void	check_input(t_tr *tr)
{
	if (tr->max_ttl > 255)
	{
		printf("ft_traceroute : max hops cannot be more than 255\n");
		exit(1);
	}
	else if (tr->first_ttl > tr->max_ttl)
	{
		printf("ft_traceroute : first hop out of range\n");
		exit(1);
	}
	else if (tr->dport > 65535)
	{
		printf("ft_traceroute : port cannot be more than 65535\n");
		exit(1);
	}
	else if (tr->nprobes > 10)
	{
		printf("ft_traceroute : no more than 10 probes per hop\n");
		exit(1);
	}
}

void	parse(int argc, char **argv, t_tr* tr)
{
	if (argc == 1)
	{
		print_help();
		exit(1);
	}

	init(tr);
	int	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			if (!ft_strcmp(*argv, "--version"))
			{
				printf("Cdarrell best version!\n");
				exit (0);
			}
			else if (!ft_strcmp(*argv, "--help"))
			{
				print_help();
				exit (0);
			}
			else
			{
				size_t j = 1;
				while (j < ft_strlen(argv[i]))
				{
					char	flag = argv[i][j];
					if (flag == 'n' && ++j)
					{
						tr->no_dns = true;
						continue;
					}
					else if (flag == 'v' && ++j)
					{
						tr->verbose = true;
						continue;
					}
					else if (flag == 'f' && ((!argv[i][j + 1] && argv[i][2]) || (j == 1 && argv[i + 1])))
						tr->first_ttl = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
					else if (flag == 'm' && ((!argv[i][j + 1] && argv[i][2]) || (j == 1 && argv[i + 1])))
						tr->max_ttl = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
					else if (flag == 'p' && ((!argv[i][j + 1] && argv[i][2]) || (j == 1 && argv[i + 1])))
						tr->dport = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
					else if (flag == 'q' && ((!argv[i][j + 1] && argv[i][2]) || (j == 1 && argv[i + 1])))
						tr->nprobes = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
					else
					{
						printf("ft_traceroute: Bad option `-%c'\n", argv[i][j]);
						exit(1);
					}
					break;
				}
			}
		}
		else
		{
			tr->destination = argv[i];
		}
		i++;
	}
	check_input(tr);

	if (IS_DEBUG)
		parse_print(tr);
}
