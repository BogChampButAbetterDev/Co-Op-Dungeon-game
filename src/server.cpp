#include "shared.h"
#include <enet/enet.h>
#include <iostream>
#include <unordered_map>

struct ServerPlayer
{
	int clientID;
	vec2 pos;
};

std::unordered_map<ENetPeer*, ServerPlayer> players;

int nextClientID = 1;

void serverFunction()
{
	ENetAddress address{};
	address.host = ENET_HOST_ANY;
	address.port = 3987;

	ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
	if (!server)
	{
		std::cerr << "Failed to create server\n";
	}

	std::cout << "Server started\n";

	ENetEvent event;
	while (true)
	{
		while (enet_host_service(server, &event, 10) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				std::cout << "Player joined!\n";
				int id = nextClientID++;

				players[event.peer] = ServerPlayer{ id, vec2{0, 0} };

				WelcomePacket pkt;
				pkt.type = PACKET_WELCOME;
				pkt.clientID = id;

				ENetPacket* welcome = enet_packet_create(&pkt, sizeof(pkt), ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, welcome);

				break;
			}

			case ENET_EVENT_TYPE_RECEIVE:
			{
				if (event.packet->dataLength >= sizeof(PositionPacket))
				{
					PositionPacket pkt;
					memcpy(&pkt, event.packet->data, sizeof(PositionPacket));

					if (players.find(event.peer) != players.end())
					{
						players[event.peer].pos = pkt.position;
						pkt.clientID = players[event.peer].clientID;

						for (auto& [peer, pos] : players)
						{
							if (peer->state == ENET_PEER_STATE_CONNECTED)
							{
								ENetPacket* packet = enet_packet_create(&pkt, sizeof(pkt), ENET_PACKET_FLAG_UNSEQUENCED);
								enet_peer_send(peer, 0, packet);
							}
						}
					}
				}

				enet_packet_destroy(event.packet);
				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				std::cout << "Player left!\n";
				players.erase(event.peer);
				break;
			}
			}
		}
	}

	enet_host_destroy(server);
}
