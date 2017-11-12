﻿#include "Samus.h"

#define SPEED_X 0.4f
#define SPEED_Y 0.6f
#define MAX_HEIGHT 150.0f

Samus::Samus(void) : DynamicObject()
{
}

Samus::Samus(int _posX, int _posY) : DynamicObject(_posX, _posY, 0, 0, EnumID::Samus_ID)
{
	_action = Action::Idle;
	_allowPress = true;
	_vLast = 0.2f;
	_hasJump = false;
	_hasRoll = false;
	_hasTurnUp = false;
	_hasShot = false;
	_heightJump = 0.0f;
	_isDraw = true;
	bullet = new list<Bullet*>();;
	samusJump1 = new CSprite(Singleton::getInstance()->getTexture(EnumID::SamusJump1_ID), 0, 0, 300);
	samusJump2 = new CSprite(Singleton::getInstance()->getTexture(EnumID::SamusJump2_ID), 0, 3, 10);
	samusRoll = new CSprite(Singleton::getInstance()->getTexture(EnumID::SamusRoll_ID), 0, 3, 10);
	samusShotUp = new CSprite(Singleton::getInstance()->getTexture(EnumID::SamusShotUp_ID), 1, 3, 20);
	samusShot = new CSprite(Singleton::getInstance()->getTexture(EnumID::SamusShot_ID), 1, 3, 20);
	Initialize();
}

void Samus::Update(int deltaTime)
{
	//List các đạn được bắn
	list<Bullet*>::iterator it = bullet->begin();
	while (it != bullet->end())
	{
		if (!(*it)->active)
			bullet->erase(it++); //xóa đạn khi đạn ko còn active
		else
		{
			(*it)->Update(deltaTime);
			++it;
		}
	}
	
	switch (_action)
	{
		case Action::Run_Left:
			sprite->Update(deltaTime);
			break;
		case Action::Run_Right:
			sprite->Update(deltaTime);
			break;	
	}
	
	//Update vị trí
	posX += vX *deltaTime;
	posY += vY *deltaTime;

	//Nhảy
	if (_hasJump)
	{
		samusJump2->Update(deltaTime); //tạm thời set jump của samus là kiểu nhảy 2
		_heightJump += vY * deltaTime;
		if (_heightJump >= MAX_HEIGHT) //nếu nhảy lên max height thì cho rớt xuống
		{
			vY = -(SPEED_Y);
		}
		if (posY < 129) //nếu chạm đất (tạm thời set đất là 129 do chưa gắn gạch)
		{
			vY = 0;
			vX = 0;
			posY = 129;
			_hasJump = false;
		}
	}

	//Lăn tròn
	if (_hasRoll)
	{
		samusRoll->Update(deltaTime);
	
	}

	//Bắn lên
	if (_hasTurnUp)
	{
		posY = 135;  //tạm thời set posY = 135
		if (_action == Action::Run_Left || _action == Action::Run_Right)		
			samusShotUp->Update(deltaTime);
	}
	else if (!_hasJump) posY = 129;

	//Bắn
	if (_hasShot)
	{
		//Nếu vừa chạy vừa bắn
		if (_action == Action::Run_Left || _action == Action::Run_Right)
			samusShot->Update(deltaTime);
	}
}

void Samus::Draw(CCamera* camera)
{
	D3DXVECTOR2 center = camera->Transform(posX, posY);
	//draw list đạn
	for (list<Bullet*>::iterator i = bullet->begin(); i != bullet->end(); i++)
	{
		if ((*i)->active)
			(*i)->Draw(camera, posX, posY);
	}

	if (vX > 0 || _vLast>0)
	{
		if (_hasJump)
		{
			samusJump2->Draw(center.x, center.y);
			return;
		}
		if (_hasRoll)
		{
			samusRoll->Draw(center.x, center.y);
			return;
		}
		if (_hasTurnUp)
		{
			samusShotUp->Draw(center.x, center.y);
			return;
		}
		if (_hasShot)
		{
			samusShot->Draw(center.x, center.y);
			return;
		}
		sprite->Draw(center.x, center.y);
		
	}
	else
	{
		if (_hasJump)
		{
			samusJump2->DrawFlipX(center.x, center.y);
			return;
		}
		if (_hasRoll)
		{
			samusRoll->DrawFlipX(center.x, center.y);
			return;
		}
		if (_hasTurnUp)
		{
			samusShotUp->DrawFlipX(center.x, center.y);
			return;
		}
		if (_hasShot)
		{
			samusShot->DrawFlipX(center.x, center.y);
			return;
		}
		sprite->DrawFlipX(center.x, center.y);
		
	}
}

void Samus::TurnLeft()
{
	if (_allowPress)
	{
		vX = -SPEED_X;
		_vLast = vX;
		_action = Action::Run_Left;
	}
}

void Samus::TurnRight()
{
	if (_allowPress)
	{
		vX = SPEED_X;
		_vLast = vX;
		_action = Action::Run_Right;

	}
}

void Samus::Jump()
{
	if (_allowPress)
	{
		if (!_hasJump)
		{
			vY = SPEED_Y;
			_heightJump = 0;
			samusJump2->SelectIndex(0);
			_action = Action::Jump2; //tạm thời chọn kiểu jump2
			_hasJump = true;
		}
	}
}

void Samus::Roll()
{
	if (_allowPress)
	{
		_hasRoll = true;
		_action = Action::Roll;
	}
}

void Samus::TurnUp()
{
	if (_allowPress)
	{
		_hasRoll = false;
		_hasTurnUp = true;
		samusShotUp->SelectIndex(0);
	}
}

void Samus::Initialize()
{
}

void Samus::Stop()
{
	if (!_hasJump) vX = 0;
	_hasTurnUp = false;
	_hasShot = false;
	_action = Action::Idle;
	sprite->SelectIndex(0);
}


void Samus::Shot()
{
	if (_allowPress)
	{
		bullet->push_back(new Bullet(posX, posY+11, _vLast, EnumID::Bullet_ID));
		_hasShot = true;
	}
}



Samus::~Samus(void)
{
}

D3DXVECTOR2* Samus::getPos()
{
	return new D3DXVECTOR2(this->posX, this->posY);
}


