#include "../../framework.h"
#include "../../Framework/environment.h"
#include "player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="folder">モデルがあるフォルダ名</param>
/// <param name="file">モデルのファイル名</param>
/// <param name="boxColliderCenter">モデルの当たり判定位置</param>
/// <param name="boxColliderExtents">モデルの当たり判定の大きさ</param>
Player::Player(const WCHAR* folder, const WCHAR* file, const XMFLOAT3& boxColliderCenter, const XMFLOAT3& boxColliderExtents)
{
	model = new vnModel(folder, file);
	aabbCollider.Initialize(boxColliderCenter, boxColliderExtents);	//AABBの初期化

	velocity = XMVectorZero();	//移動量ベクトルの初期化
	gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);	//重力ベクトルの初期化

	jumpForce = 0.3f;	//ジャンプ力

	radius = 10.0f;	//半径
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	//メモリ解放
	delete(model);
}

/// <summary>
/// 初期化関数
/// </summary>
void Player::Initialize()
{
	direction = eDirection::NONE;	//衝突方向

	isJump = false;	//ジャンプフラグ

	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	reverseH = false;	//水平反転
	reverseV = false;	//垂直反転
}

/// <summary>
/// 継続処理関数
/// </summary>
void Player::Execute()
{
	bool isMovingForPlayer = false;	//移動フラグ

	//移動方向を示す方向ベクトル
	vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	if (vnKeyboard::on(DIK_W))
	{
		vMove = XMVectorSetZ(vMove, 0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_S))
	{
		vMove = XMVectorSetZ(vMove, -0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_D))
	{
		vMove = XMVectorSetX(vMove, 0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_A))
	{
		vMove = XMVectorSetX(vMove, -0.1f);
		isMovingForPlayer = true;	//移動中
	}

	//ジャンプ処理
	if (vnKeyboard::trg(DIK_SPACE) && !isJump)
	{
		//pJumpSE->play();
		isJump = true;	//ジャンプ中はジャンプできない
		velocity = XMVectorSet(0.0f, jumpForce, 0.0f, 0.0f);
	}

	//落下処理
	velocity += gravity;
	model->addPositionY(XMVectorGetY(velocity));

	if (XMVectorGetY(velocity) < 0.0f)
	{
		//落下中はジャンプ不可
		isJump = true;
	}

	//プレイヤーの移動ベクトル（vMove）をカメラの角度（theta）に応じて回転
	XMMATRIX mrotate = XMMatrixRotationY(theta);

	//移動ベクトルに回転マトリクスを適用
	vMove = XMVector3TransformNormal(vMove, mrotate);

	//プレイヤーの座標に移動ベクトルを加算して更新
	model->addPosition(&vMove);

	//プレイヤーの角度を進行方向に向ける
	if (isMovingForPlayer)
	{
		float movePX = XMVectorGetX(vMove);	//進行方向ベクトルのX成分
		float movePZ = XMVectorGetZ(vMove);	//進行方向ベクトルのZ成分
		float rotPY = atan2f(movePX, movePZ);	//進行方向ベクトルのXとZからY軸の回転角度を計算

		//現在の角度(source) ; pPlayer->getRotation()
		//目的の角度(destination) ; rotPY
		XMMATRIX mSrc, mDst, mAns;
		XMVECTOR qSrc, qDst, qAns;

		//オイラー角からクオータニオンに変換（Y軸回転のみ）
		//qSrc = XMQuaternionRotationRollPitchYaw(0.0f, pPlayer->getPositionY(), 0.0f);
		//qDst = XMQuaternionRotationRollPitchYaw(0.0f, rotPY, 0.0f);

		//オイラー角をマトリクスに変換（Y軸回転のみ）
		mSrc = XMMatrixRotationY(model->getRotationY());
		mDst = XMMatrixRotationY(rotPY);

		//マトリクスをクオータニオンに変換
		qSrc = XMQuaternionRotationMatrix(mSrc);
		qDst = XMQuaternionRotationMatrix(mDst);

		//球面線形補間での中間の姿勢（角度）を計算
		qAns = XMQuaternionSlerp(qSrc, qDst, 0.1f);	//第三引数の数値が大きいほど速く回転する

		//クオータニオンをマトリクスに変換
		mAns = XMMatrixRotationQuaternion(qAns);

		//マトリクスかオイラー角(Y軸回転値)を計算
		float ry = atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		//計算された中間角度を設定
		model->setRotationY(ry);	//プレイヤーのY軸の回転角度を更新
	}

	//AABBの中心座標を設定
	playerPos = XMFLOAT3(
		model->getPositionX(),
		model->getPositionY() + aabbCollider.GetBoundingBox().Extents.y,
		model->getPositionZ());	//プレイヤーの座標を取得

	aabbCollider.SetBoundingBoxCenter(playerPos);	//AABBの中心座標を設定

	//地面判定
	if (model->getPositionY() < 0.0f)
	{
		model->setPositionY(0.0f);
		velocity = XMVectorZero();
		isJump = false;	//地面に着地したらジャンプ可能
	}

	//マウスの移動量を取得
	float dx = vnMouse::getDX();
	float dy = vnMouse::getDY();

	const float value_r = 0.5f * 3.141592f / 180.0f;	//ラジアン角度

	//角度の更新
	if (reverseH)
	{
		dx = -dx;	//水平反転
	}
	if (reverseV)
	{
		dy = -dy;	//垂直反転
	}

	theta += dx * value_r;
	phi += dy * value_r;

	//角度の制限
	if (phi > 1.5f)
	{
		phi = 1.5f;
	}
	else if (phi < -1.5f)
	{
		phi = -1.5f;
	}

	//== １人称カメラ座標の計算 ==
	XMVECTOR camBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);	//カメラの位置のプレイヤーに対するオフセット
	XMVECTOR camTarget;	//カメラの注視点（プレイヤーの座標 + オフセット）
	XMVECTOR camPos;	//カメラの位置

	//回転していない基準となるベクトル
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -0.01f, 0.0f);

	//回転マトリクスを作成
	XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//基準となるベクトルを回転させる
	eye = XMVector3TransformNormal(eye, camRotate);

	//回転の中心をプレイヤーにする（プレイヤーの座標を加算する）
	eye = XMVectorAdd(eye, *model->getPosition());

	camPos = XMVectorAdd(eye, camBias);

	//カメラの位置を更新
	vnCamera::setPosition(&camPos);

	//カメラの注視点を更新
	camTarget = XMVectorAdd(*model->getPosition(), camBias);

	vnCamera::setTarget(&camTarget);

#ifdef _DEBUG
	if (vnKeyboard::on(DIK_UP))
	{
		vMove = XMVectorSetY(vMove, 10.0f);
	}

#endif // DEBUG

}

/// <summary>
/// 描画関数
/// </summary>
void Player::Render()
{
#ifdef _DEBUG
	if (aabbCollider.GetEnable())
	{
		aabbCollider.DebugLineRenderer();
	}
#endif // _DEBUG
}


bool Player::CheckPlayerCollision(AABBCollider& c)
{
	//プレイヤーのAABBCollider取得
	AABBCollider& pc = aabbCollider;

	//BoundingBoxの取得
	BoundingBox& m1BoundingBox = pc.GetBoundingBox();
	BoundingBox& m2BoundingBox = c.GetBoundingBox();

	if (pc.Intersects(c))
	{
		//x, y, zの重なりの大きさを計算
		float overlapX = m1BoundingBox.Extents.x + m2BoundingBox.Extents.x - abs(m1BoundingBox.Center.x - m2BoundingBox.Center.x);
		float overlapY = m1BoundingBox.Extents.y + m2BoundingBox.Extents.y - abs(m1BoundingBox.Center.y - m2BoundingBox.Center.y);
		float overlapZ = m1BoundingBox.Extents.z + m2BoundingBox.Extents.z - abs(m1BoundingBox.Center.z - m2BoundingBox.Center.z);

		//プレイヤーのx, z軸への移動成分
		float playerMoveX = 0.0f;// XMVectorGetX(vMove);
		float playerMoveZ = 0.0f;// XMVectorGetZ(vMove);
		if (overlapX < overlapY)
		{
			if (overlapX < overlapZ)
			{
				//X方向の衝突処理
				if (m1BoundingBox.Center.x > m2BoundingBox.Center.x)
				{
					//m1がm2より右にある場合
					direction = eDirection::X_RIGHT;
					if (playerMoveX < 0.0f)
					{
						model->addPositionX(-playerMoveX);
					}
					model->addPositionX(overlapX);
				}
				else
				{
					//m1がm2より左にある場合
					direction = eDirection::X_LEFT;
					if (playerMoveX > 0.0f)
					{
						model->addPositionX(-playerMoveX);
					}
					model->addPositionX(-overlapX);
				}
			}
			else
			{
				//Z方向の衝突処理
				if (m1BoundingBox.Center.z > m2BoundingBox.Center.z)
				{
					//m1がm2より手前にある場合
					direction = eDirection::Z_FRONT;
					if (playerMoveZ < 0.0f)
					{
						model->addPositionZ(-playerMoveZ);
					}
					model->addPositionZ(overlapZ);
				}
				else
				{
					//m1がm2より後ろにある場合
					direction = eDirection::Z_BACK;
					if (playerMoveZ > 0.0f)
					{
						model->addPositionZ(-playerMoveZ);
					}
					model->addPositionZ(-overlapZ);
				}
			}
		}
		else
		{
			if (overlapY < overlapZ)
			{
				//Y方向の衝突処理
				if (m1BoundingBox.Center.y > m2BoundingBox.Center.y)
				{
					//m1がm2より上にある場合
					direction = eDirection::Y_TOP;
					model->addPositionY(overlapY);
					if (XMVectorGetY(velocity) < 0.0f)
					{
						velocity = XMVectorZero();	//速度ベクトルをリセット
					}
					isJump = true;	//ジャンプ可能フラグを有効にする
				}
				else
				{
					//m1がm2より下にある場合
					direction = eDirection::Y_DOWN;
					model->addPositionY(-overlapY);
					velocity = XMVectorZero();	//速度ベクトルをリセット
				}
			}
			else
			{
				//Z方向の衝突処理
				if (m1BoundingBox.Center.z > m2BoundingBox.Center.z)
				{
					//m1がm2より手前にある場合
					direction = eDirection::Z_FRONT;
					if (playerMoveZ < 0.0f)
					{
						model->addPositionZ(-playerMoveZ);
					}
					model->addPositionZ(overlapZ);
				}
				else
				{
					//m1がm2より後ろにある場合
					direction = eDirection::Z_BACK;
					if (playerMoveZ > 0.0f)
					{
						model->addPositionZ(-playerMoveZ);
					}
					model->addPositionZ(-overlapZ);
				}
			}
		}
		return true;
	}
	else
	{
		direction = eDirection::NONE;
		return false;
	}
}
