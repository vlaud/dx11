#pragma once
#include "../GameObject.h"
#include "Renderer.h"
#include "../../../EngineFileSystem.h"
template<typename T>
class MeshRenderer : public Renderer
{
	vector<Mesh<T>> meshes;
	bool LoadFile(const string& filePath) override
	{
		directory = GetDirectoryFromPath(filePath);
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (scene == nullptr) return false;
		ProcessNode(scene->mRootNode, scene, XMMatrixIdentity());
		return true;
	}
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parent)
	{
		XMMATRIX local = XMMATRIX(&node->mTransformation.a1) * parent;
		for (UINT i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, local));
		}
		for (UINT i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, local);
		}
	}
	Mesh<T> ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& local)
	{
		vector<T> vertices;
		vector<DWORD> indices;
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex v;
			v.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			v.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			if (mesh->mTextureCoords[0])
			{
				v.uv.x = (float)mesh->mTextureCoords[0][i].x;
				v.uv.y = (float)mesh->mTextureCoords[0][i].y;
			}
			vertices.push_back(*reinterpret_cast<T*>(&v)); //강제 형변환 (주소 값 전달)
		}
		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT n = 0; n < face.mNumIndices; ++n)
			{
				indices.push_back(face.mIndices[n]);
			}
		}
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> textures = LoadMaterialTextures(mat, aiTextureType::aiTextureType_DIFFUSE, scene);

		return Mesh<T>(device, dc, vertices, indices, textures, local);
	}
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* material,
		UINT idx, aiTextureType type)
	{
		if (material->GetTextureCount(type) == 0) return TextureStorageType::None;

		aiString path;
		material->GetTexture(type, idx, &path);
		string texPath = path.C_Str();
		if (texPath[0] == '*') // 배열
		{
			if (scene->mTextures[0]->mHeight == 0) // 압축되어있음
			{
				return TextureStorageType::EmbeddedIndexCompressed;
			}
			else
			{
				assert("Support does not exist yet for indexed no compressed textures!" && 0);
				return TextureStorageType::EmbeddedIndexNonCompressed;
			}
		}
		if (auto tex = scene->GetEmbeddedTexture(texPath.c_str()))
		{
			if (tex->mHeight == 0)
			{
				return TextureStorageType::EmbeddedCompressed;
			}
			else
			{
				assert("Support does not exist yet for no compressed textures!" && 0);
				return TextureStorageType::EmbeddedNonCompressed;
			}
		}
		if (texPath.find('.') != string::npos)
		{
			return TextureStorageType::Disk;
		}
		return TextureStorageType::Invaild;
	}
	vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)

	{
		vector<Texture> textures;

		TextureStorageType texType = TextureStorageType::Invaild;
		UINT texCount = material->GetTextureCount(type);
		if (texCount == 0)
		{
			texType = TextureStorageType::None;
			aiColor3D aiColor = { 0,0,0 };
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				if (aiColor.IsBlack())
				{
					textures.push_back(Texture(device, COLORS::UnLoadedTextureColor, type));
					break;
				}
				textures.push_back(Texture(device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), type));
				break;
			}
		}
		else
		{
			for (UINT i = 0; i < texCount; ++i)
			{
				aiString path;
				material->GetTexture(type, i, &path);
				texType = DetermineTextureStorageType(scene, material, i, type);
				switch (texType)
				{
				case TextureStorageType::Disk:
				{
					string filename = directory + '/' + path.C_Str();
					textures.push_back(Texture(device, filename, type));
					break;
				}
				case TextureStorageType::EmbeddedCompressed:
				{
					const aiTexture* tex = scene->GetEmbeddedTexture(path.C_Str());
					Texture embeddedTex(device, (uint8_t*)tex->pcData, tex->mWidth, type);
					textures.push_back(embeddedTex);
					break;
				}
				case TextureStorageType::EmbeddedIndexCompressed:
				{
					int idx = GetTextureIndex(&path);
					Texture embeddedTex(device, (uint8_t*)scene->mTextures[idx]->pcData,
						scene->mTextures[idx]->mWidth, type);
					textures.push_back(embeddedTex);
					break;
				}
				}
			}
		}

		if (textures.size() == 0)
		{
			textures.push_back(Texture(device, COLORS::UnHandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
		}
		return textures;
	}
	int GetTextureIndex(aiString* aiStr)
	{
		assert(aiStr->length >= 2);
		return atoi(&aiStr->C_Str()[1]);
	}
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* dc,
		string path, ConstantBuffer<CB_VS_WVP>& cb_mat,
		vector<T>& vertices, vector<DWORD>& indices)
	{
		this->device = device;
		this->dc = dc;
		this->cb_mat = &cb_mat;

		vector<Texture> textures;
		textures.push_back(Texture(device, path, aiTextureType::aiTextureType_DIFFUSE));

		meshes.push_back(Mesh<T>(device, dc, vertices, indices, textures, XMMatrixIdentity()));
		return true;
	}
	void Update() override
	{

	}
	void Draw(const XMMATRIX vpMat) override
	{
		UINT offset = 0;
		dc->VSSetConstantBuffers(0, 1, cb_mat->GetAddressOf());

		for (Mesh<T> mesh : meshes)
		{
			cb_mat->data.world = mesh.GetLocal() * gameObject->GetTransform()->GetWorld();
			cb_mat->data.vp = vpMat;
			cb_mat->Update();
			mesh.Draw();
		}
	}
	void Draw(const XMMATRIX vpMat, UINT faceNum, vector<Vertex>& vertices, vector<DWORD>& indices,
		ID3D11RasterizerState* Default, ID3D11RasterizerState* Wire)
	{
		UINT offset = 0;
		dc->VSSetConstantBuffers(0, 1, cb_mat->GetAddressOf());

		for (Mesh<T> mesh : meshes)
		{
			cb_mat->data.world = mesh.GetLocal() * gameObject->GetTransform()->GetWorld();
			cb_mat->data.vp = vpMat;
			cb_mat->Update();
			mesh.DrawWireframeAt(faceNum, vertices, indices, Default, Wire);
		}
	}
};

