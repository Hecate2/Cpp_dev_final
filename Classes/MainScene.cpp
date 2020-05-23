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
	// 初始化Physics
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
	
	
	//下面有关于怪物的都是之前用于测试碰撞的代码，因为后面可能会用到，暂时保留
	// 初始化了随机数生成器。如果不执行这一步，每次运行程序都会产生一样的随机数。
	srand((unsigned int)time(nullptr));
	// 每隔1.5秒生成一个怪物
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addMonster), 1);
	auto eventListener = EventListenerTouchOneByOne::create();
	// 定义回调函数onTouchBegan():手指第一次碰到屏幕时被调用。
	eventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	// 使用EventDispatcher来处理各种各样的事件，如触摸和其他键盘事件。
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

	// 碰撞检测
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

	// 定义移动的object
	// 在randomDuration这个时间内(2-4秒内)，让怪物从屏幕右边移动到左边。(怪物有快有慢)
	auto actionMove = MoveTo::create(randomDuration, Vec2(-monsterContentSize.width / 2, randomY));
	// 定义消除的Object。怪物移出屏幕后被消除，释放资源。
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
	// 说明一下作为第二个参数传递给addEventListenerWithSceneGraphPriority(eventListener, _player)的_player对象被访问的方式。
	// auto node = unused_event->getcurrentTarget();

	// 2.获取触摸点的坐标，并计算这个点相对于_player的偏移量。
	Vec2 touchLocation = touch->getLocation();
	Vec2 offset = touchLocation - _player->getPosition();
	// 如果offset的x值是负值，这表明玩家正试图朝后射击。在本游戏中这是不允许的。
	if (offset.x < 0) {
		return true;
	}

	// 3.在玩家所在的位置创建一个飞镖，将其添加到场景中。
	auto projectile = Sprite::create("bullet.png");
	projectile->setPosition(_player->getPosition());
	auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	projectile->setPhysicsBody(physicsBody);
	projectile->setTag(10);
	this->addChild(projectile);

	// 4.将偏移量转化为单位向量，即长度为1的向量。
	offset.normalize();
	// 将其乘以1000，你就获得了一个指向用户触屏方向的长度为1000的向量。为什么是1000呢？因为长度应当足以超过当前分辨率下屏幕的边界。
	auto shootAmount = offset * 1000;
	// 将此向量添加到飞镖的位置上去，这样你就有了一个目标位置。
	auto realDest = shootAmount + projectile->getPosition();

	// 5.创建一个动作，将飞镖在2秒内移动到目标位置，然后将它从场景中移除。
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


