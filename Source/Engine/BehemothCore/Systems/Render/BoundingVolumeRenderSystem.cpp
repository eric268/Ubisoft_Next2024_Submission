#include "pch.h"
#include "BoundingVolumeRenderSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/CameraHelper.h"

namespace Behemoth
{
	void BoundingVolumeRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* mainCameraComp = nullptr;
		BMath::Vector3 mainCameraPosition = BMath::Vector3::Zero();

		for (const auto& [
			entityHandle,
				cameraComp,
				transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (cameraComp->isMain)
			{
				mainCameraComp = cameraComp;
				mainCameraPosition = transformComp->worldPosition;
				break;
			}
		}

		assert(mainCameraComp);

		BMath::Matrix4x4 viewProjMatrix = mainCameraComp->projMatrix * mainCameraComp->viewMatrix;

		for (const auto& [
			entityHandle,
				boundingVolumeComp,
				transformComp] : registry.Get<BoundingVolumeComponent, TransformComponent>())
		{
			if (!boundingVolumeComp->isVisible)
			{
				continue;
			}

			if (!IsBoundingVolumeInFrustrum( mainCameraComp, transformComp, boundingVolumeComp))
			{
				continue;
			}

			ProcessBoundingVolume(boundingVolumeComp, transformComp, mainCameraPosition, viewProjMatrix);
		}
	}

	void BoundingVolumeRenderSystem::ProcessBoundingVolume(
		BoundingVolumeComponent* boundingComp,
		TransformComponent* transformComp,
		const BMath::Vector3 cameraPosition, 
		const BMath::Matrix4x4& viewProjMatrix)
	{
		// Scale the sphere mesh by bounding radius
		float maxScale = std::max(transformComp->worldScale[0], std::max(transformComp->worldScale[1], transformComp->worldScale[2]));
		BMath::Matrix4x4 boundingMatrix = BMath::Matrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			boundingMatrix.data[i][i] = boundingComp->radius * maxScale;
		}
		boundingMatrix._41 = transformComp->worldTransform._41 + boundingComp->localPosition[0];
		boundingMatrix._42 = transformComp->worldTransform._42 + boundingComp->localPosition[1];
		boundingMatrix._43 = transformComp->worldTransform._43 + boundingComp->localPosition[2];

		ProcessWireframe(boundingComp->mesh, boundingMatrix, viewProjMatrix, true);
	}
}