#include "pch.h"
#include "HoleTwoScene.h"
#include "HoleThreeScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

#include "Systems/SystemManager.h"

#include "Factories/CameraFactory.h"
#include "Factories/SkySphereFactory.h"
#include "Factories/GameObjectFactory.h"

#include "Scripts/LevelHelper.h"
#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/GoalObject.h"
#include "Scripts/PlatformObject.h"
#include "Scripts/BarrierObject.h"
#include "Scripts/GolfUIHelper.h"
#include "Scripts/PlayerScore.h"

HoleTwoScene::HoleTwoScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
	}
	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	if (ambientLight)
	{
		ambientLight->intensity = 2;
	}
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });


	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));

	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 1.0f, 4));

	bottomOOBTrigger = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });
	registry.AddComponent<Behemoth::OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<Behemoth::StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<Behemoth::ScalingComponent>(bottomOOBTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<Behemoth::MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -20, 10.0f));

	if (Behemoth::MeshComponent* mesh = registry.GetComponent<Behemoth::MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(0, 2, -20), 5, 20, 10, 0);
	registry.AddComponent<Behemoth::TransformComponent>(levelViewEntity);
	registry.AddComponent<Behemoth::RotationComponent>(levelViewEntity);

	par = 2;
	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));

	delayUntilSceneChange = 5.0f;
	changeScene = false;
}

void HoleTwoScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void HoleTwoScene::OnEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void HoleTwoScene::Update(const float deltaTime)
{
	CheckOutOfBound(registry, playerCharacter, bottomOOBTrigger);

	if (Behemoth::Input::IsKeyDown(Behemoth::KeyCode::KC_Space))
	{
		Behemoth::Scene* mainScene = new HoleThreeScene();
		Behemoth::World::GetInstance().ChangeScene(mainScene);
	}
}

void HoleTwoScene::Shutdown()
{

}