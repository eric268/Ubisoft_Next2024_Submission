#pragma once

#include "BehemothEngine.h"

struct BarrierFactory
{
	static ECS::EntityHandle CreateObstacle(
		ECS::Registry& registry,
		const BMath::Vector3& location,
		const BMath::Vector3& scale,
		const BMath::Quaternion& q,
		bool isStatic = true);
};

