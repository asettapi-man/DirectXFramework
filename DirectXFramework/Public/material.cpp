#include "../framework.h"
#include "../Framework/environment.h"
#include <random>
#include "material.h"

Material::~Material()
{
	delete itemData.model;
}

float Material::GetRandomWeight()
{
	std::random_device rd;
	std::mt19937 mt(rd);
	std::uniform_real_distribution<float> dist(minMaterialWeight, maxMaterialWeight);
	float weight = dist(mt);

	return weight;
}

std::vector<vnModel*> Material::RandomDropMaterials()
{
	std::vector<vnModel*> result;	//Œ‹‰Ê—p

	std::random_device rd;
	std::mt19937 mt(rd);
	std::uniform_int_distribution<unsigned int> dist(minDropCount, maxDropCount);
	unsigned int rand = dist(mt);	//—”¶¬

	for (unsigned int i = 0; i < rand; i++)
	{

	}

	return result;
}
