#include "BLpch.h"
#include "UUID.h"

#include <random>

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_Distribution;

Blanco::UUID::UUID()
	:m_UUID(s_Distribution(s_Engine))
{
}

Blanco::UUID::UUID(uint64_t uuid)
	:m_UUID(uuid)
{
}
