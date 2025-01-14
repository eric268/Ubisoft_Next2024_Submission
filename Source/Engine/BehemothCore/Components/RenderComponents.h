#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Geometry/Mesh.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct MeshComponent : public ECS::Component
	{
		MeshComponent() : modelFileName(""), textureFileName(""), mesh(), isVisible(true), affectedByLighting(true) {}

		MeshComponent(
			const std::string& modelName,
			const std::string& textureName,
			const BMath::Vector2 uvScale = { 1.0f,1.0f },
			bool visible = true,
			bool affectedByLighting = true)
			:
			modelFileName(modelName),
			textureFileName(textureName),
			mesh(modelName, textureName, uvScale, affectedByLighting),
			isVisible(visible),
			affectedByLighting(affectedByLighting)
		{}
														
		bool isVisible;
		bool affectedByLighting;
		Mesh mesh;
		std::string modelFileName;
		std::string textureFileName;
	};

	struct WireframeComponent : public ECS::Component
	{
		WireframeComponent() :
			mesh(),
			modelFileName(),
			scale(BMath::Vector3(1.0f)),
			allowRotation(false),
			isVisible(false),
			wireframeColor(BMath::Vector3(1.0f))
		{}
		WireframeComponent(
			const std::string& modelName,
			BMath::Vector3 scale = BMath::Vector3::One(),
			bool allowRotation = true,
			bool visible = true,
			BMath::Vector3 color = BMath::Vector3(0.0f, 1.0f, 0.0f))
			:
			mesh(modelName),
			modelFileName(modelName),
			scale(scale),
			allowRotation(allowRotation),
			isVisible(visible),
			wireframeColor(color)
		{}
													
		Mesh mesh;
		std::string modelFileName;
		BMath::Vector3 scale;
		bool allowRotation = true;
		bool isVisible;
		BMath::Vector3 wireframeColor;
	};

	struct BoundingVolumeComponent : public ECS::Component
	{
																			
		BoundingVolumeComponent() : 
			mesh("sphere.obj"),
			localPosition(BMath::Vector3::Zero()),
			radius(1.0f),
			isVisible(false)
		{}
		BoundingVolumeComponent(bool visible) :
			mesh("sphere.obj"),
			localPosition(BMath::Vector3::Zero()),
			radius(1.0f),
			isVisible(visible)
		{}
		BoundingVolumeComponent(BMath::Vector3 pos, float radius, bool visible = false) :
			mesh("sphere.obj"),
			localPosition(pos),
			radius(radius),
			isVisible(visible) 
		{}
		
		Mesh mesh;
		BMath::Vector3 localPosition;
		float radius;
		bool isVisible;
	};

	struct DebugLineComponent : public ECS::Component
	{
		using Point = BMath::Vector3;

		DebugLineComponent() :
			startPoint(BMath::Vector3()),
			endPoint(BMath::Vector3()),
			lifetime(2.0f),
			displayCounter(0.0f),
			color(BMath::Vector3(0.0f, 1.0f, 0.0f))
		{}

		DebugLineComponent(
			Point start,
			Point end,
			float displayTime,
			BMath::Vector3 c = BMath::Vector3(0.0f, 1.0f, 0.0f))
			:
			startPoint(start),
			endPoint(end),
			lifetime(displayTime),
			displayCounter(0.0f),
			color(c)
		{}

		float lifetime;
		float displayCounter;
		Point startPoint;
		Point endPoint;
		BMath::Vector3 color;
	};

	struct SkySphereComponent : public ECS::Component
	{
		SkySphereComponent() : modelFileName(""), textureFileName(""), mesh(), isVisible(false), isInitalized(false) {}

		SkySphereComponent(const std::string& textureName,
			const BMath::Vector2 uvScale = { 1.0f,1.0f },
			bool visible = true)
			:
			modelFileName("SkySphere.obj"),
			textureFileName(textureName),
			mesh("SkySphere.obj", textureName, uvScale),
			isVisible(visible),
			isInitalized(false)
		{}

		bool isVisible;
		bool isInitalized;
		Mesh mesh;
		std::string modelFileName;
		std::string textureFileName;
		std::vector<BMath::Vector3> vertices;
	};
}