#pragma once

#include "BehemothEngine.h"

namespace ECS
{
	class Registry;
	class Entity;
}

struct PlayerComponent;
struct PCComponent;

class PCSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:
	void IncreasePower(const float deltaTime, PlayerComponent* playerComponent, PCComponent* pcComponent);
	void DecreasePower(const float deltaTime, PlayerComponent* playerComponent, PCComponent* pcComponent);
	void Look(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent);
	void Aim(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent);
	void Fire(ECS::Registry& registry, const ECS::EntityHandle& handle, PlayerComponent* playerComponent, PCComponent* pcComponent);


	void SetArrowMeshVisibility(ECS::Registry& registry, PlayerComponent* playerComponent, bool isVisible);

	void RotateMeshWhileMoving(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComponent);

	void CheckPlayerLanded(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComponent);
};

