#include "cocos2d.h"
#include "monster.h"
#include <cmath>
#define BLOODBAR_TAG 1



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

int getdirection(cocos2d::Vec2 diff){
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

void BigMonster::move(int dir) {
	//0->上  1->下  2->左   3->右
	int distance = 80;
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
	this->runAction(seq);
}

void BigMonster::attack(cocos2d::Vec2 diff) {
	//僵尸朝向攻击的方向
	int dir = getdirection(diff);
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
	
	
	//创建火球
	auto fireball = Sprite::create("fireball.png");
	fireball->setPosition(cocos2d::Vec2(75,75));
	auto physicsBody = cocos2d::PhysicsBody::createBox(fireball->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	fireball->setPhysicsBody(physicsBody);
	fireball->setTag(5);
	//添加到场景中
	this->addChild(fireball);
	// 将偏移量转化为单位向量，即长度为1的向量。
	diff.normalize();
	// 将其乘以1000，你就获得了一个指向用户触屏方向的长度为2000的向量。为什么是2000呢？因为长度应当足以超过当前分辨率下屏幕的边界。
	auto shootAmount = diff * 2000;
	// 5.创建一个动作，在12秒内移动到目标位置，然后将它从场景中移除。
	auto actionMove = cocos2d::MoveBy::create(12.0f, shootAmount);
	auto actionRemove = cocos2d::RemoveSelf::create();
	fireball->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));

}

