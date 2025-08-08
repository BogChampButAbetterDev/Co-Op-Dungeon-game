#pragma once

#include <enet/enet.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum PacketType
{
	PACKET_POSITION = 0
};

using vec2 = glm::vec2;

struct PositionPacket
{
	PacketType type = PACKET_POSITION;
	int clientID;
	vec2 position;
};
