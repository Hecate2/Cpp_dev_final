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

USING_NS_CC;


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
	// ��ʼ��Physics
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);

	
	//auto spritecache = SpriteFrameCache::getInstance();
	//spritecache->addSpriteFramesWithFile("player.plist");
	
	_player = player::create("UP_02.png");
	_player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
	this->addChild(_player);
	
	
	//�����й��ڹ���Ķ���֮ǰ���ڲ�����ײ�Ĵ��룬��Ϊ������ܻ��õ�����ʱ����
	// ��ʼ����������������������ִ����һ����ÿ�����г��򶼻����һ�����������
	srand((unsigned int)time(nullptr));
	// ÿ��1.5������һ������
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addMonster), 1);
	auto eventListener = EventListenerTouchOneByOne::create();
	// ����ص�����onTouchBegan():��ָ��һ��������Ļʱ�����á�
	eventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	// ʹ��EventDispatcher��������ָ������¼����紥�������������¼���
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

	// ��ײ���
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	// creating a keyboard event listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
bool MainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* event)
{	//keyboard callbackfunction to controll the player
	log("Key with keycode %d pressed", keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode== EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D) {
		auto animation = Animation::create();
		char nameSize[20];
		char openfile[11]="UP_0%d.png";
		auto moveBy = MoveBy::create(0.1, Vec2(0, 5));
		_player->set_direction(player::UP);
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			openfile[0] = 'L';
			openfile[1] = 'F';
			moveBy = MoveBy::create(0.1, Vec2(-5, 0));
			_player->set_direction(player::LEFT);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			openfile[0] = 'D';
			openfile[1] = 'O';
			moveBy = MoveBy::create(0.1, Vec2(0,-5));
			_player->set_direction(player::DOWN);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			openfile[0] = 'R';
			openfile[1] = 'I';
			moveBy = MoveBy::create(0.1, Vec2(5, 0));
			_player->set_direction(player::RIGHT);
			break;
		default:
			break;
		}
		for (int i = 1; i < 3; i++) {
			sprintf(nameSize, openfile, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
		animation->setDelayPerUnit(0.2f);
		animation->setLoops(1);
		animation->setRestoreOriginalFrame(false);
		auto animate_up = Animate::create(animation);
		
		auto seq = Spawn::create(moveBy,animate_up, nullptr);
		_player->runAction(seq);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_J) {
		auto projectile = Sprite::create("bullet.png");
		projectile->setPosition(_player->getPosition());
		auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		projectile->setPhysicsBody(physicsBody);
		projectile->setTag(10);
		this->addChild(projectile);
		Vec2 shootAmount;
		weapon tmp_weapon;
		tmp_weapon = _player->get_weapon_attribute();
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
		auto realDest = shootAmount + projectile->getPosition();
		int normal_speed = 5;
		auto actionMove = MoveTo::create(0.5f*normal_speed/tmp_weapon.speed, realDest);
		auto actionRemove = RemoveSelf::create();
		projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
	}
	return true;
}

void MainScene::addMonster(float dt) {
	auto monster = Sprite::create("Monster.png");

	// Add monster
	auto monsterContentSize = monster->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minY = monsterContentSize.height / 2;
	int maxY = selfContentSize.height - minY;
	int rangeY = maxY - minY;
	int randomY = (rand() % rangeY) + minY;
	monster->setPosition(Vec2(selfContentSize.width + monsterContentSize.width / 2, randomY));
	// Add monster's physicsBody
	auto physicsBody = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	monster->setPhysicsBody(physicsBody);

	// Add projectile's physicsBody


	this->addChild(monster);

	// Let monster run
	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = (rand() % rangeDuration) + minDuration;

	// �����ƶ���object
	// ��randomDuration���ʱ����(2-4����)���ù������Ļ�ұ��ƶ�����ߡ�(�����п�����)
	auto actionMove = MoveTo::create(randomDuration, Vec2(-monsterContentSize.width / 2, randomY));
	// ����������Object�������Ƴ���Ļ���������ͷ���Դ��
	auto actionRemove = RemoveSelf::create();
	monster->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

void MainScene::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();
}

// HelloWorldScene.cpp
bool MainScene::onTouchBegan(Touch* touch, Event* unused_event) {
	// 1 - Just an example for how to get the player object
	// ˵��һ����Ϊ�ڶ����������ݸ�addEventListenerWithSceneGraphPriority(eventListener, _player)��_player���󱻷��ʵķ�ʽ��
	// auto node = unused_event->getcurrentTarget();

	// 2.��ȡ����������꣬����������������_player��ƫ������
	Vec2 touchLocation = touch->getLocation();
	Vec2 offset = touchLocation - _player->getPosition();
	// ���offset��xֵ�Ǹ�ֵ��������������ͼ����������ڱ���Ϸ�����ǲ�����ġ�
	if (offset.x < 0) {
		return true;
	}

	// 3.��������ڵ�λ�ô���һ�����ڣ�������ӵ������С�
	auto projectile = Sprite::create("bullet.png");
	projectile->setPosition(_player->getPosition());
	auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	projectile->setPhysicsBody(physicsBody);
	projectile->setTag(10);
	this->addChild(projectile);

	// 4.��ƫ����ת��Ϊ��λ������������Ϊ1��������
	offset.normalize();
	// �������1000����ͻ����һ��ָ���û���������ĳ���Ϊ1000��������Ϊʲô��1000�أ���Ϊ����Ӧ�����Գ�����ǰ�ֱ�������Ļ�ı߽硣
	auto shootAmount = offset * 1000;
	// ����������ӵ����ڵ�λ����ȥ�������������һ��Ŀ��λ�á�
	auto realDest = shootAmount + projectile->getPosition();

	// 5.����һ����������������2�����ƶ���Ŀ��λ�ã�Ȼ�����ӳ������Ƴ���
	auto actionMove = MoveTo::create(2.0f, realDest);
	auto actionRemove = RemoveSelf::create();
	projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));

	return true;
}

bool MainScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 10)
		{
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == 10)
		{
			nodeA->removeFromParentAndCleanup(true);
		}
	}

	return true;
}


