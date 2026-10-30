#include "../../framework.h"
#include "../../Framework/environment.h"
#include "player.h"

Player::Player(const WCHAR* folder, const WCHAR* file, const XMFLOAT3& boxColliderCenter, const XMFLOAT3& boxColliderExtents)
{
	model = new vnModel(folder, file);
	aabbCollider.initialize(boxColliderCenter, boxColliderExtents);	//AABB‚Ì‰Šú‰»
}

Player::~Player()
{
	//ƒƒ‚ƒŠ‰ğ•ú
	delete(model);
}

void Player::execute()
{

}

void Player::render()
{
#ifdef _DEBUG
	if (aabbCollider.getEnable())
	{
		aabbCollider.debugLineRenderer();
	}
#endif // _DEBUG
}

AABBCollider& Player::getAABBCollider()
{
	return aabbCollider;
}
