#pragma once
#include "WireframeRenderSystem.h"

namespace Behemoth
{
	struct BoundingVolumeComponent;
	struct TransformComponent;

	// This is just a system used for debugging the sphere bounding volume for frustum culling
	// It will not run unless in DEBUG 
	class BoundingVolumeRenderSystem : public WireframeRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;

	private:
		void ProcessBoundingVolume(
			BoundingVolumeComponent* boundingVolumeComp,
			TransformComponent* transform,
			const BMath::Vector3 cameraPosition,
			const BMath::Matrix4x4& viewProjMatrix);
	};
}

