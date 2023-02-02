#include "GeometryHelper.h"

void GeometryHelper::GeneratorBox(float w, float h, float d, vector<Vertex>* vertices, vector<unsigned int>* indices)
{
	vertices->clear();
	vertices->resize(24);
	Vertex v[] =
	{
		//Front
		Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 0.0f}), //0
		Vertex({0.5f * w, 0.5f * h, -0.5f * d},  {1.0f, 0.0f}), //1
		Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 1.0f}), //2
		Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 1.0f}), //3
		//Back
		Vertex({-0.5f * w, 0.5f * h, 0.5f * d}, {1.0f, 0.0f}), //4
		Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {0.0f, 0.0f}), //5
		Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {1.0f, 1.0f}), //6
		Vertex({0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}), //7
		//Up
		Vertex({-0.5f * w, 0.5f * h, 0.5f * d}, {0.0f, 0.0f}), //8
		Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {1.0f, 0.0f}), //9
		Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 1.0f}), //10
		Vertex({0.5f * w, 0.5f * h, -0.5f * d}, {1.0f, 1.0f}), //11
		//Down
		Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}), //12
		Vertex({0.5f * w, -0.5f * h, 0.5f * d},  {1.0f, 1.0f}), //13
		Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 0.0f}), //14
		Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 0.0f}), //15
		//Right
		Vertex({0.5f * w, 0.5f * h, -0.5f * d}, {0.0f, 0.0f}), //16
		Vertex({0.5f * w, 0.5f * h, 0.5f * d},  {1.0f, 0.0f}), //17
		Vertex({0.5f * w, -0.5f * h, -0.5f * d}, {0.0f, 1.0f}), //18
		Vertex({0.5f * w, -0.5f * h, 0.5f * d}, {1.0f, 1.0f}), //19
		//Left
		Vertex({-0.5f * w, 0.5f * h, -0.5f * d}, {1.0f, 0.0f}), //20
		Vertex({-0.5f * w, 0.5f * h, 0.5f * d},  {0.0f, 0.0f}), //21
		Vertex({-0.5f * w, -0.5f * h, -0.5f * d}, {1.0f, 1.0f}), //22
		Vertex({-0.5f * w, -0.5f * h, 0.5f * d}, {0.0f, 1.0f}), //23
	};
	for (int i = 0; i < vertices->size(); ++i)
	{
		(*vertices)[i] = v[i];
	}

	indices->clear();
	indices->resize(36);
	unsigned int ind[] =
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
