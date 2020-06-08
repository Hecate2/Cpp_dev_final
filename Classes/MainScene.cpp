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


bool MainScene::init()
{
    //////////////////////////////
    // 1. 首先继承
	if (!Scene::init())
	{
		return false;
	}
	// 物理效果初始化用于继承
	if (!Scene::initWithPhysics())
	{
		return false;
	}
<<<<<<< Updated upstream
=======
	getPhysicsWorld();
	getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//窗口尺寸和原点
>>>>>>> Stashed changes
	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
<<<<<<< Updated upstream
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);
	this->addBarrel();
	this->addBox();
	//auto spritecache = SpriteFrameCache::getInstance();
	//spritecache->addSpriteFramesWithFile("player.plist");
=======
	//设置游戏背景
	auto background = Sprite::create("background.png");
	if (background) {
		background->setPosition(Vec2(origin.x + CENTER_X, origin.y + CENTER_Y));
		background->setScale(1.0f);
		this->addChild(background);
	}
<<<<<<< Updated upstream
=======
	background->setGlobalZOrder(-1);	//显示在最下层
	current_score_label = Label::createWithTTF("Score:0000", "score.ttf", 24);
	current_score_label->setPosition(Vec2(winSize.width * 0.5, winSize.height*0.9));
	this->addChild(current_score_label);
	//渲染顺序的调度器
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::set_z_oder));
>>>>>>> Stashed changes
	
	//玩家初始化
>>>>>>> Stashed changes
	_player = player::create("UP_02.png");
	_player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
	_player->init();
	_player->change_weapon(1);
	auto physicsBody = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	_player->setPhysicsBody(physicsBody);
<<<<<<< Updated upstream
	_player->setTag(PLAYER_TAG);
	this->addChild(_player);
<<<<<<< Updated upstream
	//renew the blood timely
	schedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood), 0.1f);  //renew the display of the blood
	schedule(CC_SCHEDULE_SELECTOR(MainScene::addhp), 1.0f);//add hp every 1s
	
=======
	this->addBarrel(Vec2(50, 50));
	this->addBox();

	this->addBigMonster(0);

	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_move), 1.0, -1, 0);
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_attack), 1.0, -1, 0);
	//renew the blood timely
	schedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood), 0.1f);  //renew the display of the blood
	schedule(CC_SCHEDULE_SELECTOR(MainScene::addhp), 1.0f);//add hp every 1s
	schedule(CC_SCHEDULE_SELECTOR(MainScene::always_move), 0.1f);
>>>>>>> Stashed changes
	// detects the contaction of bullet and barrel
=======
	_player->setTag(PLAYER_TAG);//设立标签
	this->addChild(_player); //添加进场景
	//添加油漆桶，箱子和大怪物
	this->addBarrel(Vec2(50, 50));
	this->addBox(Vec2(300, 350));
	this->addBigMonster(0);
	for (int i = 0; i < 10; i++) {
		this->addSmallMonster(1);
	}
	//僵尸的移动和攻击调度器和死亡
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_move), 3.0, -1, 0);
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_attack), 1.0, -1, 0);
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::monster_death), 1.0, -1, 0);

	//实时更新血量
	schedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood), 0.1f);  //更新血条的显示
	schedule(CC_SCHEDULE_SELECTOR(MainScene::addhp), 1.0f);//每秒钟自动回血

	//人物移动调度器
	schedule(CC_SCHEDULE_SELECTOR(MainScene::always_move), 0.2f);
	
	//碰撞检测
	// 子弹和油漆桶
>>>>>>> Stashed changes
	auto contactListener0 = EventListenerPhysicsContact::create();
	contactListener0->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin_bullet_barrel, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener0, this);
	//箱子和玩家
	auto contactListener1 = EventListenerPhysicsContact::create();
	contactListener1->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin_player_box, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener1, this);

	// 键盘回调事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
<<<<<<< Updated upstream
=======
//用于控制人物移动、攻击、换枪的按键回调函数
>>>>>>> Stashed changes
bool MainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* event)
{	//keyboard callbackfunction to controll the player
	log("Key with keycode %d pressed", keyCode);
	//控制人物移动
	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode== EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D) {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
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
=======
		player_move(keyCode);
>>>>>>> Stashed changes
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
=======
		_player->player_move(keyCode);
	}
	keys[keyCode] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_J) {
		player_attack();
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

=======
void MainScene::player_attack() {
	//当前武器子弹耗尽
	if (_player->Is_out_of_bullet()) {
		change_weapon_animation((_player->get_weapon_attribute()).weapon_name, true);
		return;
	}
	//当前武器为油漆桶
	if ((_player->get_weapon_attribute()).weapon_name == "barrel") {
		int tmp_barrel_distance = 300;
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
		addBarrel(_player->getPosition() + shootAmount);
		_player->decrease_weapon_num();
		_player->renew_display_num();
		return ;
	}
	//其他枪类武器事
	auto bullet = Sprite::create("bullet.png");
	bullet->setPosition(_player->getPosition());
	//增加physics用于碰撞
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
		shootAmount = Vec2(-tmp_weapon.distance, 0);
		break;
	case player::RIGHT:
		shootAmount = Vec2(tmp_weapon.distance, 0);
		break;
	}
	_player->decrease_weapon_num(); //更新子弹数
	_player->renew_display_num();//更新显示
	//子弹的动作
	auto realDest = shootAmount + bullet->getPosition();
	int normal_speed = 5;
	auto actionMove = MoveTo::create(0.5f * normal_speed / tmp_weapon.speed, realDest);
	auto actionRemove = RemoveSelf::create();
	bullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}
>>>>>>> Stashed changes
bool MainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	keys[keyCode] = false;
	return true;
}
void MainScene::always_move(float delta) {//用于长时间按的调度函数
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
		_player->player_move(keyCode);
	}
	
}
<<<<<<< Updated upstream
void MainScene::player_move(cocos2d::EventKeyboard::KeyCode keyCode) {
	auto animation = Animation::create();
	char nameSize[20];
	char openfile[11] = "UP_0%d.png";
	auto moveBy = MoveBy::create(0.2, Vec2(0, 10));
	_player->set_direction(player::UP);
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		openfile[0] = 'L';
		openfile[1] = 'F';
		moveBy = MoveBy::create(0.2, Vec2(-10, 0));
		_player->set_direction(player::LEFT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		openfile[0] = 'D';
		openfile[1] = 'O';
		moveBy = MoveBy::create(0.2, Vec2(0, -10));
		_player->set_direction(player::DOWN);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		openfile[0] = 'R';
		openfile[1] = 'I';
		moveBy = MoveBy::create(0.2, Vec2(10, 0));
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

	auto seq = Spawn::create(moveBy, animate_up, nullptr);
	_player->runAction(seq);
}
//用于控制人物移动、攻击、换枪的按键回调函数
=======

//换枪的动画
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream

>>>>>>> Stashed changes
=======
//血条显示更新
>>>>>>> Stashed changes
void MainScene::scheduleBlood(float delta) {
	auto progress = (ProgressTimer*)_player->getChildByTag(1);
	
	progress->setPercentage((((float)(_player->get_hp())) / 100) * 100);  //这里是百分制显示
	if (progress->getPercentage() < 0) {
		this->unschedule(CC_SCHEDULE_SELECTOR(MainScene::scheduleBlood));
	}
}
<<<<<<< Updated upstream
void MainScene::addBox() {
=======

void MainScene::addBox(cocos2d::Vec2 pos) {
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
void MainScene::addBarrel() {
=======
void MainScene::addBarrel(const cocos2d::Vec2& s) {
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
bool MainScene::onContactBegin_player_fireball(cocos2d::PhysicsContact& contact){
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	bool contact_flag = false;
	Vec2 position_tmp1;
	Vec2 pos1, pos2, diff;
	
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == FIREBALL_TAG && nodeB->getTag() == PLAYER_TAG){
			position_tmp1 = nodeA->getPosition();
			pos1 = nodeA->getParent()->convertToWorldSpace(position_tmp1);
			pos2 = nodeB->getPosition();			

			nodeA->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
		else if (nodeB->getTag() == FIREBALL_TAG && nodeA->getTag() == PLAYER_TAG)
		{
			position_tmp1 = nodeB->getPosition();
			pos1 = nodeB->getParent()->convertToWorldSpace(position_tmp1);
			pos2 = nodeA->getPosition();

			nodeB->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
	}
	if (contact_flag) {
		diff = pos1 - pos2;
		int dir = getdirection(diff);	//0->上方  1->下  2->左   3->右
		_player->Is_under_attack(dir);
	}
	return true;
}
//子弹 僵尸
bool MainScene::onContactBegin_bullet_monster(cocos2d::PhysicsContact& contact){
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	BigMonster* a = nullptr;
	SmallMonster* b = nullptr;
	bool contact_flag = false;
	bool contact_flag2 = false;
	Vec2 pos1, pos2, diff;
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == BOSS_TAG && nodeB->getTag() == BULLET_TAG)
		{
			a = (BigMonster*)nodeA;	
			pos1 = nodeA->getPosition();
			pos2 = nodeB->getPosition();
			if (a->hp > 0)
				nodeB->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
		else if (nodeB->getTag() == BOSS_TAG && nodeA->getTag() == BULLET_TAG)
		{
			a = (BigMonster*)nodeB;
			pos2 = nodeA->getPosition();
			pos1 = nodeB->getPosition();
			if (a->hp > 0)
				nodeA->removeFromParentAndCleanup(true);
			contact_flag = true;
		}
		else if (nodeA->getTag() == SMALL_TAG && nodeB->getTag() == BULLET_TAG)
		{
			b = (SmallMonster*)nodeA;
			pos1 = nodeA->getPosition();
			pos2 = nodeB->getPosition();
			if (b->hp > 0)
				nodeB->removeFromParentAndCleanup(true);
			contact_flag2 = true;
		}
		else if (nodeB->getTag() == SMALL_TAG && nodeA->getTag() == BULLET_TAG)
		{
			b = (SmallMonster*)nodeB;
			pos2 = nodeA->getPosition();
			pos1 = nodeB->getPosition();
			if (b->hp > 0)
				nodeA->removeFromParentAndCleanup(true);
			contact_flag2 = true;
		}
	}
	if (contact_flag) {
		diff = pos1 - pos2;
		if(a->hp>0)
			a->under_attack(diff,_player->get_weapon_attribute().Damage/5*20);
	}
	if (contact_flag2) {
		diff = pos1 - pos2;
		if (b->hp > 0)
			b->under_attack(diff, _player->get_weapon_attribute().Damage / 5 * 20);
	}
	return true;
}

>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
//添加boss僵尸的方法 	birthpoint: 0->上  1->下  2->左   3->右
void MainScene::addBigMonster(int birth_point) {
	auto boss = BigMonster::create("Boss_DO_01.png");
	_BigMonster.push_back(boss);
	//根据出生地选择初始的位置  768*1024
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

//僵尸移动的调度器
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
//僵尸攻击的调度器
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
<<<<<<< Updated upstream

>>>>>>> Stashed changes
=======
//僵尸死亡调度器
void MainScene::monster_death(float dt){
	for (auto k : _BigMonster) {
		if (k->hp <= 0) {
			k->death();
			score += 1000;
			current_score_label->setString("Score" + std::to_string(score));
		}
	}
	_BigMonster.erase(remove_if(_BigMonster.begin(), _BigMonster.end(), 
		[](BigMonster* x) {  return (x->hp <= 0); }), _BigMonster.end());
	for (auto k : _SmallMonster) {
		if (k->hp <= 0) {
			k->death();
			score += 100;
			current_score_label->setString("Score" + std::to_string(score));
		}
	}
	_SmallMonster.erase(remove_if(_SmallMonster.begin(), _SmallMonster.end(),
		[](SmallMonster* x) { return (x->hp <= 0); }), _SmallMonster.end());
}
>>>>>>> Stashed changes

