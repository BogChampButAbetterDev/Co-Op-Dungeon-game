#include "shared.h"
#include <enet/enet.h>
#include <SDL.h>
#include <GL/gl.h>
#include <iostream>
//#include <vector>
#include <unordered_map>

vec2 myPos{ 0, 0 };
std::unordered_map<int, RemotePlayer> otherPlayers;

int myClientID = -1;

void drawSquare(vec2 pos, float r, float  g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(pos.x - 0.05f, pos.y - 0.05f);
	glVertex2f(pos.x + 0.05f, pos.y - 0.05f);
	glVertex2f(pos.x + 0.05f, pos.y + 0.05f);
	glVertex2f(pos.x - 0.05f, pos.y + 0.05f);
	glEnd();
}

void clientFunction()
{
	ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
	ENetAddress address;
	ENetPeer* peer;
	enet_address_set_host(&address, "192.168.56.1");
	address.port = 3987;

	peer = enet_host_connect(client, &address, 2, 0);
	if (!peer)
	{
		std::cerr << "Connection failed\n";
		return;
	}

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	bool running = true;
	SDL_Event sdlEvent;

	while (running)
	{
		while (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT) running = false;
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) myPos.y += 0.01f;
		if (state[SDL_SCANCODE_S]) myPos.y -= 0.01f;
		if (state[SDL_SCANCODE_A]) myPos.x -= 0.01f;
		if (state[SDL_SCANCODE_D]) myPos.x += 0.01f;

		if (myClientID != -1)
		{
			PositionPacket pkt;
			pkt.clientID = myClientID;
			pkt.position = myPos;
			ENetPacket* packet = enet_packet_create(&pkt, sizeof(pkt), ENET_PACKET_FLAG_UNSEQUENCED);
			enet_peer_send(peer, 0, packet);
		}

		ENetEvent event;
		otherPlayers.clear();
		while (enet_host_service(client, &event, 0) > 0)
		{
			if (event.type == ENET_EVENT_TYPE_RECEIVE && event.packet->dataLength >= sizeof(WelcomePacket))
			{
				PacketType type;
				memcpy(&type, event.packet->data, sizeof(PacketType));

				if (type == PACKET_WELCOME)
				{
					WelcomePacket pkt;
					memcpy(&pkt, event.packet->data, sizeof(WelcomePacket));
					myClientID = pkt.clientID;
					std::cout << "Recieved myclientID: " << myClientID << "\n";
				}
			}
			if (event.type == ENET_EVENT_TYPE_RECEIVE && event.packet->dataLength >= sizeof(PositionPacket))
			{
				PositionPacket pkt;
				memcpy(&pkt, event.packet->data, sizeof(PositionPacket));
				
				if (pkt.clientID != myClientID) {
					otherPlayers[pkt.clientID] = { pkt.clientID, pkt.position };
				}
			}
		}

		enet_packet_destroy(event.packet); 

		glClear(GL_COLOR_BUFFER_BIT);
		drawSquare(myPos, 0, 1, 0);

		for (auto& [id, rp] : otherPlayers)
		{
			drawSquare(rp.position, 1, 0, 0); 
		}

		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}

	SDL_Quit();
	enet_host_destroy(client);
}
