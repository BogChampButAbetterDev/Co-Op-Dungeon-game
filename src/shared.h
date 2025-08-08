#pragma once

#include <enet/enet.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum PacketType
{
	PACKET_POSITION = 0,
	PACKET_WELCOME = 1
};

using vec2 = glm::vec2;

struct PositionPacket
{
	PacketType type = PACKET_POSITION;
	int clientID;
	vec2 position;
};

struct WelcomePacket
{
	PacketType type = PACKET_WELCOME;
	int clientID;
};

struct RemotePlayer
{
	int clientID;
	vec2 position;
};
