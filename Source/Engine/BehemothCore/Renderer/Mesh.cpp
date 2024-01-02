#include "pch.h"
#include "Mesh.h"
#include "Application/ResourceManager.h"

#include <iostream>
namespace Behemoth
{
	Mesh::Mesh(const std::string& modelPath, const std::string& texturePath, const Math::Vector2 uv) :
		meshData{},
		materialData{}
	{
		meshData.modelFileName = modelPath;
		materialData.textureFileName = texturePath;
		materialData.uvScale = uv;
		materialData.diffuse = Math::Vector3(0.8f, 0.8f, 0.8f);
		materialData.specular = Math::Vector3(1.0f, 1.0f, 1.0f);
		materialData.shininess = 32.0f;
	}

	void Mesh::GenerateMesh(const MeshData& meshData, const std::vector<VertexData>& vertexData)
	{
		meshPrimitives.clear();
		meshPrimitives.reserve(meshData.totalPrimitives);
		this->meshData = meshData;
		
		GeneratePrimitives(vertexData, TRIANGLE, meshData.triangleVertexCount, 0);
		GeneratePrimitives(vertexData, QUAD, meshData.quadVertexCount, meshData.triangleVertexCount);
	}

	void Mesh::GeneratePrimitives(const std::vector<VertexData>& vertexData, PrimitiveType type, int totalVerticies, int vertexOffset)
	{
		int numVerticies = (type == TRIANGLE) ? 3 : 4;

		for (int i = vertexOffset; i < totalVerticies + vertexOffset; i += numVerticies)
		{
			Math::Vector4 v[4];
			Math::Vector3 n[4];
			Math::Vector2 uv[4];

			for (int j = 0; j < numVerticies; j++)
			{
				v[j].x = vertexData[i + j].vertex.x;
				v[j].y = vertexData[i + j].vertex.y;
				v[j].z = vertexData[i + j].vertex.z;
				v[j].w = 1.0f;

				n[j] = vertexData[i + j].normal;

				uv[j].x = vertexData[i + j].uv.x * materialData.uvScale.x;
				uv[j].y = vertexData[i + j].uv.y * materialData.uvScale.y;
			}
			const std::string& path = Behemoth::ResourceManager::GetInstance().GetTexturePath(materialData.textureFileName);
			Primitives p{ path, materialData.textureFileName, type, v, n, uv };

			p.diffuse	= materialData.diffuse;
			p.specular	= materialData.specular;
			p.shininess = materialData.shininess;
			
			meshPrimitives.push_back(std::move(p));
		}
	}
}