#pragma once
#include "animal.h"

class Cat : public Animal {
public:
	static DWORD WINAPI animalReceive(LPVOID);
};
