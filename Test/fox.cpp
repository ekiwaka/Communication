// Creates the Server, hereby referenced as Fox
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <winsock2.h>
#include "translate.h"
#include "fox.h" 

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

DWORD WINAPI Fox::animalReceive(LPVOID lpParam)
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

		// If fox exits
		if (strcmp(buffer, "exit") == 0) {
			cout << "Conversation has ended."
				<< endl;

			//exit(1);
			return 1;
		}

		// Print the message that was stored in buffer
		cout << "\nMessage received by Fox: " << buffer;
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

// Function to create sockets
DWORD WINAPI Fox::animalCommunicate(LPVOID lpParam)
{
	WSADATA WSAData;

	// Create sockets for fox (server) and cat (client)
	SOCKET fox, foxFriend;

	// Socket addresses for fox and cat
	SOCKADDR_IN foxAddr, foxFriendAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// Making fox
	fox = socket(AF_INET, SOCK_STREAM, 0);

	// If invalid socket created, return -1
	if (fox == INVALID_SOCKET) {
		cout << "Socket creation failed with error:"
			<< WSAGetLastError() << endl;
		return -1;
	}
	foxAddr.sin_addr.s_addr = INADDR_ANY;
	foxAddr.sin_family = AF_INET;
	foxAddr.sin_port = htons(5555);

	// If socket error occurred, return -1
	if (bind(fox,
		(SOCKADDR*)&foxAddr,
		sizeof(foxAddr))
		== SOCKET_ERROR) {
		cout << "Bind function failed with error: "
			<< WSAGetLastError() << endl;
		return -1;
	}

	// Get the request from fox
	if (listen(fox, 0)
		== SOCKET_ERROR) {
		cout << "Listen function failed with error:"
			<< WSAGetLastError() << endl;
		return -1;
	}

	cout << "Listening for incoming connections...." << endl;

	// Intialise address for cat socket
	int catAddrSize = sizeof(foxFriendAddr);

	// If connection established
	if ((foxFriend = accept(fox,
		(SOCKADDR*)&foxFriendAddr,
		&catAddrSize))
		!= INVALID_SOCKET) {
		cout << "Ready to chat!" << endl
			<< "Enter \"exit\" to disconnect"
			<< endl;


		DWORD threadID;

		// Create Thread to receive data by fox
		HANDLE threadReceive = CreateThread(NULL,
			0,
			animalReceive,
			&foxFriend,
			0,
			&threadID);

		// If created thread is not created
		if (threadReceive == NULL) {
			cout << "Thread Creation Error: "
				<< WSAGetLastError() << endl;
		}

		// Create Thread to send data from fox
		HANDLE threadSend = CreateThread(NULL,
			0,
			animalSend,
			&foxFriend,
			0,
			&threadID);

		// If created thread is not created
		if (threadSend == NULL) {
			cout << "Thread Creation Error: "
				<< WSAGetLastError() << endl;
		}

		// Wait for threads to finish
		WaitForSingleObject(threadReceive, INFINITE);
		WaitForSingleObject(threadSend, INFINITE);

		// Close the socket
		closesocket(foxFriend);

		// If socket closing failed.
		if (closesocket(fox)
			== SOCKET_ERROR) {
			cout << "Close socket failed with error: "
				<< WSAGetLastError() << endl;
			return -1;
		}
		WSACleanup();
	}
}
