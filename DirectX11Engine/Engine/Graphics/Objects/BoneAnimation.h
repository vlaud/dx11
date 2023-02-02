#pragma once
#include <memory>
#include <unordered_map>
#include "Mesh.h"

struct BoneNode
{
	string name;
	int idx = 0;
	int parentIdx = 0;
	XMMATRIX offset = XMMatrixIdentity();
};
struct KeyPosition
{
	float time;
	XMFLOAT3 pos;
};
struct KeyRotation
{
	float time;
	XMFLOAT4 rot;
};
struct KeyScale
{
	float time;
	XMFLOAT3 scale;
};
struct AnimationNodeData
{
	string name;
	vector<KeyPosition> position;
	vector<KeyRotation> rotation;
	vector<KeyScale> scales;
};
struct AnimationClipData
{
	string name;
	float duration;
	float tickPerSec;
	unordered_map<string, AnimationNodeData> keyFrames;
};
struct AnimationTransform
{
	XMFLOAT3 pos;
	XMFLOAT4 rot;
	XMFLOAT3 scale;
};
struct AnimationClip
{
	string name;
	float duration;
	float tickPerSec;
	unique_ptr<unique_ptr<AnimationTransform[]>[]> keyFrames; // [frame][node]
};
enum class ANIMSTATE
{
	LOOP, ONCE, STOP
};
struct AnimationState
{
	bool isPlaying = false;
	int idx = 0;
	int currentFrame = 1;
	int nextFrame = 2;
	float weight = 0.0f;
	ANIMSTATE state = ANIMSTATE::STOP;
};
#define MAX_BONE_LINK 4

struct SkinnedVertexWeights
{
	UINT boneIDs[MAX_BONE_LINK] = { 0 };
	float weights[MAX_BONE_LINK] = { 0.0f };
	SkinnedVertexWeights() {}
	SkinnedVertexWeights(const SkinnedVertexWeights& src)
	{
		//얕은 복사 = 메모리 누수 (참조자 소실) : 참조 대상만 바꿈
		//boneIDs = src.boneIDs;
		//깊은 복사 = 데이터를 통째로 복사
		memcpy(boneIDs, src.boneIDs, sizeof(boneIDs));
		memcpy(weights, src.weights, sizeof(weights));
	}
	void Add(const UINT& boneID, const float& weight)
	{
		for (UINT i = 0; i < MAX_BONE_LINK; ++i)
		{
			if (weights[i] == 0.0f)
			{
				boneIDs[i] = boneID;
				weights[i] = weight;
				break;
			}
		}
	}
	void Normalize()
	{
		float total = 0.0f;
		for (float weight : weights)
		{
			total += weight;
		}
		if (total == 0.0f) return;
		for (UINT i = 0; i < MAX_BONE_LINK; ++i)
		{
			if (weights[i] != 0.0f)
			{
				weights[i] /= total;
			}
		}
	}
};