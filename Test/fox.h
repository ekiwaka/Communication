#pragma once
#include "animal.h"

class Fox : public Animal {

public:
	static DWORD WINAPI animalReceive(LPVOID);
	static DWORD WINAPI animalCommunicate(LPVOID);
};