#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"

void passwordGate(){
	char serverAddr[15]={'1','3','0','.','1','9','5','.','6','.','1','9','6'};
	char message[24]={'P','l','e','a','s','e'};
	char password[24];
	connect_to_server(serverAddr, 1024);
	send_to_server(message);
	receive_from_server(password);
	printf(password);
	send_to_server(password);
}

int main()
{
	init();
	passwordGate();
	return 0;
}
