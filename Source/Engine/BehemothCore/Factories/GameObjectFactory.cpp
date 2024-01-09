#include "pch.h"
#include "GameObjectFactory.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
	ECS::EntityHandle GameObjectFactory::CreateGameObject(ECS::Registry& registry, std::string modelFilePath, std::string texturePath, std::string entityName)
	{
		ECS::EntityHandle entity = registry.CreateEntity(entityName);
		registry.AddComponent<MeshComponent>(entity, modelFilePath, texturePath);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MeshInitalizeComponent>(entity);
		registry.AddComponent<VelocityComponent>(entity);
		registry.AddComponent<RotationComponent>(entity);
		registry.AddComponent<BoundingVolumeComponent>(entity, 1.5f, false);

		return entity;
	}

	ECS::EntityHandle GameObjectFactory::AddChildObject(ECS::Registry& registry, ECS::EntityHandle parentHandle, std::string modelFilePath, std::string texturePath, std::string entityName)
	{
		if (parentHandle.ID == NULL_ENTITY || !registry.IsValidHandle(parentHandle))
		{
			LOG_MESSAGE(MessageType::Error, "Invalid parent handle found");
			return NULL_ENTITY;
		}

		// If parent doesn't already have a parent component, then add it
		ParentComponent* parentComp = registry.GetComponent<ParentComponent>(parentHandle);
		if (!parentComp)
		{
			registry.AddComponent<ParentComponent>(parentHandle);
			parentComp = registry.GetComponent<ParentComponent>(parentHandle);
			if (!parentComp)
			{
				LOG_MESSAGE(MessageType::Error, "Error adding parent component to entity: " + registry.GetName(parentHandle));
				return NULL_ENTITY;
			}
		}

		ECS::EntityHandle childHandle = CreateGameObject(registry, modelFilePath, texturePath, entityName);
		registry.AddComponent<ChildComponent>(childHandle, parentHandle);
		parentComp->childHandles.push_back(childHandle);

		return childHandle;
	}
}