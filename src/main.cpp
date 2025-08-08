#include <iostream>
#include <enet/enet.h>

void serverFunction();
void clientFunction();

int main()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "No multiplayer for you asshole\n";
		return 1;
	}

	std::cout << "Run as (s)erver or (c)lient\n";
	char c;
	std::cin >> c;
	if (c == 's') serverFunction();
	else clientFunction();

	enet_deinitialize();
	return 0;
}
