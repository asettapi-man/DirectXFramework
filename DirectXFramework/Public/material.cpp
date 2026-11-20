//--------------------------------------------------------------//
//	"material.cpp"												//
//	資材クラス													//
//  フィールド上での挙動やアイテムの情報を担当					//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

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
	//初期化
	state = MaterialState::NONE;
	colliderExtents = XMFLOAT3(1.25f, 0.15f, 1.25f);

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
/// 初期化
/// </summary>
void Material::Initialize()
{
	state = MaterialState::NONE;

	velocity = XMVectorZero();	//移動量ベクトルの初期化
	gravity = XMVectorSet(0.0f, gravityY, 0.0f, 0.0f);	//重力ベクトルの初期化

	isHold = false;	
	isThrow = false;
}

/// <summary>
/// 継続処理
/// </summary>
/// <param name="deltaTime">1フレームの処理にかかった時間</param>
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
			model->getPositionY() + OFFSET,	//BoundingBoxの位置調整で0.15f加えている
			model->getPositionZ()
		);

		collider.SetBoundingBoxCenter(materialPos);
		break;
	case MaterialState::HOLD:	//保持状態
		model->setPosition(&holdPos);

		materialPos = XMFLOAT3(
			model->getPositionX(),
			model->getPositionY() + OFFSET,	//BoundingBoxの位置調整で0.1f加えている
			model->getPositionZ()
		);

		collider.SetBoundingBoxCenter(materialPos);
		break;

	case MaterialState::ROTATE:	//回転状態
		RotateMaterial(deltaTime);	//回転処理

		model->setPosition(&holdPos);

		materialPos = XMFLOAT3(
			model->getPositionX(),
			model->getPositionY() + OFFSET,	//BoundingBoxの位置調整で0.1f加えている
			model->getPositionZ()
		);

		collider.SetBoundingBoxCenter(materialPos);
		break;

	case MaterialState::CHARGE:
		ThrowCharge(deltaTime, holdPos);
		break;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Material::Render()
{
#ifdef _DEBUG
	collider.DebugLineRenderer();	//コライダーの枠組み表示
#endif
}

/// <summary>
/// 資材の重量取得
/// </summary>
/// <returns>決められたランダムな値を返す</returns>
float Material::GetRandomWeight()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(MINWEIGHT, MAXWEIGHT);
	float weight = dist(mt);

	return weight;
}

/// <summary>
/// モデル作成
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
	collider.SetBoundingBoxExtents(colliderExtents);
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

/// <summary>
/// 回転処理
/// </summary>
/// <param name="deltaTime">1フレームの処理にかかった時間</param>
void Material::RotateMaterial(double deltaTime)
{
	//マウスのx, yの移動量
	float mx = (float)vnMouse::getDX();
	float my = (float)vnMouse::getDY();

	theta = mx * deltaTime;
	phi   = my * deltaTime;

	model->addRotationX(phi);
	model->addRotationY(theta);
}

void Material::ThrowCharge(double deltaTime, XMVECTOR holdPos)
{
	elapsed += deltaTime;	//経過時間
	force += elapsed;		//投擲力を計算

	if (vnMouse::relL())//左クリックを離した時
	{
		ThrowMaterial(holdPos);
	}
}


void Material::ThrowMaterial(XMVECTOR holdPos)
{

	XMVECTOR addForce = holdPos * force;

	model->addPosition(&addForce);

	//初期化
	elapsed = 0.0f;
	force = 1.0f;
	state = MaterialState::THROW;
}
