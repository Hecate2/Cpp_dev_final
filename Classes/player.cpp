#include "cocos2d.h"
#include "player.h"
#include "AudioEngine.h"
#define BLOODBAR_TAG 1
#define PLAYER_MOVE_ACTION_TAG 2
#define BULLET_TAG 10
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
bool player::init() {//初始化
	init_weapon_system();//初始化武器库和显示
	init_hp_bar();//初始化血条显示
	return true;
}
void player::player_invincible() {
	invincible = !invincible;
}
void player::init_weapon_system() {
	//初始化武器库
	weapon_store.resize(4);
	weapon_store[0] = weapon(100, 5, 5, 200, 100, "pistol");
	weapon_store[1] = weapon(100, 10, 5, 200, 100, "UZI");
	weapon_store[2] = weapon(20, 5, 20, 150, 20, "shotgun");
	weapon_store[3] = weapon(10, 0, 0, 0, 10, "barrel");
	//初始化当前武器为pistol
	current_weapon = &(weapon_store[0]);
	//初始化当前武器显示的label,显示于人物上方
	current_weapon_label = cocos2d::Label::createWithSystemFont("pistol", "Arial", 15);
	current_weapon_label->setPosition(cocos2d::Vec2(80, 180));
	this->addChild(current_weapon_label);
	return;
}

void player::init_hp_bar() {
	//创建血条外的方框
	auto sprite = Sprite::create("bar.png");
	sprite->setPosition(cocos2d::Vec2(this->getPosition().x - 20, 150));
	this->addChild(sprite);
	//创建血条
	auto sprBlood = Sprite::create("blood.png");  //create blood bar
	cocos2d::ProgressTimer* progress = cocos2d::ProgressTimer::create(sprBlood); //create progress
	progress->setType(cocos2d::ProgressTimer::Type::BAR);        //settype bar
	progress->setPosition(cocos2d::Vec2(this->getPosition().x - 20, 150));
	//从右往左减少血量
	progress->setMidpoint(cocos2d::Point(0, 0.5));
	progress->setBarChangeRate(cocos2d::Point(1, 0));
	progress->setTag(BLOODBAR_TAG);       //make a tag
	this->addChild(progress);
	return;
}
bool player::change_weapon(int weapon_option) {//切换武器以及显示
	if (!weapon_store[weapon_option].enable)
		return false;
	current_weapon = &(weapon_store[weapon_option]);
	if (weapon_option)
		current_weapon_label->setString(current_weapon->weapon_name + ":" + std::to_string(current_weapon->num));
	else
		current_weapon_label->setString(current_weapon->weapon_name);
	return true;
}

void player::add_bullet() {//与补给箱碰撞后补齐所有武器
	for (int i = 1; i < 4; i++) {
		if (weapon_store[i].num < weapon_store[i].max_num) {
			weapon_store[i].num = weapon_store[i].max_num;
		}
	}
}
void player::player_move(cocos2d::EventKeyboard::KeyCode keyCode) {//根据键盘移动角色，并播放相应动画
	auto animation = cocos2d::Animation::create();
	char nameSize[20];
	char openfile[11] = "UP_0%d.png";

	float distance = 20;
	float time = 0.2;
	//默认移动方向向上
	auto moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	this->set_direction(player::UP);
	//根据输入修改移动方向和改变读取动作动画文件
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
	//读入方向对应帧动画文件
	for (int i = 1; i < 3; i++) {
		sprintf(nameSize, openfile, i);
		animation->addSpriteFrameWithFile(nameSize);
	}
	animation->setDelayPerUnit(0.1f);//每帧之间时间间隔
	animation->setLoops(1);//循环次数
	animation->setRestoreOriginalFrame(false);//动画结束后显示是否回到动画播放前
	auto animate_up = cocos2d::Animate::create(animation);//创建动画
	auto seq = cocos2d::Spawn::create(moveBy, animate_up, nullptr);//创建方向移动和动画同时进行的序列
	this->runAction(seq);//播放序列
}

void player::Is_under_attack(int dir) {//人物收到攻击后后退
	if (!invincible) {
		this->hp -= 30;
		stopActionByTag(PLAYER_MOVE_ACTION_TAG);	//停掉移动的动作
	}
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//根据输入方向选择僵直后退方向
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
	auto animate = cocos2d::Animate::create(animation); //创建动画
	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);//移动和动画同时进行
	this->runAction(cocos2d::Sequence::create(seq, nullptr));//播放序列
}

void player::player_attack() {
	//其他枪类武器时	
	auto bullet = Sprite::create("bullet.png");
	auto soundEffectID = cocos2d::AudioEngine::play2d("gameeffect.mp3", false);//攻击音效
	bullet->setPosition(this->getContentSize()/2);
	//增加physics用于碰撞
	auto physicsBody = cocos2d::PhysicsBody::createBox(bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	bullet->setPhysicsBody(physicsBody);

	bullet->setTag(BULLET_TAG);//设置标签
	this->addChild(bullet);

	cocos2d::Vec2 shootAmount;
	auto tmp_weapon = this->get_weapon_attribute();//获得当前武器属性
	//根据当前武器的距离属性选择子弹在确定方向上的攻击范围
	switch (this->get_direction()) {
	case player::UP:
		shootAmount = cocos2d::Vec2(0, tmp_weapon.distance);
		break;
	case player::DOWN:
		shootAmount = cocos2d::Vec2(0, -tmp_weapon.distance);
		break;
	case player::LEFT:
		shootAmount = cocos2d::Vec2(-tmp_weapon.distance, 0);
		break;
	case player::RIGHT:
		shootAmount = cocos2d::Vec2(tmp_weapon.distance, 0);
		break;
	}
	if (!invincible) {
		this->decrease_weapon_num(); //减少子弹数
	}
	this->renew_display_num();//更新显示
	//子弹的动作
	auto realDest = shootAmount + bullet->getPosition();//实际子弹轨迹终点
	int normal_speed = 5;//正常速度
	//根据武器速度属性设定子弹攻击速度
	auto actionMove = cocos2d::MoveTo::create(0.5f * normal_speed / tmp_weapon.speed, realDest);
	auto actionRemove = cocos2d::RemoveSelf::create();//子弹移除
	bullet->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));//播放序列
}


