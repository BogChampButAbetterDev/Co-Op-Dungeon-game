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
	}
	else
	{
		std::cout << "running from exe\n";
	}

	std::cout << "Run as (s)erver or (c)lient? ";
	char choice;
	std::cin >> choice;

	if (choice == 's' || choice == 'S') {
		serverFunction();
	}
	else {
		clientFunction();
	}

	enet_deinitialize();
	return 0;

	enet_deinitialize();
	return 0;
}
