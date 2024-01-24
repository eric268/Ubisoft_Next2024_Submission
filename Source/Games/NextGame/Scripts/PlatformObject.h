#pragma once
#include "BehemothEngine.h"
#include "Math/Quaternion.h"

namespace Behemoth
{
	class PhysicsMaterial;
}

class PlatformObject
{
public:
	static ECS::EntityHandle CreateGrassPlatform(
		ECS::Registry& registry,
		const BMath::Vector3& position,
		const BMath::Vector3& scale,
		const BMath::Quaternion& q = BMath::Quaternion(),
		bool isStatic = true);

	static ECS::EntityHandle CreateRockPlatform(
		ECS::Registry& registry,
		const BMath::Vector3& position,
		const BMath::Vector3& scale,
		const BMath::Quaternion& q = BMath::Quaternion(),
		bool isStatic = true);

	static ECS::EntityHandle CreateSandPlatform(
		ECS::Registry& registry,
		const BMath::Vector3& position,
		const BMath::Vector3& scale,
		const BMath::Quaternion& q = BMath::Quaternion(),
		bool isStatic = true);

private:
	static ECS::EntityHandle CreatePlatform(
		ECS::Registry& registry,
		const Behemoth::PhysicsMaterial& physicsMaterial,
		const std::string& modelPath,
		const std::string& texturePath,
		const std::string& name,
		const BMath::Vector2& scale,
		bool isRotated);
};

