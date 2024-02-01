#pragma once
#include "World/Scene.h"
#include "BehemothEngine.h"
#include "GameComponents/Level/GoalComponent.h"
#include <unordered_map>

class GameScene : public Behemoth::Scene
{
public:
	std::string GetHoleResultText(int numStrokes, int par);

	void CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle);
	bool CheckLevelComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle);
	void OnHoleComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle, int par);

	ECS::EntityHandle CreateGoalObject(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle);
	ECS::EntityHandle CreateOOBEntity(ECS::Registry& registry);

protected:
	GameScene();
	GameScene(ECS::Registry& registry, int p, float delay = 3.0f);

	virtual void Update(const float deltaTime) override;
	virtual void ConstructEnvironment(ECS::Registry& registry) = 0;
	ECS::EntityHandle CreateGoalCollider(ECS::Registry& registry, BMath::Vector3 offset, BMath::Vector3 scale, bool drawColliders = false);

	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle oobTrigger;
	ECS::EntityHandle playerCharacter;
	ECS::EntityHandle teeOffPlatform;
	ECS::EntityHandle levelCompleteText;
	ECS::EntityHandle goalHandle;

	GoalComponent* goalComponent;

	static std::unordered_map<int, std::string> holeResultsText;

	int par;
	float delayUntilSceneChange;
	bool changeScene;
};

