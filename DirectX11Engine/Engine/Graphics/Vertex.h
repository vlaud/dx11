#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Vertex
{
public:
	XMFLOAT3 pos;
	XMFLOAT3 color;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	Vertex() : pos(0, 0, 0), color(1.0f, 1.0f, 1.0f), uv(0,0), normal(0,0,0) {}
	Vertex(XMFLOAT3 pos) : pos(pos), color(1.0f, 1.0f, 1.0f), uv(0, 0), normal(0, 0, 0) {}
	Vertex(XMFLOAT3 pos, XMFLOAT3 color) : pos(pos), color(color), uv(0, 0), normal(0, 0, 0) {}
	Vertex(XMFLOAT3 pos, XMFLOAT2 uv) : pos(pos), color(1,1,1), uv(uv), normal(0, 0, 0) {}
	Vertex(XMFLOAT3 pos, XMFLOAT2 uv, XMFLOAT3 normal) : pos(pos), color(1, 1, 1), uv(uv), normal(normal) {}
};

struct Vertex2D
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	Vertex2D() : pos(0, 0, 0), uv(0, 0) {}
	Vertex2D(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v) {}
	//Vertex2D(XMFLOAT3 pos) : pos(pos), uv(0, 0) {}
};

class VertexSkinned : public Vertex
{
public:
	XMFLOAT4 boneIDs = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT4 weights = XMFLOAT4(0, 0, 0, 0);
	VertexSkinned() : Vertex() {};
};

class VertexBump :  public Vertex
{
public:
	XMFLOAT3 tangent = { 0,0,0 };
	XMFLOAT3 binormal = { 0,0,0 };
	VertexBump() : Vertex() {};
};