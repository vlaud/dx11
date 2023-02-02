#pragma once
#include <vector>
#include "../Vertex.h"
using namespace std;
class GeometryHelper
{
public:
	static void GeneratorBox(float w, float h, float d, vector<Vertex>* vertices, vector<unsigned int>* indices);
};

