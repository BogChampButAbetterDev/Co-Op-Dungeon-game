#include <iostream>
#include <enet/enet.h>
#include <Windows.h>

void serverFunction();
void clientFunction();

int main()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "No multiplayer for you asshole\n";
		return 1;
	}

	// use this to figure out where to search for files
	if (IsDebuggerPresent())
	{
		std::cout << "running in visual studio\n";
		serverFunction();
	}
	else
	{
		std::cout << "running from exe\n";
		clientFunction();
	}

	enet_deinitialize();
	return 0;
}
