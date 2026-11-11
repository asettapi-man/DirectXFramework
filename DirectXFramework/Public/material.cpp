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

	//初期化
	state = MaterialState::NONE;
}

/// <summary>
/// デストラクタ
/// </summary>
Material::~Material()
{
	
}

/// <summary>
/// 初期化関数
/// </summary>
void Material::Initialize()
{
	state = MaterialState::NONE;

	velocity = XMVectorZero();	//移動量ベクトルの初期化
	gravity = XMVectorSet(0.0f, gravityY, 0.0f, 0.0f);	//重力ベクトルの初期化
}

/// <summary>
/// 処理関数
/// </summary>
/// <param name="deltaTime">経過時間</param>
void Material::Execute(double deltaTime, const XMVECTOR holdPos)
{
	XMFLOAT3 materialPos;

	switch (state)
	{
	case MaterialState::NONE:	//無し

		//落下処理
		velocity += gravity * deltaTime;
		model->addPositionY(XMVectorGetY(velocity) * deltaTime);

		//地面判定
		if (model->getPositionY() < 0.0f)
		{
			model->setPositionY(0.0f);
			velocity = XMVectorZero();
		}

		materialPos = XMFLOAT3(
			model->getPositionX(),
			model->getPositionY() + 0.15f,	//BoundingBoxの位置調整で0.1f加えている
			model->getPositionZ()
		);

		collider.SetBoundingBoxCenter(materialPos);
		break;
	case MaterialState::HOLD:	//保持状態
		model->setPosition(&holdPos);

		materialPos = XMFLOAT3(
			model->getPositionX(),
			model->getPositionY() + 0.15f,	//BoundingBoxの位置調整で0.1f加えている
			model->getPositionZ()
		);

		collider.SetBoundingBoxCenter(materialPos);

		break;
	}
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
	case MaterialType::WOOD:	//木材
		model = new vnModel(L"data/model/Wood/WoodenSlab/", L"WoodenSlab.vnm");	//木材モデル
		break;

	case MaterialType::METAL:	//金属
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
	collider.SetBoundingBoxExtents(XMFLOAT3(1.25f, 0.15f, 1.25f));
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
