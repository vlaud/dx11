#pragma once
#include <vector>
#include "../Vertex.h"
using namespace std;
class GeometryHelper
{
public:
	template<typename T>
	static void GeneratorPyramid(float w, float h, float d, vector<T>* vertices, vector<unsigned long>* indices)
	{
		vertices->clear();
		vertices->resize(16);
		Vertex v[] =
		{
			Vertex(0.0f * w, 1.0f * h, 0.0f * d, 0.5f, 0.5f),
			Vertex(-0.5f * w, 0.0f * h, 0.5f * d, 0.0f, 1.0f),
			Vertex(0.5f * w, 0.0f * h, 0.5f * d, 1.0f, 1.0f),

			Vertex(0.0f * w, 1.0f * h, 0.0f * d, 0.5f, 0.5f),
			Vertex(0.5f * w, 0.0f * h, 0.5f * d, 1.0f,1.0f),
			Vertex(0.5f * w, 0.0f * h, -0.5f * d, 1.0f, 0.0f),

			Vertex(0.0f * w, 1.0f * h, 0.0f * d, 0.5f, 0.5f),
			Vertex(0.5f * w, 0.0f * h, -0.5f * d, 1.0f, 0.0f),
			Vertex(-0.5f * w, 0.0f * h, -0.5f * d, 0.0f, 0.0f),

			Vertex(0.0f * w, 1.0f * h, 0.0f * d, 0.5f, 0.5f),
			Vertex(-0.5f * w, 0.0f * h, -0.5f * d, 0.0f, 0.0f),
			Vertex(-0.5f * w, 0.0f * h, 0.5f * d, 0.0f, 1.0f),

			Vertex(-0.5f * w, 0.0f * h, 0.5f * d, 0.0f, 0.0f),
			Vertex(0.5f * w, 0.0f * h, 0.5f * d, 1.0f, 0.0f),
			Vertex(-0.5f * w, 0.0f * h, -0.5f * d, 0.0f, 1.0f),
			Vertex(0.5f * w, 0.0f * h, -0.5f * d, 1.0f, 1.0f),
		};
		for (int i = 0; i < vertices->size(); ++i)
		{
			memcpy(&(*vertices)[i], &v[i], sizeof(Vertex));
		}

		indices->clear();
		indices->resize(18);
		
		unsigned long ind[18];
		for (int i = 0; i < 12; ++i)
		{
			ind[i] = i;
		}
		ind[12] = 12;
		ind[13] = 14;
		ind[14] = 13;
		ind[15] = 13;
		ind[16] = 14;
		ind[17] = 15;
		for (int i = 0; i < indices->size(); ++i)
		{
			(*indices)[i] = ind[i];
		}
	}
	template<typename T>
	static void GeneratorBox(float w, float h, float d, vector<T>* vertices, vector<unsigned long>* indices)
	{
		vertices->clear();
		vertices->resize(24);
		Vertex v[] =
		{
			//Front
			Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 0.0f}, {0,0, -1}),//0
			Vertex({0.5f * w, 0.5f * h, -0.5f * d},  {1.0f, 0.0f}, {0,0, -1}), //1
			Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 1.0f}, {0,0, -1}), //2
			Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 1.0f}, {0,0, -1}), //3
			//Back
			Vertex({-0.5f * w, 0.5f * h, 0.5f * d}, {1.0f, 0.0f}, {0,0, 1}), //4
			Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {0.0f, 0.0f}, {0,0, 1}), //5
			Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {1.0f, 1.0f}, {0,0, 1}), //6
			Vertex({0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}, {0,0, 1}), //7
			//Up
			Vertex({-0.5f * w, 0.5f * h, 0.5f * d}, {0.0f, 0.0f}, {0,1, 0}), //8
			Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {1.0f, 0.0f}, {0,1, 0}), //9
			Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 1.0f}, {0,1, 0}), //10
			Vertex({0.5f * w, 0.5f * h, -0.5f * d}, {1.0f, 1.0f}, {0,1, 0}), //11
			//Down
			Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}, {0,-1, 0}), //12
			Vertex({0.5f * w, -0.5f * h, 0.5f * d},  {1.0f, 1.0f}, {0,-1, 0}), //13
			Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 0.0f}, {0,-1, 0}), //14
			Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 0.0f}, {0,-1, 0}), //15
			//Right
			Vertex({0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 0.0f}, {1, 0, 0}), //16
			Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {1.0f, 0.0f}, {1, 0, 0}), //17
			Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 1.0f}, {1, 0, 0}), //18
			Vertex({0.5f * w, -0.5f * h, 0.5f * d}, {1.0f, 1.0f}, {1, 0, 0}), //19
			//Left
			Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {1.0f, 0.0f}, {-1, 0, 0}), //20
			Vertex({-0.5f * w, 0.5f * h, 0.5f * d},  {0.0f, 0.0f}, {-1, 0, 0}), //21
			Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 1.0f}, {-1, 0, 0}), //22
			Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}, {-1, 0, 0}), //23
		};
		for (int i = 0; i < vertices->size(); ++i)
		{
			memcpy(&(*vertices)[i], &v[i], sizeof(Vertex));
		}

		indices->clear();
		indices->resize(36);
		unsigned long ind[] =
		{
			//Front
			0,1,3,
			0,3,2,
			//Back
			4,7,5,
			4,6,7,
			//Up
			8,9,10,
			9,11,10,
			//Down
			12,14,13,
			14,15,13,
			//Right
			16,17,18,
			17,19,18,
			//Left
			20,22,21,
			22,23,21
		};
		for (int i = 0; i < indices->size(); ++i)
		{
			(*indices)[i] = ind[i];
		}
	}
	template<typename T>
	static void GeneratorSphere(float radius, unsigned long sliceCount, unsigned long stackCount,
		vector<T>* vertices, vector<unsigned long>* indices)
	{
		vertices->clear();

		Vertex top({ 0, radius, 0 }, { 0,0 }, { 0, 1, 0 });
		vertices->push_back(*reinterpret_cast<T*>(&top));

		float piStep = XM_PI / (float)stackCount;
		float thetaStep = XM_2PI / (float)sliceCount; // ¿ø¹Ý °ãÄ¡±â

		for (unsigned long n = 1; n <= stackCount - 1; ++n)
		{
			float pi = (float)n * piStep;
			for (unsigned long m = 0; m <= sliceCount; ++m)
			{
				float theta = (float)m * thetaStep;

				Vertex v;
				v.pos.x = radius * sinf(pi)* cosf(theta);
				v.pos.y = radius * cosf(pi);
				v.pos.z = radius * sinf(pi) * sinf(theta);

				XMVECTOR vec = XMLoadFloat3(&v.pos);
				XMStoreFloat3(&v.normal, XMVector3Normalize(vec));

				v.uv.x = theta / XM_2PI;
				v.uv.y = pi / XM_PI;

				vertices->push_back(*reinterpret_cast<T*>(&v));
			}
		}

		Vertex bot({ 0, -radius, 0 }, { 0,1 }, { 0, -1, 0 });
		vertices->push_back(*reinterpret_cast<T*>(&bot));

		indices->clear();

		for (unsigned long i = 0; i < sliceCount; ++i)
		{
			indices->push_back(0);
			indices->push_back(i + 1);
			indices->push_back(i);
		}
		unsigned long baseIndex = 1;
		unsigned long ringVertexCount = sliceCount + 1;
		for (unsigned long n = 0; n < stackCount - 2; ++n)
		{
			for (unsigned long m = 0; m < sliceCount; ++m)
			{
				indices->push_back(baseIndex + n * ringVertexCount + m);
				indices->push_back(baseIndex + n * ringVertexCount + m + 1);
				indices->push_back(baseIndex + (n + 1) * ringVertexCount + m);

				indices->push_back(baseIndex + (n + 1) * ringVertexCount + m);
				indices->push_back(baseIndex + n * ringVertexCount + m + 1);
				indices->push_back(baseIndex + (n + 1) * ringVertexCount + m + 1);
			}
		}

		unsigned long soutPoleIndex = (unsigned long)vertices->size() - 1;
		baseIndex = soutPoleIndex - ringVertexCount;

		for (unsigned long i = 0; i < sliceCount; ++i)
		{
			indices->push_back(soutPoleIndex);
			indices->push_back(baseIndex + i);
			indices->push_back(baseIndex + i + 1);
		}
	}
};

