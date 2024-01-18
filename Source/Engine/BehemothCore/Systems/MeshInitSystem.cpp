#include "pch.h"
#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "ECS/Registry.h"
#include "Geometry/Mesh.h"
#include "Core/ResourceManager.h"
#include "Core/Log.h"
#include "Misc/TransformHelper.h"



namespace Behemoth
{
	void MeshInitSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		// Will need to get the sparse set container no matter what since the .Get<>() returns a tuple and iterating backwards is difficult
		// This way we can do a very quick and easy check to see if there are any meshes that need to be initialized
		// most likely this check will fail and exit early so want to do this before calling .Get on the registry
		auto components = registry.Get<MeshInitalizeComponent>();

		for (const auto& [entity, meshInitComp] : components)
		{
			MeshComponent* meshComponent = registry.GetComponent<MeshComponent>(entity);
			if (meshComponent)
			{
				InitMesh(meshComponent->mesh);


				if (meshInitComp->initBroadCollider)
				{
					InitAABBBoundingVolume(registry, meshComponent, entity);
				}

				if (meshInitComp->initBoundingVolume)
				{
					InitSphereBoundingVolume(registry, meshComponent, entity);
				}
			}

#ifdef DEBUG

			WireframeComponent* wireframeMesh = registry.GetComponent<WireframeComponent>(entity);
			if (wireframeMesh)
			{
				InitMesh(wireframeMesh->mesh);
			}
#endif

			registry.RemoveComponent<MeshInitalizeComponent>(entity);
		}

	}

	void MeshInitSystem::InitMesh(Mesh& mesh)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVerticies(mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(mesh.meshData.modelFileName);
		mesh.GenerateMesh(meshData, vertexData);
	}

	void MeshInitSystem::GenerateAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BVHColliderComponent>(handle);
		BVHColliderComponent* broadCollider = registry.GetComponent<BVHColliderComponent>(handle);
		if (broadCollider)
		{
			BMath::Vector3 scale = BMath::Vector3::One();

			if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(handle))
			{
				scale = transformComp->worldScale;
			}
			else
			{
				LOGMESSAGE(Error, "Error getting transform from entity: " + registry.GetName(handle));
			}
			BMath::Vector3 temp = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName).worldExtents;
			
			broadCollider->extents = temp;
			broadCollider->collider.worldExtents = temp;
		}
	}
	void MeshInitSystem::InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BoundingVolumeComponent>(handle, true);
		BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(handle);
		if (boundingVolume)
		{
			// InitMesh(boundingVolume->mesh, std::ref(Path));
			SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComp->modelFileName);
			boundingVolume->radius = collider.radius;
			boundingVolume->localPosition = collider.position;
		}
	}

	void MeshInitSystem::InitAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		BVHColliderComponent* broadCollider = registry.AddComponent<BVHColliderComponent>(handle);
		if (broadCollider)
		{
			BMath::Vector3 scale = BMath::Vector3::One();
			AABBCollider baseCollider = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName);
			AABBCollider rotatedCollider{};

			if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(handle))
			{
				scale = transformComp->worldScale;
				BMath::Matrix3x3f rot = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform);
				GetRotatedAABB(baseCollider, rot,  rotatedCollider);
			}
			else
			{
				LOGMESSAGE(Error, "Error getting transform from entity: " + registry.GetName(handle));
			}

			broadCollider->extents = rotatedCollider.worldExtents;
			broadCollider->collider.worldExtents = broadCollider->extents;

			// registry.AddComponent<Behemoth::WireframeComponent>(handle, "cube.obj", broadCollider->extents, false);
		}
	}

	void MeshInitSystem::GetRotatedAABB(const AABBCollider& a, const BMath::Matrix3x3<float>& rotation,  AABBCollider& result)
	{
		for (int i = 0; i < 3; i++)
		{
			result.worldExtents[i] = 0.0f;
			for (int j = 0; j < 3; j++)
			{
				result.worldExtents[i] += std::abs(rotation.data[i][j]) * a.worldExtents[i];
			}
		}
	}
}