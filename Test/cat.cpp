#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <winsock2.h>
#include "translate.h"
#include "cat.h" 

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

DWORD WINAPI Cat::animalReceive(LPVOID lpParam)
{
	// Created buffer[] to receive message
	char buffer[1024] = { 0 };
	//string check = getName();

	// Create socket for animal
	SOCKET animal = *(SOCKET*)lpParam;

	// Pointer to the first element of the reversed array returned by translate()
	char* reversed;

	// animal executes continuously
	while (true) {

		// If received buffer gives error then return -1
		if (recv(animal, buffer,
			sizeof(buffer), 0)
			== SOCKET_ERROR) {
			cout << "recv function failed with error: "
				<< WSAGetLastError()
				<< endl;
			return -1;
		}

		// If cat exits
		if (strcmp(buffer, "exit") == 0) {
			cout << "Conversation has ended."
				<< endl;

			//exit(1);
			return 1;
		}

		// Print the message that was stored in buffer
		cout << "\nMessage received by Cat: " << buffer;
		cout << "\nTranslated message: ";
		reversed = translate(buffer);

		for (int i = 0; i < strlen(buffer); i++) {
			cout << *(reversed + i);
		}

		cout << "\n\n";


		// Clear buffer message
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
};
