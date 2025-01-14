#include "pch.h"
#include "GameScene.h"
#include "Components/UIComponents.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Scripts/PlayerScore.h"
#include "Factories/GameObjectFactory.h"
#include "Components/RenderComponents.h"
#include "Misc/GameObjectHelper.h"

std::unordered_map<int, std::string> GameScene::holeResultsText =
{
	{-3, "Albatross"},
	{-2, "Eagle"},
	{-1, "Birdie"},
	{0, "Par"},
	{1, "Bogey"},
	{2, "Double Bogey"},
	{3, "Triple Bogey"}
};

GameScene::GameScene() : par(0), delayUntilSceneChange(0.0f), changeScene(false), goalComp(nullptr) {}

GameScene::GameScene(ECS::Registry& registry, int p, float delay) : par(p), delayUntilSceneChange(delay), changeScene(false), goalComp(nullptr)
{
	CreateOOBEntity(registry);
}

void GameScene::Update(const float deltaTime)
{
	CheckOutOfBound(registry, playerHandle, oobTriggerHandle);

	if (CheckLevelComplete(registry, playerHandle))
	{
		OnHoleComplete(registry, playerHandle, par);

		ECS::EntityHandle changeSceneEntity = registry.CreateEntity("Change scene entity");
		registry.AddComponent<Behemoth::TimerComponent>(changeSceneEntity, delayUntilSceneChange, [this]()
		{
			changeScene = true;
		});
	}
}

void GameScene::CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle)
{
	if (Behemoth::TriggerDataComponent* triggerData = registry.GetComponent<Behemoth::TriggerDataComponent>(playerHandle))
	{
		PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
		if (!playerComponent)
		{
			LOGMESSAGE(Error, "Null player component found");
			return;
		}

		if (playerComponent->isRespawning)
		{
			// Player respawn is already being processed
			return;
		}

		for (const auto& d : triggerData->data)
		{
			if (d.otherHandle.ID == oobHandle.ID)
			{
				registry.AddComponent<PlayerFallComponent>(playerHandle, 3.0f);
			}
		}
	}
}

bool GameScene::CheckLevelComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle)
{
	if (!goalComp)
	{
		LOGMESSAGE(Error, "Unable to find goal component");
		return false;
	}

	if (Behemoth::CollisionDataComponent* collisionData = registry.GetComponent<Behemoth::CollisionDataComponent>(playerHandle))
	{
		for (const auto& d : collisionData->data)
		{
			for (const auto& goalColliderHandle : goalComp->goalColliderHandles)
			{
				if (d.otherHandle.ID == goalColliderHandle.ID)
				{
					PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
					if (!playerComponent)
					{
						LOGMESSAGE(Error, "Unable to find player component");
						return false;
					}
					// Level complete process already happening
					if (playerComponent->levelComplete)
					{
						return false;
					}

					playerComponent->levelComplete = true;
					return true;
				}
			}
		}
	}
	return false;
}

void GameScene::OnHoleComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle, int par)
{
	PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
	if (!playerComponent)
	{
		LOGMESSAGE(Error, "Unable to find player component");
		return;
	}

	PlayerScore::AddScore(par, playerComponent->strokesUsed);

	levelCompleteTextHandle = registry.CreateEntity("Level complete text");
	registry.AddComponent<Behemoth::TextComponent>(levelCompleteTextHandle, GetHoleResultText(playerComponent->strokesUsed, par), BMath::Vector2(0, 0.5f));

	if (Behemoth::VelocityComponent* velocity = registry.GetComponent<Behemoth::VelocityComponent>(playerHandle))
	{
		velocity->velocity = BMath::Vector3::Zero();
	}

	if (Behemoth::RigidBodyComponent* rigidBodyComponent = registry.GetComponent<Behemoth::RigidBodyComponent>(playerHandle))
	{
		rigidBodyComponent->affectedByGravity = false;
	}
}

// Possibly move these two functions into separate factory classes
ECS::EntityHandle GameScene::CreateOOBEntity(ECS::Registry& registry)
{
	oobTriggerHandle = registry.CreateEntity("Out of bounds trigger");
	registry.AddComponent<Behemoth::TransformComponent>(oobTriggerHandle);
	registry.AddComponent<Behemoth::AABBColliderComponent>(oobTriggerHandle, BMath::Vector3(1.0f), true, true);
	registry.AddComponent<Behemoth::StaticComponent>(oobTriggerHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(oobTriggerHandle, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<Behemoth::MoveComponent>(oobTriggerHandle, BMath::Vector3(0, -20, 10.0f));

	return oobTriggerHandle;
}

ECS::EntityHandle GameScene::CreateGoalObject(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle)
{
	ECS::EntityHandle goalHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png", "Goal Game Object");
	registry.AddComponent<Behemoth::MoveComponent>(goalHandle, location);
	registry.AddComponent<Behemoth::RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(rotationAngle), BMath::Vector3(0, 1, 0)));
	registry.AddComponent<Behemoth::StaticComponent>(goalHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(goalHandle, scale);

	ECS::EntityHandle collider1 = CreateGoalCollider(registry, BMath::Vector3(0, 0.75f, -0.25f), BMath::Vector3(0.85f));
	ECS::EntityHandle collider2 = CreateGoalCollider(registry, BMath::Vector3(0, -2, 1.5f), BMath::Vector3(0.3f));
	ECS::EntityHandle collider3 = CreateGoalCollider(registry, BMath::Vector3(-3.25, 0.5f, -1.0f), BMath::Vector3(0.3f));
	ECS::EntityHandle collider4 = CreateGoalCollider(registry, BMath::Vector3(3.25, 0.5f, -1.0f), BMath::Vector3(0.3f));

	Behemoth::GameObjectHelper::AddChildObject(registry, goalHandle, collider1);
	Behemoth::GameObjectHelper::AddChildObject(registry, goalHandle, collider2);
	Behemoth::GameObjectHelper::AddChildObject(registry, goalHandle, collider3);
	Behemoth::GameObjectHelper::AddChildObject(registry, goalHandle, collider4);

	if (GoalComponent* goalComp = registry.AddComponent<GoalComponent>(goalHandle))
	{
		goalComp->goalColliderHandles.reserve(4);
		goalComp->goalColliderHandles.push_back(collider1);
		goalComp->goalColliderHandles.push_back(collider2);
		goalComp->goalColliderHandles.push_back(collider3);
		goalComp->goalColliderHandles.push_back(collider4);
	}

	return goalHandle;
}

ECS::EntityHandle GameScene::CreateGoalCollider(ECS::Registry& registry, BMath::Vector3 offset, BMath::Vector3 scale, bool drawColliders)
{
	ECS::EntityHandle collider1 = registry.CreateEntity("Goal Collider");
	registry.AddComponent<Behemoth::TransformComponent>(collider1);
	registry.AddComponent<Behemoth::SphereColliderComponent>(collider1);
	registry.AddComponent<Behemoth::ScalingComponent>(collider1, scale);
	registry.AddComponent<Behemoth::MoveComponent>(collider1, offset);
	registry.AddComponent<Behemoth::RotationComponent>(collider1);
	registry.AddComponent<Behemoth::StaticComponent>(collider1);

#ifdef DEBUG
	if (drawColliders)
	{
		registry.AddComponent<Behemoth::MeshInitializeComponent>(collider1);
		registry.AddComponent<Behemoth::WireframeComponent>(collider1, "sphere.obj", BMath::Vector3(1.0f));
	}
#endif 
	return collider1;
}

std::string GameScene::GetHoleResultText(int numStrokes, int par)
{
	int result = numStrokes - par;

	if (numStrokes == 1)
	{
		return std::string("Hole in One! Well Done!");
	}
	else if (result < 4)
	{
		return holeResultsText[result];
	}
	else
	{
		return std::string("+" + std::to_string(result));
	}
}