#include "cocos2d.h"
#include "monster.h"
#include <cmath>
#define FIREBALL_TAG 11
#define MOVE_ACTION_TAG 1
#define SMALL_MOVE_ACTION_TAG 2

int getdirection(cocos2d::Vec2 diff) {
	//0->上方  1->下  2->左   3->右
	if (diff.y <= 0 && -diff.y >= fabs(diff.x)) {
		//往下移动
		return  1;
	}
	else if (diff.y >= 0 && diff.y >= fabs(diff.x)) {
		//往上移动
		return 0;
	}
	else if (diff.x >= 0 && diff.x >= fabs(diff.y)) {
		//往右移动
		return 3;
	}
	else {
		//往左移动
		return 2;
	}
	return 0;
}

BigMonster* BigMonster::create(const std::string& filename) {
	BigMonster* bigSprite = new(std::nothrow) BigMonster();
	if (bigSprite && bigSprite->initWithFile(filename))
	{
		bigSprite->autorelease();
		return bigSprite;
	}
	CC_SAFE_DELETE(bigSprite);
	return nullptr;
}

void BigMonster::move(int dir) {
	//0->上  1->下  2->左   3->右
	//随机波动
	std::random_device rd;
	auto eng = std::default_random_engine{ rd() };
	auto dis = std::uniform_int_distribution<>{ -40,80 };
	int rand = dis(eng);
	int distance = 80 + rand;
	float time = 3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//移动
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "Boss_UP_0%d.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[5] = 'D'; openfile[6] = 'O'; break;
	case 2:openfile[5] = 'L'; openfile[6] = 'F'; break;
	case 3:openfile[5] = 'R'; openfile[6] = 'I'; break;
	default:
		break;
	}
	for (int i = 1; i <= 2; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(3);                  //-1无限循环
	//创建动画
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	seq->setTag(MOVE_ACTION_TAG);
	this->runAction(seq);
}

void BigMonster::attack(cocos2d::Vec2 diff) {
	//僵尸朝向攻击的方向
	int dir = getdirection(diff);
	stopActionByTag(MOVE_ACTION_TAG);	//停掉移动的动作
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "Boss_UP_0%d.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[5] = 'D'; openfile[6] = 'O'; break;
	case 2:openfile[5] = 'L'; openfile[6] = 'F'; break;
	case 3:openfile[5] = 'R'; openfile[6] = 'I'; break;
	default:break;
	}
	for (int i = 1; i <= 1; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);
	auto seq = cocos2d::Spawn::create(animate, nullptr);
	this->runAction(seq);
	
	if (!is_under_attack) {	//如果被攻击，有僵直，无法攻击
		//创建火球
		auto fireball = Sprite::create("fireball.png");
		fireball->setPosition(cocos2d::Vec2(75, 75));
		auto physicsBody = cocos2d::PhysicsBody::createCircle(20.0f, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		fireball->setPhysicsBody(physicsBody);
		fireball->setTag(FIREBALL_TAG);
		//添加到场景中
		this->addChild(fireball);
		// 将偏移量转化为单位向量，即长度为1的向量。
		diff.normalize();
		// 将其乘以2000，你就获得了一个指向用户触屏方向的长度为2000的向量。为什么是2000呢？因为长度应当足以超过当前分辨率下屏幕的边界。
		auto shootAmount = diff * 2000;
		// 5.创建一个动作，在12秒内移动到目标位置，然后将它从场景中移除。
		auto actionMove = cocos2d::MoveBy::create(12.0f, shootAmount);
		auto actionRemove = cocos2d::RemoveSelf::create();
		fireball->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));
	}
}

void BigMonster::under_attack(cocos2d::Vec2 diff,int de_hp){
	is_under_attack = true;
	this->hp -= de_hp;
	stopActionByTag(MOVE_ACTION_TAG);	//停掉移动的动作
	//后退，向被攻击的反方向
	int dir = getdirection(diff);
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//移动
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "Boss_UP_0%d.png";
	switch (dir) {
	case 1:break;
	case 0:openfile[5] = 'D'; openfile[6] = 'O'; break;
	case 3:openfile[5] = 'L'; openfile[6] = 'F'; break;
	case 2:openfile[5] = 'R'; openfile[6] = 'I'; break;
	default:break;
	}
	for (int i = 1; i <= 1; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby,animate, nullptr);
	//auto delay = cocos2d::DelayTime::create(2);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
	is_under_attack = false;
}

void BigMonster::death() {
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "BOSS_d.png";
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);
	auto fade = cocos2d::FadeTo::create(2, 0);
	auto seq = cocos2d::Spawn::create(fade, animate, nullptr);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
}

SmallMonster* SmallMonster::create(const std::string& filename){
	SmallMonster* smallSprite = new(std::nothrow) SmallMonster();
	if (smallSprite && smallSprite->initWithFile(filename))
	{
		smallSprite->autorelease();
		return smallSprite;
	}
	CC_SAFE_DELETE(smallSprite);
	return nullptr;
}

void SmallMonster::move(int dir){
	//0->上  1->下  2->左   3->右
	//随机波动
	std::random_device rd;
	auto eng = std::default_random_engine{ rd() };
	auto dis = std::uniform_int_distribution<>{ -50,100 };
	int rand = dis(eng);
	int distance = 80 + rand;
	float time = 3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//移动
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "0_0%d.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[0] = '1'; break;
	case 2:openfile[0] = '2'; break;
	case 3:openfile[0] = '3'; break;
	default:
		break;
	}
	for (int i = 1; i <= 2; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(3);                  //-1无限循环
	//创建动画
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	seq->setTag(SMALL_MOVE_ACTION_TAG);
	this->runAction(seq);
}

void SmallMonster::attack(cocos2d::Vec2 diff){
	//僵尸朝向攻击的方向
	int dir = getdirection(diff);
	stopActionByTag(SMALL_MOVE_ACTION_TAG);	//停掉移动的动作
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "a0_0%d.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[1] = '1'; break;
	case 2:openfile[1] = '2'; break;
	case 3:openfile[1] = '3'; break;
	default:
		break;
	}
	for (int i = 1; i <= 2; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(1);                  //
	//创建动画
	auto animate = cocos2d::Animate::create(animation);
	auto seq = cocos2d::Spawn::create(animate, nullptr);
	this->runAction(seq);

	if (!is_under_attack) {	//如果被攻击，有僵直，无法攻击
		//创建空白球
		auto fireball = Sprite::create("blank.png");
		fireball->setPosition(cocos2d::Vec2(75, 75));
		auto physicsBody = cocos2d::PhysicsBody::createCircle(20.0f, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		fireball->setPhysicsBody(physicsBody);
		fireball->setTag(FIREBALL_TAG);
		//添加到场景中
		this->addChild(fireball);
		// 将偏移量转化为单位向量，即长度为1的向量。
		diff.normalize();
		// 将其乘以2000，你就获得了一个指向用户触屏方向的长度为2000的向量。为什么是2000呢？因为长度应当足以超过当前分辨率下屏幕的边界。
		auto shootAmount = diff * 75;
		// 5.创建一个动作，在2秒内移动到目标位置，然后将它从场景中移除。
		auto actionMove = cocos2d::MoveBy::create(1.0f, shootAmount);
		auto actionRemove = cocos2d::RemoveSelf::create();
		fireball->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));
	}
}

void SmallMonster::under_attack(cocos2d::Vec2 diff,int de_hp){
	is_under_attack = true;
	this->hp -= de_hp;
	stopActionByTag(SMALL_MOVE_ACTION_TAG);	//停掉移动的动作
	//后退，向被攻击的反方向
	int dir = getdirection(diff);
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//移动
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "0_0%d.png";
	switch (dir) {
	case 1:break;
	case 0:openfile[0] = '1'; break;
	case 3:openfile[0] = '2'; break;
	case 2:openfile[0] = '3'; break;
	default:break;
	}
	for (int i = 1; i <= 1; i++) {
		char str[50];
		sprintf(str, openfile, i);
		animation->cocos2d::Animation::addSpriteFrameWithFile(str);
	}
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	//auto delay = cocos2d::DelayTime::create(2);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
	is_under_attack = false;
}

void SmallMonster::death(){
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[16] = "Small_d.png";
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);
	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(1);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);
	auto fade = cocos2d::FadeTo::create(2, 0);
	auto seq = cocos2d::Spawn::create(fade, animate, nullptr);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
}
