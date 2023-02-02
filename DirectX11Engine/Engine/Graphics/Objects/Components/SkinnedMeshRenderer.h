#pragma once
#include "Renderer.h"
#include "../../interpolate.h"

class SkinnedMeshRenderer : public Renderer
{
	//for Animation
	unordered_map<string, UINT> boneMap; //ª¿¥Î¡§∫∏
	vector<BoneNode> bones;
	vector<BoneNode> nodes;
	vector<AnimationClip> animClips;
	ConstantBuffer<CB_VS_BoneData> boneData;
	AnimationState animStateInfo;
	//for Animation Blending
	bool isChangeAnimation = false;
	float blendTime = 2.0f;
	float curBlendTime = 0.0f;
	AnimationState changeAnimStateInfo;

	void LoadAnimation(const aiScene* scene, vector<AnimationClipData>& animDatas);
	void MakeAnimationClip(vector<AnimationClipData>& animDatas);
	void ProcessBone(aiMesh* mesh, vector<SkinnedVertexWeights>& weights);
	void ProcessKeyFrameAnimation();

	vector<Mesh<VertexSkinned>> meshes;
	bool LoadFile(const string& filePath) override;
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parent,
		UINT idx, UINT parentIdx);
	Mesh<VertexSkinned> ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& local);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* material,
		UINT idx, aiTextureType type);
	vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
	int GetTextureIndex(aiString* aiStr);
public:
	void Update() override;
	void Draw(const XMMATRIX vpMat) override;
	void AddAnimation(const string& filePath);
	void ChangeAnimation(int idx, float t = 0.25f);
};

