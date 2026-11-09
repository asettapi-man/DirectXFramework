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
	CreateModel(type);	//資材モデル生成
	itemData = CreateItemData(type);	//採取時にランダムな資材を生成
}

/// <summary>
/// デストラクタ
/// </summary>
Material::~Material()
{
	
}

/// <summary>
/// 描画関数
/// </summary>
void Material::Render()
{
#ifdef _DEBUG
	collider.DebugLineRenderer();	//コライダーの枠組み表示
#endif
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
void Material::CreateModel(MaterialType type)
{
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

	//AABBコライダーの設定
	XMFLOAT3 materialPos = XMFLOAT3(
		model->getPositionX(),
		model->getPositionY(),
		model->getPositionZ()
	);

	collider.SetBoundingBoxCenter(materialPos);
	collider.SetBoundingBoxExtents(XMFLOAT3(1.25f, 0.3f, 1.25f));
}

/// <summary>
/// 資材情報の作成
/// </summary>
/// <param name="type">資材の種類</param>
/// <returns>資材情報を返す</returns>
ItemData Material::CreateItemData(MaterialType type)
{
	float weight = GetRandomWeight();

	itemData = {type , weight };

	return itemData;
}
