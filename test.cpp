#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"

int main()
{
	init();
	passwordGate();
	return 0;
}

void passwordGate(){
	char[15] serverAddr=("130.195.6.196");
	char[24] message=("Please");
	char[24] password;
	int connect_to_server(serverAddr, 1024);
	int send_to_server(message);
	int recieve_from_server(password);
	int send_to_server(password);
}
