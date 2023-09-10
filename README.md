# ft_traceroute
make your own traceroute

## Mandatory
- traceroute - print the route packets trace to network host

## Bonus
- DNS management
- add options:
	- --help             help info
	- --version          print version
	- -f <count>         Start from the first_ttl hop (instead from 1)
	- -m <count>         Set the max number of hops (max TTL to bereached). Default is 30
	- -n                 Do not resolve IP addresses to their domain names
	- -p <count>         Set the destination port to use.
	- -q <count>         Set the number of probes per each hop. Default is 3

## Clone of project
- git clone --recursive ....this_project
- or git clone ....this_project + git submodule update --init external/libft

## References
- W.R. Stevens - UNIX Network Programming Volume 1
