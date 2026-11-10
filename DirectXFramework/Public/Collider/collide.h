//--------------------------------------------------------------//
//	"vn_collide.h"												//
//		衝突演算のインターフェース								//
//													2026/11/10	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once	//インクルードガード

namespace vnCollide
{
	//形状：直線
	struct stLine
	{
		XMVECTOR Pos;	//始点
		XMVECTOR Dir;	//方向（正規化）

		//２点から直線の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2);
	};

	//形状：平面
	struct stPlane
	{
		XMVECTOR Normal;	//平面の法線
		float Distance;		//原点からの距離

		//３点から平面の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//形状：三角形
	struct stTriangle
	{
		stPlane plane;	//内包
		XMVECTOR v[3];	//三角形の頂点座標

		//３点から三角形の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//形状：球
	struct stSphere
	{
		XMVECTOR Pos;	//中心座標
		float Radius;	//半径
	};

	/// <summary>
	/// 直線と点の最近傍距離を示す直線上の点を求める
	/// </summary>
	/// <param name="ret">[out] 直線上の点を格納するアドレス</param>
	/// <param name="line">[in] 直線の情報</param>
	/// <param name="pos">[in] 点の座標</param>
	void closesPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos);

	/// <summary>
	/// 直線と平面の交点を調べる
	/// </summary>
	/// <param name="hit">[out] 衝突座標を格納するアドレス</param>
	/// <param name="line">[in] 直線の情報</param>
	/// <param name="plane">[in] 平面の情報</param>
	/// <returns>true = 衝突している, false = 衝突していない</returns>
	bool isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane);

	/// <summary>
	/// 直線と三角形の交点を調べる
	/// </summary>
	/// <param name="hit">[out] 衝突座標を格納するアドレス</param>
	/// <param name="line">[in] 直線の情報</param>
	/// <param name="plane">[in] 三角形の情報</param>
	/// <returns>true = 衝突している, false = 衝突していない</returns>
	bool isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri);

	/// <summary>
	/// 直線と球の交点を調べる
	/// </summary>
	/// <param name="hit">[out] 衝突座標を格納する配列の先頭アドレス</param>
	/// <param name="line">[in] 直線の情報</param>
	/// <param name="plane">[in] 球の情報</param>
	/// <returns>交点の数</returns>
	int isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere);

}
