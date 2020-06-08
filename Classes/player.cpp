#include "cocos2d.h"
#include "player.h"
#define BLOODBAR_TAG 1
#define PLAYER_MOVE_ACTION_TAG 2
player* player::create(const std::string& filename)
{
	player* pSprite = new (std::nothrow) player();
	if (pSprite && pSprite->initWithFile(filename))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}
bool player::init() {
	//initialize weapon store
	weapon_store.resize(4);
	weapon_store[0] = weapon(100, 5, 5, 200,100, "pistol");
	weapon_store[1] = weapon(100, 10, 5, 200, 100,"UZI");
	weapon_store[2] = weapon(20, 5, 20, 150, 20, "shotgun");
	weapon_store[3] = weapon(10, 0, 0, 0, 10, "barrel");
	//initialze current weapon
	current_weapon = &(weapon_store[0]);
	current_weapon_label = cocos2d::Label::createWithSystemFont("pistol", "Arial", 15);
	current_weapon_label->setPosition(cocos2d::Vec2(80, 180));
	this->addChild(current_weapon_label);
	//initialize hp bar
	auto sprite = Sprite::create("bar.png");   
	sprite->setPosition(cocos2d::Vec2(this->getPosition().x-20, 150)); 
	this->addChild(sprite);            
	auto sprBlood = Sprite::create("blood.png");  //create blood bar
	cocos2d::ProgressTimer* progress = cocos2d::ProgressTimer::create(sprBlood); //create progress
	progress->setType(cocos2d::ProgressTimer::Type::BAR);        //settype bar
	progress->setPosition(cocos2d::Vec2(this->getPosition().x-20, 150));
	//decrease from right to left
	progress->setMidpoint(cocos2d::Point(0, 0.5));    
	progress->setBarChangeRate(cocos2d::Point(1, 0));
	progress->setTag(BLOODBAR_TAG);       //make a tag
	this->addChild(progress);
	return true;
}

void player::player_move(cocos2d::EventKeyboard::KeyCode keyCode) {
	auto animation = cocos2d::Animation::create();
	char nameSize[20];
	char openfile[11] = "UP_0%d.png";

	float distance = 20;
	float time = 0.2;

	auto moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	this->set_direction(player::UP);
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		openfile[0] = 'L';
		openfile[1] = 'F';
		moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
		this->set_direction(player::LEFT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		openfile[0] = 'D';
		openfile[1] = 'O';
		moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
		this->set_direction(player::DOWN);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		openfile[0] = 'R';
		openfile[1] = 'I';
		moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
		this->set_direction(player::RIGHT);
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
	auto animate_up = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveBy, animate_up, nullptr);
	this->runAction(seq);
}


void player::Is_under_attack(int dir) {
	under_attack = true;
	this->hp -= 30;
	stopActionByTag(PLAYER_MOVE_ACTION_TAG);	//停掉移动的动作

	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//移动
	switch (dir) {
	case 0:moveby = move_down; break;
	case 1:moveby = move_up; break;
	case 2:moveby = move_right; break;
	case 3:moveby = move_left; break;
	default:
		moveby = move_up;
		break;
	}
	//相应的移动动画
	auto animation = cocos2d::Animation::create();
	//添加图片资源
	char openfile[50] = "UP_02.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[0] = 'D'; openfile[1] = 'O'; break;
	case 2:openfile[0] = 'L'; openfile[1] = 'F'; break;
	case 3:openfile[0] = 'R'; openfile[1] = 'I'; break;
	default:break;
	}
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);

	//设置属性
	animation->setRestoreOriginalFrame(false); //还原第一帧
	animation->setDelayPerUnit(0.5);     //单位帧间隔
	animation->setLoops(0);                  //静止的图片
	//创建动画
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	//auto delay = cocos2d::DelayTime::create(2);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
	under_attack = false;
}