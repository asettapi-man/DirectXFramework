#include "../framework.h"
#include "../Framework/environment.h"
#include <random>
#include "material.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">資材の種類</param>
Material::Material(MaterialType type)
{
	itemData = CreateItemData(type);	//採取時にランダムな資材を生成
}

/// <summary>
/// デストラクタ
/// </summary>
Material::~Material()
{

}

/// <summary>
/// 資材の重量取得関数
/// </summary>
/// <returns>決められたランダムな値を返す</returns>
float Material::GetRandomWeight()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(minMaterialWeight, maxMaterialWeight);
	float weight = dist(mt);

	return weight;
}

/// <summary>
/// 資材モデル作成関数
/// </summary>
/// <param name="type">資材の種類</param>
/// <returns>種類ごとの資材モデルを返す/returns>
vnModel* Material::CreateModel(MaterialType type)
{
	vnModel* model = nullptr;	//出力結果

	switch (type)
	{
	case MaterialType::Wood:	//木材
		model = new vnModel(L"data/model/Wood/WoodenSlab/", L"WoodenSlab.vnm");	//木材モデル
		break;

	case MaterialType::Metal:	//金属
		model = new vnModel(L"data/model/", L"crown.vnm");	//金属モデル
		break;

	default:	//該当なし
		break;
	}

	return model;
}

/// <summary>
/// 資材情報の作成
/// </summary>
/// <param name="type">資材の種類</param>
/// <returns>資材情報を返す</returns>
ItemData Material::CreateItemData(MaterialType type)
{
	float weight = GetRandomWeight();
	vnModel* model = CreateModel(type);

	itemData = { model, weight };

	return itemData;
}
