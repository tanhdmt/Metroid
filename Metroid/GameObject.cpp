﻿
#include "GameObject.h"
#include <d3d9.h>

GameObject::GameObject(void)
{
	posX = 0;
	posY = 0;
	width = 0;
	height = 0;
	canMove = false;
}

void GameObject::SetActive(float x, float y) {}
void GameObject::SetActive()
{
	if (!active)
		active = true;
}

GameObject::GameObject(float _posX, float _posY, EnumID _id)
{
	posX = _posX;
	posY = _posY;
	vX = 0;
	vY = 0;
	id = _id;
	hearts = 0;
	hp = 1;
	damage = 1;
	point = 0;
	type = ObjectType::None_Type;
	canMove = false;
	active = true;
	death = false;
	CreateSprite();
	if (sprite != NULL)
	{
		width = sprite->_texture->FrameWidth;
		height = sprite->_texture->FrameHeight;
	}
}

void GameObject::CreateSprite()
{
	switch (id)
	{
	case EnumID::Samus_ID:
		sprite = new CSprite(Singleton::getInstance()->getTexture(id), 1, 3, 20);
		break;
	case EnumID::Ground1_ID:
	case EnumID::Ground2_ID:
	case EnumID::Ground3_ID:
	case EnumID::Ground4_ID:
	case EnumID::Ground5_ID:
	case EnumID::Ground6_ID:
	case EnumID::Ground7_ID:
	case EnumID::Ground8_ID:
	case EnumID::Ground9_ID:
	case EnumID::Ground10_ID:
	case EnumID::Ground11_ID:
	case EnumID::Ground12_ID:
		sprite = NULL;
		break;
	default:
		sprite = new CSprite(Singleton::getInstance()->getTexture(id), 1);
		break;
	}
}

void GameObject::Collision(list<GameObject*> obj, int dt)
{
}

void GameObject::Update(int deltaTime)
{
	if (sprite != NULL)
		sprite->Update(deltaTime);
}

void GameObject::Draw(CCamera* camera)
{
	if (sprite != NULL)
	{
		D3DXVECTOR2 center = camera->Transform(posX, posY);
		sprite->Draw(center.x, center.y);
	}
}

ECollisionDirect GameObject::GetCollisionDirect(float normalx, float normaly)
{
	if (normalx == 0 && normaly == 1)
	{
		return ECollisionDirect::Colls_Bot;
	}
	if (normalx == 0 && normaly == -1)
	{
		return ECollisionDirect::Colls_Top;
	}
	if (normalx == 1 && normaly == 0)
	{
		return ECollisionDirect::Colls_Left;
	}
	if (normalx == -1 && normaly == 0)
	{
		return ECollisionDirect::Colls_Right;
	}
	return ECollisionDirect::Colls_None;
}

ECollisionDirect GameObject::GetCollisionDirect(GameObject* other)
{
	float x = this->posX - other->posX;
	float y = this->posY - other->posY;
	if (abs(x) > abs(y)) {
		if (x < 0 && x > -36) 
			return ECollisionDirect::Colls_Left;
		else if (x > 0 && x < 36)
 			return ECollisionDirect::Colls_Right;
	}
	else
	{
		if (y < 0) 
			return ECollisionDirect::Colls_Top;
		else
			return ECollisionDirect::Colls_Bot;
	}
	/*Box _myBox = this->GetBox();
	Box _yourBox = other->GetBox();
	if (_myBox.x + _myBox.w <= _yourBox.x) {
		return ECollisionDirect::Colls_Left;
	}
	if (_myBox.x >= _yourBox.x+_yourBox.w) {
		return ECollisionDirect::Colls_Right;
	}
	if (_myBox.y + _myBox.h <= _yourBox.y) {
		return ECollisionDirect::Colls_Top;
	}
	if (_myBox.y >= _yourBox.y + _yourBox.h) {
		return ECollisionDirect::Colls_Bot;
	}*/
	return ECollisionDirect::Colls_None;
}

void GameObject::Remove()
{
	active = false;
	death = true;
}

void GameObject::ReceiveDamage(int damagePoint)
{
	if (hp <= 0)
		return;
	hp -= damagePoint;
	if (hp == 0)
		death = true;
}

Box GameObject::GetBox()
{
	//Box result(posX - width / 2, posY + height / 2, width, height);
	Box result(posX, posY + height / 2, width, height);
	return result;
}

void GameObject::ProcessInput(LPDIRECT3DDEVICE9 d3ddv, int t) {}
void GameObject::OnKeyDown(int KeyCode) {}
GameObject::~GameObject(void) {}