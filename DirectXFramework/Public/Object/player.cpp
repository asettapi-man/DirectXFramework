#include "../../framework.h"
#include "../../Framework/environment.h"
#include "player.h"

void Player::initialize()
{
	model = new vnModel(L"data/model/bear/", L"bear.vnm");
	//model->setPosition(0.0f, 0.0f, 0.0f);
}

void Player::execute()
{

}

void Player::render()
{
	model->render();

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
