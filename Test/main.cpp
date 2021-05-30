// Driver code
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include "cat.h"
#include "fox.h"

int main()
{
	// Instantiate objects
	Fox fox; //<-- Server
	Cat cat; //<-- Client



	// Multi-threading to allow both objects to run concurrently
	HANDLE _fox = CreateThread(NULL, 0, cat.animalCommunicate, NULL, 0, NULL);
	HANDLE _cat = CreateThread(NULL, 0, fox.animalCommunicate, NULL, 0, NULL);

	// Wait for them to finish
	WaitForSingleObject(_fox, INFINITE);
	WaitForSingleObject(_cat, INFINITE);

	return 0;
}
