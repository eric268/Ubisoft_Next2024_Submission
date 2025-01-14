#pragma once

#include "Geometry/Mesh.h"
#include "Math/MathCore.h"
#include "Physics/Collision/Colliders.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace Behemoth
{
	// TODO:
	// Split manager into separate managers for textures, mesh, sound etc.
	class ResourceManager
	{
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		static ResourceManager& GetInstance();

		const MeshData& GetMeshData(const std::string& filePath);
		const std::vector<VertexData>& GetMeshVertices(const std::string& filePath);

		const std::string& GetTexturePath(const std::string& textureName);
		const std::string& GetSoundClipPath(const std::string& soundclipName);
		const std::string& GetImagePath(const std::string& soundclipName);
		AABBCollider GetMeshAABBBounds(const std::string& filenpath);
		SphereCollider GetMeshSphereBounds(const std::string& filepath);

	private:
		ResourceManager() = default;
		const std::pair <MeshData, std::vector<VertexData>>& GetMesh(const std::string& filepath);
		bool LoadMeshData(const std::string& filePath, const std::size_t id);

		std::hash<std::string> hasher;
		std::unordered_map<std::string, std::string> imageMap;
		std::unordered_map<std::string, std::string> textureMap;
		std::unordered_map<std::string, std::string> soundClipMap;
		std::unordered_map<std::size_t, std::pair<MeshData, std::vector<VertexData>>> meshMap;
		std::unordered_map<std::size_t, AABBCollider> meshAABBBounds;
		std::unordered_map<std::size_t, SphereCollider> meshSphereBounds;
	};

}
