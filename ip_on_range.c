#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processIp(char *ip)
{
	char *ip_cpy = strdup(ip);
	char *to_free_ip_copy = ip_cpy;
	char *delim = NULL;
	char *number;
	int ip_numbers[4];
	int length = 0;

	while ((number = strtok_r(ip_cpy, ".", &delim))) {
		ip_numbers[length] = atoi(number);
		ip_cpy = NULL;
		length++;
		if (length >= 4)
			break;
	}
	
	free(to_free_ip_copy);
	return  (( ip_numbers[0] << 24 ) & 0xFF000000) 
		| (( ip_numbers[1] << 16 ) & 0xFF0000) 
		| (( ip_numbers[2] << 8 ) & 0xFF00) 
		|  ( ip_numbers[3] & 0xFF);
}

int getCIDR(int mask)
{
	return (-1) << (32 - mask);
	
}

int get_lowest(addr, mask) {
	return addr & mask;
}

int get_highest(addr, mask) {	
	int lowest = get_lowest(addr, mask);
	return lowest + (~mask);
}

int main(int argc, char *argv[])
{
	int debug = 0;
	if (argc < 4) {
		puts("Parameters missing");
		puts("\n ./checkIpOnRange net_ip mask ip2check [debug]");
		return 1;
	}
	if (argc > 4) {
		debug = 1;
	}

	char *ip_net =	argv[1];
	char *ip_check = argv[3];
	int mask_net = atoi(argv[2]);

	if (debug) {
		printf("Params %d", argc - 1 );

		for (int i = 1; i < argc; i++) {
			printf("\n- [%d] %s", i, argv[i]);
		}
	}

	int ip_net_int = processIp(ip_net);
	int ip_check_int = processIp(ip_check);
	int cidr = getCIDR(mask_net);

	int lowest = get_lowest(ip_net_int, cidr);
	int highest = get_highest(ip_net_int, cidr);
	
	if (debug) {
		printf("\n\nlowest    %d", lowest);
		printf("\nhighest   %d", highest);
		printf("\ncheckedIp %d", ip_check_int);
	}

	if (lowest <= ip_check_int && ip_check_int <= highest) {
		puts("\nSUCCESS! IP in range :)");
	} else {
		puts("\nFAILURE! IP NOT in range :(");
	}

	return 0;
}
