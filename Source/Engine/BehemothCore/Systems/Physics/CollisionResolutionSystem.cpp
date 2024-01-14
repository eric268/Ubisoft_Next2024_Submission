#include "pch.h"
#include "CollisionResolutionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{
	void CollisionResolutionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, transformComp, velocityComp, collisionData] : registry.Get<TransformComponent, VelocityComponent,  CollisionDataComponent>())
		{
			BMath::Vector3 offsetPosition = -velocityComp->velocity * deltaTime;

			for (const auto& collision : collisionData->data)
			{
				offsetPosition += collision.data.collisionNormal * collision.data.penetrationDepth;
			}

			transformComp->worldTransform._41 += offsetPosition.x;
			transformComp->worldTransform._42 += offsetPosition.y;
			transformComp->worldTransform._43 += offsetPosition.z;

			transformComp->isDirty = true;
		}
	}
}