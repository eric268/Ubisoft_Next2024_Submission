#include "pch.h"
#include "MeshRenderSystem.h"
#include "Misc/Log.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"
#include "ECS/Entity.h"
#include "Renderer/Renderer.h"
#include "Renderer/MeshLoader.h"
#include "Application/ResourceManager.h"

namespace Behemoth
{
	void MeshRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple1, std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple2)
			{
				return std::get<1>(tuple1)->mesh.meshPrimitives.size() < std::get<1>(tuple2)->mesh.meshPrimitives.size();
			});

		// ** Order of multiplication matters here **
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, meshComp, transformComp] : components)
		{
			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume->volumeRadius))
			{
				continue;
			}

			ProcessMesh(meshComp->mesh, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix, transformComp->isDirty);
			transformComp->isDirty = false;
		}

		Renderer::GetInstance().FreeResourceOverflow();
	}


	void MeshRenderSystem::ProcessMesh(Mesh& mesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix, bool isDirty)
	{
		const MeshData& meshData = mesh.meshData;

		ReserveResources(meshData.totalPrimitives);

		if (isDirty && cachedMeshName != meshData.modelFileName)
		{
			cachedMeshName = meshData.modelFileName;
			cachedVerticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);
		}

		int numVerticies = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
				numVerticies = 4;

			Primitives& primitive = mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty
			if (isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformMatrix * Math::Vector4(cachedVerticies[vertexIndex].vertex, 1.0f);
					vertexIndex++;
				}
			}

			if (CullBackFace(cameraPosition, primitive.verticies))
			{
				continue;
			}

			Math::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(Math::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}

			AddPrimitiveToRenderer(primitive, numVerticies, renderVerts);
		}
	}

	bool MeshRenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{
		Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
	}


	void MeshRenderSystem::AddPrimitiveToRenderer(Primitives& primitive, const int numVerticies, const Math::Vector4 vertex[])
	{
		primitive.depth = GetPrimitiveDepth(numVerticies, vertex);
		primitive.SetSpriteVerticies(numVerticies, vertex);
		Renderer::GetInstance().AddPrimitive(&primitive);
	}


	void MeshRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReservePrimitives(numPrimitives);
	}

	float MeshRenderSystem::GetPrimitiveDepth(const int numVerticies, const Math::Vector4 vertex[])
	{
		float depth = 0.0f;
		for (int j = 0; j < numVerticies; j++)
		{
			depth += vertex[j].z;
		}
		return depth / numVerticies;
	}
}