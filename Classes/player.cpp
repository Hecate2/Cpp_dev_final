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
bool player::init() {//��ʼ��
	init_weapon_system();//��ʼ�����������ʾ
	init_hp_bar();//��ʼ��Ѫ����ʾ
	return true;
}
void player::player_invincible() {
	invincible = !invincible;
}
void player::init_weapon_system() {
	//��ʼ��������
	weapon_store.resize(4);
	weapon_store[0] = weapon(100, 5, 5, 200, 100, "pistol");
	weapon_store[1] = weapon(100, 10, 5, 200, 100, "UZI");
	weapon_store[2] = weapon(20, 5, 20, 150, 20, "shotgun");
	weapon_store[3] = weapon(10, 0, 0, 0, 10, "barrel");
	//��ʼ����ǰ����Ϊpistol
	current_weapon = &(weapon_store[0]);
	//��ʼ����ǰ������ʾ��label,��ʾ�������Ϸ�
	current_weapon_label = cocos2d::Label::createWithSystemFont("pistol", "Arial", 15);
	current_weapon_label->setPosition(cocos2d::Vec2(80, 180));
	this->addChild(current_weapon_label);
	return;
}

void player::init_hp_bar() {
	//����Ѫ����ķ���
	auto sprite = Sprite::create("bar.png");
	sprite->setPosition(cocos2d::Vec2(this->getPosition().x - 20, 150));
	this->addChild(sprite);
	//����Ѫ��
	auto sprBlood = Sprite::create("blood.png");  //create blood bar
	cocos2d::ProgressTimer* progress = cocos2d::ProgressTimer::create(sprBlood); //create progress
	progress->setType(cocos2d::ProgressTimer::Type::BAR);        //settype bar
	progress->setPosition(cocos2d::Vec2(this->getPosition().x - 20, 150));
	//�����������Ѫ��
	progress->setMidpoint(cocos2d::Point(0, 0.5));
	progress->setBarChangeRate(cocos2d::Point(1, 0));
	progress->setTag(BLOODBAR_TAG);       //make a tag
	this->addChild(progress);
	return;
}
bool player::change_weapon(int weapon_option) {//�л������Լ���ʾ
	if (!weapon_store[weapon_option].enable)
		return false;
	current_weapon = &(weapon_store[weapon_option]);
	if (weapon_option)
		current_weapon_label->setString(current_weapon->weapon_name + ":" + std::to_string(current_weapon->num));
	else
		current_weapon_label->setString(current_weapon->weapon_name);
	return true;
}

void player::add_bullet() {//�벹������ײ������������
	for (int i = 1; i < 4; i++) {
		if (weapon_store[i].num < weapon_store[i].max_num) {
			weapon_store[i].num = weapon_store[i].max_num;
		}
	}
}
void player::player_move(cocos2d::EventKeyboard::KeyCode keyCode) {//���ݼ����ƶ���ɫ����������Ӧ����
	auto animation = cocos2d::Animation::create();
	char nameSize[20];
	char openfile[11] = "UP_0%d.png";

	float distance = 20;
	float time = 0.2;
	//Ĭ���ƶ���������
	auto moveBy = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	this->set_direction(player::UP);
	//���������޸��ƶ�����͸ı��ȡ���������ļ�
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
	//���뷽���Ӧ֡�����ļ�
	for (int i = 1; i < 3; i++) {
		sprintf(nameSize, openfile, i);
		animation->addSpriteFrameWithFile(nameSize);
	}
	animation->setDelayPerUnit(0.1f);//ÿ֮֡��ʱ����
	animation->setLoops(1);//ѭ������
	animation->setRestoreOriginalFrame(false);//������������ʾ�Ƿ�ص���������ǰ
	auto animate_up = cocos2d::Animate::create(animation);//��������
	auto seq = cocos2d::Spawn::create(moveBy, animate_up, nullptr);//���������ƶ��Ͷ���ͬʱ���е�����
	this->runAction(seq);//��������
}

void player::Is_under_attack(int dir) {//�����յ����������
	if (!invincible) {
		this->hp -= 30;
		stopActionByTag(PLAYER_MOVE_ACTION_TAG);	//ͣ���ƶ��Ķ���
	}
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//�������뷽��ѡ��ֱ���˷���
	switch (dir) {
	case 0:moveby = move_down; break;
	case 1:moveby = move_up; break;
	case 2:moveby = move_right; break;
	case 3:moveby = move_left; break;
	default:
		moveby = move_up;
		break;
	}
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
	char openfile[50] = "UP_02.png";
	switch (dir) {
	case 0:break;
	case 1:openfile[0] = 'D'; openfile[1] = 'O'; break;
	case 2:openfile[0] = 'L'; openfile[1] = 'F'; break;
	case 3:openfile[0] = 'R'; openfile[1] = 'I'; break;
	default:break;
	}
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);

	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	auto animate = cocos2d::Animate::create(animation); //��������
	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);//�ƶ��Ͷ���ͬʱ����
	this->runAction(cocos2d::Sequence::create(seq, nullptr));//��������
}

void player::player_attack() {
	//����ǹ������ʱ	
	auto bullet = Sprite::create("bullet.png");
	auto soundEffectID = cocos2d::AudioEngine::play2d("gameeffect.mp3", false);//������Ч
	bullet->setPosition(this->getContentSize()/2);
	//����physics������ײ
	auto physicsBody = cocos2d::PhysicsBody::createBox(bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	bullet->setPhysicsBody(physicsBody);

	bullet->setTag(BULLET_TAG);//���ñ�ǩ
	this->addChild(bullet);

	cocos2d::Vec2 shootAmount;
	auto tmp_weapon = this->get_weapon_attribute();//��õ�ǰ��������
	//���ݵ�ǰ�����ľ�������ѡ���ӵ���ȷ�������ϵĹ�����Χ
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
		this->decrease_weapon_num(); //�����ӵ���
	}
	this->renew_display_num();//������ʾ
	//�ӵ��Ķ���
	auto realDest = shootAmount + bullet->getPosition();//ʵ���ӵ��켣�յ�
	int normal_speed = 5;//�����ٶ�
	//���������ٶ������趨�ӵ������ٶ�
	auto actionMove = cocos2d::MoveTo::create(0.5f * normal_speed / tmp_weapon.speed, realDest);
	auto actionRemove = cocos2d::RemoveSelf::create();//�ӵ��Ƴ�
	bullet->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));//��������
}


