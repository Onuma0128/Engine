#include "Collider.h"

#include "DirectXEngine.h"

Collider::~Collider()
{
	DirectXEngine::GetCollisionMgr()->RemoveCollider(this);
}

void Collider::AddCollider()
{
	DirectXEngine::GetCollisionMgr()->AddCollider(this);
}
