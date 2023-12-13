#pragma once

#include "ECS/System.h"
#include "Math/MathCore.h"

#include <vector>

namespace Behemoth
{
	class MeshComponent;
	class DirectionalLightComponent;
	class PointLightComponent;
	class TransformComponent;
	class Primitives;
	
	class LightingSystem : ECS::System
	{
	public:
		virtual void Run(ECS::Registry& registry) override;

	private:
		void CalculateDirectionalLights(const DirectionalLightComponent* light, const Math::Vector3& cameraPos);
		void CalculatePointLights(const PointLightComponent* light, const Math::Vector3& cameraPos, const Math::Vector3& lightPos, const Math::Matrix4x4& viewMatrix);
		Math::Vector3 GetPrimitivePosition(Primitives* primitive);

		Math::Vector3 CalculateDiffuseLighting(const Math::Vector3& surfaceNormal,
											   const Math::Vector3& lightDir, 
											   const Math::Vector3& primitiveDiffusion, 
											   const Math::Vector3& lightColor, 
											   const float lightIntensity);

		Math::Vector3 CalculateSpecularLighting(Primitives* primitive,
			const Math::Vector3& cameraPos,
			const Math::Vector3& surfaceNormal,
											    const Math::Vector3& lightDir, 
											    const Math::Vector3& lightColor,
											    const float lightIntensity);
	};
}
