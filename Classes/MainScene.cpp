/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "AppMacros.h"
#include "math.h"
USING_NS_CC;
#define BOX_TAG 7
#define BULLET_TAG 10
#define BARREL_TAG 8
#define PLAYER_TAG 2

Scene* MainScene::scene()
{
     return MainScene::create();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	// Initialze with Physics
	if (!Scene::initWithPhysics())
	{
		return false;
	}
	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//������Ϸ����
	auto background = Sprite::create("background.png");
	if (background) {
		background->setPosition(Vec2(origin.x + CENTER_X, origin.y + CENTER_Y));
		background->setScale(1.0f);
		this->addChild(background);
	}
	background->setGlobalZOrder(-1);	//��ʾ�����²�

	//��Ⱦ˳��ĵ�����
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::set_z_oder));
	
	//��ҳ�ʼ��
	_player = player::create("DO_02.png");
	_player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
	_player->init();
	_player->change_weapon(1);
	auto physicsBody = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	_player->setPhysicsBody(physicsBody);
	_player->setTag(PLAYER_TAG);
	this->addChild(_player);
	this->addBarrel(Vec2(50, 50));
	this->addBox();

	this->addBigMonster(0);

	//��ʬ���ƶ��͹���������
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_move), 3.0, -1, 0);
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_attack), 1.0, -1, 0);

	//renew the blood timely
	schedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood), 0.1f);  //renew the display of the blood
	schedule(CC_SCHEDULE_SELECTOR(MainScene::addhp), 1.0f);//add hp every 1s
	schedule(CC_SCHEDULE_SELECTOR(MainScene::always_move), 0.2f);
	// detects the contaction of bullet and barrel
	auto contactListener0 = EventListenerPhysicsContact::create();
	contactListener0->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin_bullet_barrel, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener0, this);
	//detects the contaction of box and player
	auto contactListener1 = EventListenerPhysicsContact::create();
	contactListener1->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin_player_box, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener1, this);

	// creating a keyboard event listener to control the player
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//���ڿ��������ƶ�����������ǹ�İ����ص�����
bool MainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* event)
{	//keyboard callbackfunction to controll the player
	log("Key with keycode %d pressed", keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode== EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D) {
		player_move(keyCode);
	}
	keys[keyCode] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_J) {
		if (_player->Is_out_of_bullet()) {
			change_weapon_animation((_player->get_weapon_attribute()).weapon_name,true);
			return true;
		}
		if ((_player->get_weapon_attribute()).weapon_name == "barrel") {
			int tmp_barrel_distance=50;
			Vec2 shootAmount;
			switch (_player->get_direction()) {
			case player::UP:
				shootAmount = Vec2(0, tmp_barrel_distance);
				break;
			case player::DOWN:
				shootAmount = Vec2(0, -tmp_barrel_distance);
				break;
			case player::LEFT:
				shootAmount = Vec2(-tmp_barrel_distance, 0);
				break;
			case player::RIGHT:
				shootAmount = Vec2(tmp_barrel_distance, 0);
				break;
			}
			addBarrel(_player->getPosition()+shootAmount);
			_player->decrease_weapon_num();
			_player->renew_display_num();
			return true;
		}
		auto bullet = Sprite::create("bullet.png");
		bullet->setPosition(_player->getPosition());
		//add physics
		auto physicsBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		bullet->setPhysicsBody(physicsBody);
		bullet->setTag(10);
		this->addChild(bullet);
		Vec2 shootAmount;
		auto tmp_weapon = _player->get_weapon_attribute();
		switch (_player->get_direction()) {
		case player::UP:
			shootAmount = Vec2(0, tmp_weapon.distance);
			break;
		case player::DOWN:
			shootAmount = Vec2(0, -tmp_weapon.distance);
			break;
		case player::LEFT:
			shootAmount = Vec2(-tmp_weapon.distance,0);
			break;
		case player::RIGHT:
			shootAmount = Vec2(tmp_weapon.distance,0);
			break;
		}
		_player->decrease_weapon_num();
		_player->renew_display_num();
		auto realDest = shootAmount + bullet->getPosition();
		int normal_speed = 5;
		auto actionMove = MoveTo::create(0.5f*normal_speed/tmp_weapon.speed, realDest);
		auto actionRemove = RemoveSelf::create();
		bullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_1 || keyCode == EventKeyboard::KeyCode::KEY_2 ||
		keyCode == EventKeyboard::KeyCode::KEY_3 || keyCode == EventKeyboard::KeyCode::KEY_4) {
		bool success_change;
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_1:
			success_change=_player->change_weapon(0);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_2:
			success_change = _player->change_weapon(1);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_3:
			success_change = _player->change_weapon(2);
			break;
		default:
			success_change = _player->change_weapon(3);
			break;
		}
		if (success_change)
			change_weapon_animation((_player->get_weapon_attribute()).weapon_name);
	}
	return true;
}

bool MainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	keys[keyCode] = false;
	return true;
}
void MainScene::always_move(float delta) {
	EventKeyboard::KeyCode keyCode= EventKeyboard::KeyCode::KEY_P;
	if (keys[EventKeyboard::KeyCode::KEY_W])
			keyCode = EventKeyboard::KeyCode::KEY_W;
	else if (keys[EventKeyboard::KeyCode::KEY_A])
		keyCode = EventKeyboard::KeyCode::KEY_A;
	else if (keys[EventKeyboard::KeyCode::KEY_S])
		keyCode = EventKeyboard::KeyCode::KEY_S;
	else if (keys[EventKeyboard::KeyCode::KEY_D])
		keyCode = EventKeyboard::KeyCode::KEY_D;
	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D) {
		player_move(keyCode);
	}
	
}
void MainScene::player_move(cocos2d::EventKeyboard::KeyCode keyCode) {
	auto animation = Animation::create();
	char nameSize[20];
	char openfile[11] = "UP_0%d.png";

	float distance = 20;
	float time = 0.2;

	auto moveBy = MoveBy::create(time, Vec2(0, distance));
	_player->set_direction(player::UP);
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		openfile[0] = 'L';
		openfile[1] = 'F';
		moveBy = MoveBy::create(time, Vec2(-distance, 0));
		_player->set_direction(player::LEFT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		openfile[0] = 'D';
		openfile[1] = 'O';
		moveBy = MoveBy::create(time, Vec2(0, -distance));
		_player->set_direction(player::DOWN);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		openfile[0] = 'R';
		openfile[1] = 'I';
		moveBy = MoveBy::create(time, Vec2(distance, 0));
		_player->set_direction(player::RIGHT);
		break;
	default:
		break;
	}
	for (int i = 1; i < 3; i++) {
		sprintf(nameSize, openfile, i);
		animation->addSpriteFrameWithFile(nameSize);
	}
	animation->setDelayPerUnit(0.1f);
	animation->setLoops(1);
	animation->setRestoreOriginalFrame(false);
	auto animate_up = Animate::create(animation);

	auto seq = Spawn::create(moveBy, animate_up, nullptr);
	_player->runAction(seq);
}
//��ǹ�Ķ���
void MainScene::change_weapon_animation(const std::string& weapon_name,bool out_of_bullet) {
	auto change_label = cocos2d::Label::createWithSystemFont("change:"+weapon_name, "Arial", 20);
	if (out_of_bullet)
		change_label->setString("Out of " + weapon_name);
	change_label->setColor(Color3B::BLACK);
	change_label->setPosition(Vec2(512,0));
	this->addChild(change_label);
	auto actionMove = MoveBy::create(0.3f , Vec2(0,100));
	auto actionRemove = RemoveSelf::create();
	change_label->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

void MainScene::scheduleBlood(float delta) {
	auto progress = (ProgressTimer*)_player->getChildByTag(1);
	
	progress->setPercentage((((float)(_player->get_hp())) / 100) * 100);  //�����ǰٷ�����ʾ
	if (progress->getPercentage() < 0) {
		this->unschedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood));
	}
}
void MainScene::addBox() {
	auto Box = Sprite::create("box.png");
	// Add Box
	auto BoxContentSize = Box->getContentSize();
	Box->setPosition(Vec2(100, 100));
	// Add Box's physicsBody
	auto physicsBody = PhysicsBody::createBox(Box->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	Box->setPhysicsBody(physicsBody);
	Box->setTag(BOX_TAG);
	// Add Box's physicsBody
	this->addChild(Box);
}
void MainScene::addBarrel(const cocos2d::Vec2& s) {
	auto Barrel = Sprite::create("barrel.png");
	// Add Barrel
	auto BarrelContentSize = Barrel->getContentSize();
	Barrel->setPosition(s);
	// Add Barrel's physicsBody
	auto physicsBody = PhysicsBody::createBox(Barrel->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	Barrel->setPhysicsBody(physicsBody);
	Barrel->setTag(BARREL_TAG);
	// Add Barrel's physicsBody
	this->addChild(Barrel);
}

void MainScene::addhp(float delta) {
	_player->add_hp_timely();
}
void MainScene::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();
}
/*
void MainScene::mov_monsters(float delta) {
	int n = monster_group.size();
	for (int i = 0; i < n; i++) {
		auto mon_tmp = monster_group[i];
		auto mon_pos = mon_tmp->getPosition();
		monster_group[i]->set_direction(Judgedirection(mon_pos));
		monster_group[i]->move();
	}
}

MainScene::Direction MainScene::Judgedirection(const Vec2& mon_pos) {
	auto delta_dis =  _player->getPosition()-mon_pos;
	if (abs(delta_dis.x) >= abs(delta_dis.y)) {
		if (delta_dis.x < 0)
			return LEFT;//left
		else
			return RIGHT;//right
	}
	else {
		if (delta_dis.y < 0)
			return UP;//up
		else
			return DOWN;//down
	}
}
*/

bool MainScene::onContactBegin_bullet_barrel(cocos2d::PhysicsContact& contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == BARREL_TAG && nodeB->getTag() == BULLET_TAG)
		{
			auto position_tmp = nodeA->getPosition();
			if (abs(_player->getPosition().x - position_tmp.x)+abs(_player->getPosition().y - position_tmp.y) < 500) {
				_player->decrease_hp_barrel();
			}
			nodeA->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == BARREL_TAG && nodeA->getTag() ==BULLET_TAG)
		{
			auto position_tmp = nodeB->getPosition();
			if (abs(_player->getPosition().x - position_tmp.x)+abs(_player->getPosition().y - position_tmp.y) < 500) {
				_player->decrease_hp_barrel();
			}
			nodeB->removeFromParentAndCleanup(true);
		}
	}
	return true;
}

bool MainScene::onContactBegin_player_box(cocos2d::PhysicsContact& contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	bool contact_flag = false;
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == BOX_TAG && nodeB->getTag() ==PLAYER_TAG)
		{
			auto position_tmp = nodeA->getPosition();
			nodeA->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
		else if (nodeB->getTag() == BOX_TAG && nodeA->getTag() == PLAYER_TAG)
		{
			auto position_tmp = nodeB->getPosition();
			nodeB->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
	}
	if (contact_flag) {
		_player->add_bullet_random();
		_player->renew_display_num();
	}
	return true;
}

//���boss��ʬ�ķ��� 	birthpoint: 0->��  1->��  2->��   3->��
void MainScene::addBigMonster(int birth_point) {
	auto boss = BigMonster::create("Boss_DO_01.png");
	_BigMonster.push_back(boss);
	//���ݳ�����ѡ���ʼ��λ��  768*1024
	switch (birth_point) {
	case 0: {
		boss->setPosition(Vec2(512, 786));
		break;
	}
	case 1: {
		boss->setPosition(Vec2(512, 0));
		break;
	}
	case 2: {
		boss->setPosition(Vec2(0, 393));
		break;
	}
	case 3: {
		boss->setPosition(Vec2(1024, 393));
		break;
	}
	default: {
		boss->setPosition(Vec2(300, 300));
		break;
	}
	}

	this->addChild(boss);

}

//��������Ŀ������Ⱦ˳��ĵ���������������⣬���ڵ�����
void MainScene::set_z_oder(float dt){
	//_player->setLocalZOrder(get_z_odre(_player));
	auto all_children = getChildren();
	for (auto k : all_children) {
		k->setLocalZOrder(get_z_odre(k));
	}
	
}

int MainScene::get_z_odre(cocos2d::Node* spr){
	auto p = spr->getPosition();
	int ry = 800*1024 - p.y*1024;
	int x = p.x;
	return ry+x;
}

//��ʬ�ƶ��ĵ�����
void MainScene::monster_move(float dt) {
	auto destination = _player->getPosition();
	for (auto k : _BigMonster) {
		auto source = k->getPosition();
		auto diff = destination - source;
		if (fabs(diff.x) > 200 || fabs(diff.y) > 200) {
			auto direction = getdirection(diff);
			k->move(direction);
		}
	}

}
//��ʬ�����ĵ�����
void MainScene::monster_attack(float dt) {
	auto destination = _player->getPosition();
	for (auto k : _BigMonster) {
		auto source = k->getPosition();
		auto diff = destination - source;
		if (fabs(diff.x) < 200 && fabs(diff.y) < 200) {
			auto direction = getdirection(diff);
			k->attack(diff);
		}
	}

}


