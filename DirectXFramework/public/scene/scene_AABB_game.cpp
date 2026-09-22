#include "../../framework.h"
#include "../../Framework/environment.h"

bool SceneAABBGame::initialize()
{
	//天球
	pSky = new vnModel(L"data/model/", L"skydome.vnm");
	pSky->setScale(10.0f, 10.0f, 10.0f);
	pSky->setLighting(false);

	//地面
	pGround = new vnModel(L"data/model/", L"ground.vnm");

	//フェンスの配置
	for (int i = 0; i < FENCE_NUM; i++)
	{
		//フェンスモデルを作成
		pFence[i] = new vnModel(L"data/model/", L"fence.vnm");
		//配置角度
		float degree = (float)i * 360.0f / (float)FENCE_NUM;
		//Degree -> Radian変換
		float radian = degree * 3.141592f / 180.0f;
		//極座標 -> 直交座標
		//※角度(rotY)と配置(posXZ)が一致するようにx=sin,z=cosにする
		float x = sinf(radian) * FENCE_RADIUS;
		float z = cosf(radian) * FENCE_RADIUS;

		//フェンスに姿勢情報を設定
		pFence[i]->setPosition(x, 0.0f, z);
		pFence[i]->setRotationY(radian);
	}

	//プレイヤー
	pPlayer = new cObjAABB(L"data/model/bear/", L"bear.vnm");
	//AABBのサイズを設定
	pPlayer->size = XMVectorSet(1.5f, 3.0f, 1.5f, 0.0f);
	pPlayer->center = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);

	//UI
	pUI = new vnSprite(125.0f, 80.0f, 250.0f, 160.0f, L"");	//テクスチャなし
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{	//半透明黒
		pUI->vtx[i].r = 0.0f;
		pUI->vtx[i].g = 0.0f;
		pUI->vtx[i].b = 0.0f;
		pUI->vtx[i].a = 0.5f;
	}

	//オブジェクトをシーンに登録
	registerObject(pSky);
	registerObject(pGround);
	registerObject(pPlayer);
	for (int i = 0; i < FENCE_NUM; i++)
	{
		registerObject(pFence[i]);
	}
	registerObject(pUI);

	//ステージ上のAABBオブジェクト(作成&登録)
	AABBNum = 0;

	//初期情報
	float rad = -90.0f / 180.0f * 3.141592f;
	float radius = 20.0f;
	float height = 1.0f;
	for (int i = 0; i < AABB_MAX-1; i++)
	{
		//高さに依存するランダム強度(0.5f～1.0f)
		float rate = (float)i / (float)AABB_MAX * 0.5f + 0.5f;

		//中心からの配置半径(ランダム)
		float inc_rad = 20.0f + 5.0f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;

		//座標
		float px = cosf(rad) * radius;
		float pz = sinf(rad) * radius;
		float py = height;
		//サイズ
		float sx = 2.0f + 1.5f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;
		float sy = 2.0f + 1.5f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;
		float sz = 2.0f + 1.5f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;

		//オブジェクトを作成
		pBox[AABBNum++] = createObjAABB(px, py, pz, sx, sy, sz);

		//角度の増加分(ランダム)
		float inc_ang = 20.0f + 10.0f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;
		//高さの増加分(ランダム)
		float inc_hit = 1.0f + 1.0f * (float)((rand() % 2000) / 1000.0f - 1.0f) * rate;

		//次のオブジェクト用の変数
		rad += inc_ang / 180.0f * 3.141892f;
		height += inc_hit;
	}
	//ゴール地点
	pBox[AABBNum++] = createObjAABB(cosf(rad) * radius, height, sinf(rad) * radius, 20.0f, 0.2f, 20.0f);

	//ライト方向(天球画像の太陽位置となんとなく合わせて調整)
	vnLight::setLightDir(2.0f, -3.0f, 1.0f);

	//カメラ関連初期設定
	XMVECTOR cam_trg = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);
	vnCamera::setTarget(&cam_trg);

	cam_theta = 0.0f;
	cam_phi = XMConvertToRadians(10.0f);
	cam_distance = 15.0f;
	init_theta = cam_theta;
	init_phi = cam_phi;
	init_distance = cam_distance;
	CameraTrgRate = 0.75f;
	CameraPosRate = 0.50f;

	camSpeedTheta = 0.1f;
	camSpeedPhi = 0.05f;
	camSpeedRadius = 0.2f;
	camBias = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);

	//プレイヤー関連初期設定
	speed = 0.2f;

	//ゲーム管理
	peekHeight = 0.0f;

	//各種初期化
	air = true;
	velocity = XMVectorSet(0.0f, 0.0f, 0.0f ,0.0f);
	gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);
	jumpForce = XMVectorSet(0.0f, 0.25f, 0.0f, 0.0f);

	startPos = XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f);
	pPlayer->setPosition(&startPos);

	//開始準備

	//初期化した値で再計算させておく(CameraTrg, CameraPos)
	CameraTrg = XMVectorAdd(*pPlayer->getPosition(), camBias);
	vnCamera::setTarget(&CameraTrg);
	operateCamera(true);
	vnCamera::setPosition(&CameraPos);

	savePeek();


	return true;
}

void SceneAABBGame::terminate()
{
	deleteObject(pSky);
	deleteObject(pGround);
	deleteObject(pPlayer);
	for (int i = 0; i < FENCE_NUM; i++)
	{
		deleteObject(pFence[i]);
	}
	for (int i = 0; i < AABB_MAX; i++)
	{
		if (pBox[i] != NULL)
		{
			deleteObject(pBox[i]);
		}
	}
	deleteObject(pUI);
}

//処理関数
void SceneAABBGame::execute()
{
	//プレイヤーの移動
	bool update = movePlayer(pPlayer);

	//ステージオブジェクトとの衝突
	for (int i = 0; i < AABB_MAX; i++)
	{
		if (pBox[i] == NULL)continue;

		eDirection dir = collideAABB(pPlayer, pBox[i]);

		if (dir == eDirection::Y_Pos)	//上に乗った
		{
			velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			air = false;
		}
		else if (dir == eDirection::Y_Neg)	//下からぶつかった
		{
			velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	//ジャンプの発動
	if (air == false && vnKeyboard::trg(DIK_SPACE))
	{
		air = true;
		velocity = jumpForce;
	}
	
	//物理挙動(落下)
	velocity += gravity;
	pPlayer->addPosition(&velocity);

	//地面処理
	if (pPlayer->getPositionY() < 0.0f)
	{
		pPlayer->setPositionY(0.0f);
		velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		air = false;
	}
	checkFence(pPlayer);

	//カメラの注視点をプレイヤーの相対的な位置に置く(処理順はプレイヤーの座標が確定後)
	CameraTrg = XMVectorAdd(*pPlayer->getPosition(), camBias);

	//カメラの座標計算
	operateCamera(true);

	//カメラの補間
	// (計算で求めた位置、注視点に実際のカメラが少し遅れて追従するように線形補間する)
	XMVECTOR c_trg = XMVectorLerp(*vnCamera::getTarget(), CameraTrg, CameraTrgRate);
	XMVECTOR c_pos = XMVectorLerp(*vnCamera::getPosition(), CameraPos, CameraPosRate);
	vnCamera::setTarget(&c_trg);
	vnCamera::setPosition(&c_pos);

	//カメラが天球からはみ出すことがないように常にカメラの位置に置く
	pSky->setPosition(vnCamera::getPosition());

	//ゲームの管理
	if (air == false && pPlayer->getPositionY() > peekHeight)
	{
		savePeek();
	}

	//リトライ処理
	if (air == false && vnKeyboard::trg(DIK_R))
	{
		pPlayer->setPosition(&peekPos);
		pPlayer->setRotation(&peekRot);
		cam_phi = XMVectorGetX(peekCam);
		cam_theta = XMVectorGetY(peekCam);
		cam_distance = XMVectorGetZ(peekCam);

		CameraTrg = XMVectorAdd(*pPlayer->getPosition(), camBias);
		vnCamera::setTarget(&CameraTrg);
		operateCamera(true);
		vnCamera::setPosition(&CameraPos);
	}

	//画面表示
	vnFont::setColor(0xffffffff);
	vnFont::print(10.0f, 10.0f, L"HEIGHT : %.3f / %.3f", pPlayer->getPositionY(), peekHeight);

	vnFont::print(10.0f, 50.0f, L"WASD : 移動");
	vnFont::print(10.0f, 70.0f, L"SPACE : ジャンプ");
	vnFont::print(10.0f, 90.0f, L"↑↓←→ : カメラ回転");
	vnFont::print(10.0f, 110.0f, L"PAGE UP/DOWN : カメラ距離");
	vnFont::print(10.0f, 130.0f, air ? 0x7fffffff: 0xffffffff, L"R : リトライ");

	//vnDebugDraw::Grid();
	//vnDebugDraw::Axis();

	vnScene::execute();
}

//描画関数
void SceneAABBGame::render()
{
	vnScene::render();
}

bool SceneAABBGame::movePlayer(vnObject *p)
{
	float lx = vnJoystick::isValid() ? vnJoystick::leftX() : (vnKeyboard::on(DIK_A) == true ? -1.0f : (vnKeyboard::on(DIK_D) == true ? 1.0f : 0.0f));
	float ly = vnJoystick::isValid() ? vnJoystick::leftY() : (vnKeyboard::on(DIK_W) == true ? -1.0f : (vnKeyboard::on(DIK_S) == true ? 1.0f : 0.0f));
	if (lx == 0.0f && ly == 0.0f)return false;

	XMVECTOR cam_pos = *vnCamera::getPosition();
	XMVECTOR cam_trg = *vnCamera::getTarget();
	XMVECTOR dist = cam_pos - cam_trg;

	dist = XMVector3Normalize(dist);

	float rot = atan2f(XMVectorGetX(dist), XMVectorGetZ(dist));

	XMMATRIX mtx;
	mtx = XMMatrixRotationY(rot);

	XMVECTOR move = XMVectorSet(lx, 0.0f, -ly, 0.0f);
	XMVECTOR move_r;
	move_r = XMVector3TransformNormal(move, mtx);

	rot = atan2f(XMVectorGetX(move_r), XMVectorGetZ(move_r)) + 3.141592f;

	p->setRotationY(rot);

	move_r = XMVectorMultiply(move_r, XMVectorSet(-speed, -speed, -speed, 0.0f));
	p->addPosition(&move_r);

	return true;
}

SceneAABBGame::eDirection SceneAABBGame::collideAABB(cObjAABB* p1, cObjAABB* p2)
{
	eDirection ret = eDirection::None;

	if (p1 == NULL || p2 == NULL)return ret;

	XMVECTOR range = XMVectorAdd(p1->size*0.5f, p2->size*0.5f);
	float rx = XMVectorGetX(range);
	float ry = XMVectorGetY(range);
	float rz = XMVectorGetZ(range);

	//AABB中心のワールド座標
	XMVECTOR center1 = XMVectorAdd(*p1->getPosition(), p1->center);
	XMVECTOR center2 = XMVectorAdd(*p2->getPosition(), p2->center);
	//中心座標の差分(絶対値)
	XMVECTOR dif = XMVectorSubtract(center1, center2);
	dif = XMVectorAbs(dif);

	float dx = XMVectorGetX(dif);
	float dy = XMVectorGetY(dif);
	float dz = XMVectorGetZ(dif);

	if (dx < rx && dy < ry && dz < rz)	//衝突判定
	{
		//重なっている部分の長さ(最少の要素の方向に押し戻す)
		float sx = rx - dx;
		float sy = ry - dy;
		float sz = rz - dz;
		if (sx < sy)
		{
			if (sx < sz)
			{	//xが最少
				if (XMVectorGetX(center1) < XMVectorGetX(center2))
				{
					p1->addPositionX(-sx);
					ret = eDirection::X_Neg;
				}
				else
				{
					p1->addPositionX(sx);
					ret = eDirection::X_Pos;
				}
			}
			else
			{	//zが最少
				if (XMVectorGetZ(center1) < XMVectorGetZ(center2))
				{
					p1->addPositionZ(-sz);
					ret = eDirection::Z_Neg;
				}
				else
				{
					p1->addPositionZ(sz);
					ret = eDirection::Z_Pos;
				}
			}
		}
		else
		{
			if (sy < sz)
			{	//yが最少
				if (XMVectorGetY(center1) < XMVectorGetY(center2))
				{
					p1->addPositionY(-sy);
					ret = eDirection::Y_Neg;
				}
				else
				{
					p1->addPositionY(sy);
					ret = eDirection::Y_Pos;
				}
			}
			else
			{	//zが最少
				if (XMVectorGetZ(center1) < XMVectorGetZ(center2))
				{
					p1->addPositionZ(-sz);
					ret = eDirection::Z_Neg;
				}
				else
				{
					p1->addPositionZ(sz);
					ret = eDirection::Z_Pos;
				}
			}
		}
	}
	return ret;
}

bool SceneAABBGame::operateCamera(bool update)
{
	float rx = vnJoystick::isValid() ? vnJoystick::rightX() : (vnKeyboard::on(DIK_LEFT) == true ? -1.0f : (vnKeyboard::on(DIK_RIGHT) == true ? 1.0f : 0.0f));
	float ry = vnJoystick::isValid() ? vnJoystick::rightY() : (vnKeyboard::on(DIK_DOWN) == true ? -1.0f : (vnKeyboard::on(DIK_UP) == true ? 1.0f : 0.0f));
	bool l1 = vnJoystick::isValid() ? vnJoystick::on(4) : vnKeyboard::on(DIK_PGDN) == true;
	bool r1 = vnJoystick::isValid() ? vnJoystick::on(5) : vnKeyboard::on(DIK_PGUP) == true;
	if (rx != 0.0f || ry != 0.0f)
	{
		cam_theta += (float)rx * camSpeedTheta;
		cam_phi += (float)ry * camSpeedPhi;
		if (cam_phi > 87.0f * 3.141592f / 180.0f)		//カメラ角度上限
		{
			cam_phi = 87.0f * 3.141592f / 180.0f;
		}
		else if (cam_phi < -87.0f * 3.141592f / 180.0f)	//カメラ角度下限
		{
			cam_phi = -87.0f * 3.141592f / 180.0f;
		}
		update = true;
	}
	else if (l1 == true)
	{
		cam_distance += camSpeedRadius;
		update = true;
	}
	else if (r1 == true)
	{
		cam_distance += -camSpeedRadius;
		if (cam_distance < 1.0f)cam_distance = 1.0f;
		update = true;
	}

	if (update)
	{
		XMVECTOR v = XMVectorSet(0.0f, 0.0f, -cam_distance, 0.0f);
		XMMATRIX rotate = XMMatrixRotationRollPitchYaw(cam_phi, -cam_theta, 0.0f);

		v = XMVector3TransformCoord(v, rotate);

		v = XMVectorAdd(v, *vnCamera::getTarget());

		CameraPos = v;
	}

	return update;
}

//移動範囲の制限
void SceneAABBGame::checkFence(vnModel* p)
{
	if (!p)return;
	XMVECTOR v = *p->getPosition();
	v = XMVectorSetY(v, 0.0f);	//高さ成分は無視する
	XMVECTOR vLength = XMVector3Length(v);
	float length = XMVectorGetX(vLength);
	if (length > FIELD_RADIUS)
	{
		XMVECTOR nv = XMVector3Normalize(v);
		nv = XMVectorScale(nv, FIELD_RADIUS);
		nv = XMVectorSetY(nv, p->getPositionY());	//高さを維持する
		p->setPosition(&nv);
	}
}

//ゲーム情報の確保
void SceneAABBGame::savePeek()
{
	peekHeight = pPlayer->getPositionY();
	peekPos = *pPlayer->getPosition();
	peekRot = *pPlayer->getRotation();
	peekCam = XMVectorSet(cam_phi, cam_theta, cam_distance, 0.0f);
}

cObjAABB* SceneAABBGame::createObjAABB(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ)
{
	cObjAABB *ret = new cObjAABB(L"data/model/primitive/", L"cube.vnm");
	ret->setScale(sizeX * 0.5f, sizeY * 0.5f, sizeZ * 0.5f);
	ret->setPosition(posX, posY, posZ);
	ret->size = XMVectorSet(sizeX, sizeY, sizeZ, 0.0f);
	ret->center = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	registerObject(ret);
	return ret;
}

//コンストラクタ
cObjAABB::cObjAABB(const WCHAR* folder, const WCHAR* file) : vnModel(folder, file)
{
	center = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	size = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
}
