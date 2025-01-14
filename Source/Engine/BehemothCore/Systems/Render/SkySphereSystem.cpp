#include "pch.h"
#include "SkySphereSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Core/ResourceManager.h"
#include "Geometry/Mesh.h"
#include "Renderer/Renderer.h"
#include "Core/Stopwatch.h"

namespace Behemoth
{
	void SkySphereSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* cameraComp = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransformComp = CameraHelper::GetMainCameraTransform(registry);

		std::uint32_t renderSlotIndex = Renderer::GetInstance().GetCurrentPrimitiveCount();

		for (const auto& [
			entityHandle, 
				transformComp,
				skySphereComp] : registry.Get<TransformComponent, SkySphereComponent>())
		{
			if (!skySphereComp->isInitalized)
			{
				InitializeSphere(skySphereComp);
			}

			FollowCamera(transformComp, cameraTransformComp->worldPosition);
			transformComp->isDirty = true;

			BMath::Matrix4x4 viewProjMatrix = cameraComp->projMatrix * cameraComp->viewMatrix;

			ProcessSphere(transformComp, skySphereComp, cameraTransformComp, viewProjMatrix, renderSlotIndex);
			renderSlotIndex += skySphereComp->mesh.meshPrimitives.size();
		}

		Renderer::GetInstance().FreePrimitiveResourceOverflow();
	}

	// Want to keep sky sphere at cameras origin, but not include rotation so it appears we are looking around the world
	void SkySphereSystem::FollowCamera(TransformComponent* transformComp, const BMath::Vector3& cameraPosition)
	{
		transformComp->worldPosition = cameraPosition;
		transformComp->worldTransform._41 = cameraPosition.x;
		transformComp->worldTransform._42 = cameraPosition.y;
		transformComp->worldTransform._43 = cameraPosition.z;
	}

	// Since this primitive should only exist once in the scene it is fine to store the vertices directly inside the component
	// Otherwise the models vertices are stored in the resource manager and loaded for rendering
	void SkySphereSystem::InitializeSphere(SkySphereComponent* skySphereComp)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVertices(skySphereComp->mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(skySphereComp->mesh.meshData.modelFileName);
		skySphereComp->mesh.GenerateMesh(meshData, vertexData);
		skySphereComp->vertices.resize(vertexData.size());

		for (int i = 0; i < vertexData.size(); i++)
		{
			skySphereComp->vertices[i] = vertexData[i].position;
		}

		// Want all of the "sky sphere" primitives to be drawn first since they should always be considered the furthest possible
		for (auto& primitive : skySphereComp->mesh.meshPrimitives)
		{
			primitive.depth = std::numeric_limits<float>::max();
		}

		skySphereComp->isInitalized = true;
	}

	// Should be very similar to the mesh render class except their is no back face culling since we will be 
	void SkySphereSystem::ProcessSphere(
		TransformComponent* transformComp, 
		SkySphereComponent* skySphereComp,
		TransformComponent* cameraTransform,
		const BMath::Matrix4x4& viewProjMatrix, 
		int renderSlotIndex)
	{
		const MeshData& meshData = skySphereComp->mesh.meshData;

		ReserveResources(meshData.totalPrimitives);
		int primitiveIndex = 0;
		int numVertices = 3;

		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVertices = 4;
				int completeTriangles = meshData.triangleVertexCount / 3;
				int remainingVertices = vertexIndex - meshData.triangleVertexCount;
				int completeQuads = remainingVertices / 4;
				primitiveIndex = completeTriangles + completeQuads;
			}
			else
			{
				primitiveIndex = vertexIndex / 3;
			}

			Primitive& primitive = skySphereComp->mesh.meshPrimitives[i];

			// Only need to update the vertices if matrix dirty
			if (transformComp->isDirty)
			{
				for (int j = 0; j < numVertices; j++)
				{
					primitive.vertices[j] = transformComp->worldTransform * BMath::Vector4(skySphereComp->vertices[vertexIndex++], 1.0f);
				}
			}
			else
			{
				vertexIndex += numVertices;
			}

			// We can invert this function result since we are inside the sphere, so primitives pointing in the same direction as the 
			// camera (back face) should be renderer, and primitives pointing back at the camera should be culled (front faces)
			if (!CullBackFace(cameraTransform->worldPosition, cameraTransform->forwardVector, primitive.vertices))
			{
				continue;
			}

			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.vertices, sizeof(BMath::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVertices);

			if (!IsPrimitiveWithinFrustrum(numVertices, renderVerts))
			{
				continue;
			}

			AddPrimitiveToRenderer(primitive, numVertices, renderVerts, primitiveIndex);
			primitiveIndex++;
		}
	}
}