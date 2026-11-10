//--------------------------------------------------------------//
//	"vn_collide.h"												//
//		衝突演算のインターフェース								//
//													2026/11/10	//
//														Ichii	//
//--------------------------------------------------------------//

#include "../../framework.h"
#include "../../Framework/environment.h"

//２点から直線の情報を作成
void vnCollide::stLine::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2)
{
	Pos = *p1;							//始点
	Dir = XMVectorSubtract(*p2, *p1);	//方向
	Dir = XMVector3Normalize(Dir);
}

//３点から平面の情報を作成
void vnCollide::stPlane::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	XMVECTOR v, w;
	v = XMVectorSubtract(*p3, *p2);
	w = XMVectorSubtract(*p2, *p1);
	Normal = XMVector3Cross(v, w);
	Normal = XMVector3Normalize(Normal);
	Distance = XMVectorGetX(XMVector3Dot(Normal, *p1));
}

//３点から三角形の情報を作成
void vnCollide::stTriangle::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	plane.fromPoints(p1, p2, p3);
	v[0] = *p1;
	v[1] = *p2;
	v[2] = *p3;
}

//直線と点の最近傍距離を示す直線上の点を求める
void vnCollide::closesPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos)
{
	XMVECTOR v;
	v = XMVectorSubtract(*pos, line->Dir);

	float dot = XMVectorGetX(XMVector3Dot(v, line->Dir));
	*ret = line->Pos + line->Dir * dot;
}

//直線と平面の交点を調べる
bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane)
{
	float s, d;
	XMVECTOR vS = XMVector3Dot(plane->Normal, line->Pos);
	s = XMVectorGetX(vS);
	s += plane->Distance;

	XMVECTOR vD = XMVector3Dot(plane->Normal, line->Pos + line->Dir);
	d = XMVectorGetX(vD);
	d += plane->Distance;

	float t = s / (s - d);	//傾き
	float dif = s - d;	//floatの差

	*hit = line->Pos + line->Dir * t;

	return true;
}

bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri)
{
	vnCollide::isCollide(hit, line, &tri->plane);

	//衝突判定
	//三角形の辺のベクトル作成
	XMVECTOR edge1 = XMVectorSubtract(tri->v[1], tri->v[0]);
	XMVECTOR edge2 = XMVectorSubtract(tri->v[2], tri->v[1]);
	XMVECTOR edge3 = XMVectorSubtract(tri->v[0], tri->v[2]);

	//衝突点と三角形の各頂点のベクトル作成
	XMVECTOR hitEdge1 = XMVectorSubtract(*hit, tri->v[0]);
	XMVECTOR hitEdge2 = XMVectorSubtract(*hit, tri->v[1]);
	XMVECTOR hitEdge3 = XMVectorSubtract(*hit, tri->v[2]);

	//三角形の辺と衝突点のベクトルの外積を計算（法線ベクトルと同じ方向かどうかを判定するため）
	XMVECTOR cross1 = XMVector3Cross(edge1, hitEdge1);
	XMVECTOR cross2 = XMVector3Cross(edge2, hitEdge2);
	XMVECTOR cross3 = XMVector3Cross(edge3, hitEdge3);

	//外積と法線ベクトルの内積を計算
	cross1 = XMVector3Dot(cross1, tri->plane.Normal);
	cross2 = XMVector3Dot(cross2, tri->plane.Normal);
	cross3 = XMVector3Dot(cross3, tri->plane.Normal);

	if (XMVectorGetY(cross1) > 0.0f && XMVectorGetY(cross2) > 0.0f && XMVectorGetY(cross3) > 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere)
{
	XMVECTOR v = XMVectorSubtract(line->Pos, sphere->Pos);
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);

	XMVECTOR nv = XMVector3Normalize(v);

	XMVECTOR vdt = XMVector3Dot(nv, line->Dir);
	XMVECTOR vdl = XMVector3Length(v);
	float dt = XMVectorGetX(vdt);
	//範囲指定
	if (dt > 1.0f) dt = 1.0f;
	else if (dt < -1.0f) dt = -1.0f;

	float dl = XMVectorGetX(vdl);
	float dr = dl * sqrt(1.0f - dt * dt);	//直線と球中心の最近傍距離

	//球の半径より直線が近ければ衝突
	float dq = sqrtf(sphere->Radius * sphere->Radius - dr * dr);
	float ds = dl * dt;
	float dm = ds - dq;	//直線の視点からの各衝突位置（マイナス側）
	float dp = ds + dq;	//直線の視点からの各衝突位置（プラス側）
	hit[0] = line->Pos + line->Dir * (-dm);
	hit[1] = line->Pos + line->Dir * (-dp);


	if (dr <= sphere->Radius)
	{
		
	}
	else
	{

	}
	return 0;
}
