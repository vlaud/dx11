#pragma once
#include "Objects/BoneAnimation.h"

typedef unordered_map<string, AnimationNodeData>::iterator KeyFramesIter;

static int FindPos(KeyFramesIter iter, float time)
{
	auto& list = iter->second.position;
	if (list.empty()) return -1;
	for (UINT i = 0; i < list.size() - 1; ++i)
	{
		if (time < list[i + 1].time) return i;
	}
	return -1;
}
static int FindRot(KeyFramesIter iter, float time)
{
	auto& list = iter->second.rotation;
	if (list.empty()) return -1;
	for (UINT i = 0; i < list.size() - 1; ++i)
	{
		if (time < list[i + 1].time) return i;
	}
	return -1;
}
static int FindScale(KeyFramesIter iter, float time)
{
	auto& list = iter->second.scales;
	if (list.empty()) return -1;
	for (UINT i = 0; i < list.size() - 1; ++i)
	{
		if (time < list[i + 1].time) return i;
	}
	return -1;
}
static float Clamp(float v, float min, float max)
{
	return v > max ? max : v < min ? min : v;
}
static XMFLOAT3* Vec3Linear(XMFLOAT3* pOut, const XMFLOAT3* a, XMFLOAT3* b, float t)
{
	if (pOut == nullptr || a == nullptr || b == nullptr) return nullptr;
	t = Clamp(t, 0.0f, 1.0f);

	XMVECTOR vOut, vStart, vEnd;
	vStart = XMLoadFloat3(a);
	vEnd = XMLoadFloat3(b);
	vOut = vStart + (vEnd - vStart) * t;
	XMStoreFloat3(pOut, vOut);
	return pOut;
}
static XMFLOAT3 CalcInterpolatedPosition(KeyFramesIter iter, float t, int dur)
{
	auto& frame = iter->second.position;
	if (frame.empty()) return { 0,0,0 };
	if (frame.size() == 1) return frame.front().pos;
	if (t == dur - 1) return frame.back().pos;
	int idx = FindPos(iter, t);
	if (idx == -1) return frame.back().pos;

	UINT nextIdx = idx + 1;
	assert(nextIdx < frame.size());

	float deltaTime = frame[nextIdx].time - frame[idx].time;
	float factor = (t - frame[idx].time) / deltaTime;

	if (factor < 0.0f) return frame.front().pos;

	auto start = frame[idx].pos;
	auto end = frame[nextIdx].pos;

	Vec3Linear(&start, &start, &end, factor);
	return start;
}
static XMFLOAT4 CalcInterpolatedRotation(KeyFramesIter iter, float t, int dur)
{
	auto& frame = iter->second.rotation;
	if (frame.empty()) return { 0,0,0,1};
	if (frame.size() == 1) return frame.front().rot;
	if (t == dur - 1) return frame.back().rot;
	int idx = FindPos(iter, t);
	if (idx == -1) return frame.back().rot;

	UINT nextIdx = idx + 1;
	assert(nextIdx < frame.size());

	float deltaTime = frame[nextIdx].time - frame[idx].time;
	float factor = (t - frame[idx].time) / deltaTime;

	if (factor < 0.0f) return frame.front().rot;

	XMFLOAT4 start = frame[idx].rot;
	XMFLOAT4 end = frame[nextIdx].rot;

	XMVECTOR vStart, vEnd;
	vStart = XMLoadFloat4(&start);
	vEnd = XMLoadFloat4(&end);
	vStart = XMQuaternionSlerp(vStart, vEnd, factor);
	XMStoreFloat4(&start, vStart);
	return start;
}
static XMFLOAT3 CalcInterpolatedScale(KeyFramesIter iter, float t, int dur)
{
	auto& frame = iter->second.scales;
	if (frame.empty()) return { 0,0,0 };
	if (frame.size() == 1) return frame.front().scale;
	if (t == dur - 1) return frame.back().scale;
	int idx = FindPos(iter, t);
	if (idx == -1) return frame.back().scale;

	UINT nextIdx = idx + 1;
	assert(nextIdx < frame.size());

	float deltaTime = frame[nextIdx].time - frame[idx].time;
	float factor = (t - frame[idx].time) / deltaTime;

	if (factor < 0.0f) return frame.front().scale;

	auto start = frame[idx].scale;
	auto end = frame[nextIdx].scale;

	Vec3Linear(&start, &start, &end, factor);
	return start;
}