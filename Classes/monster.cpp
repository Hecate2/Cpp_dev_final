#include "cocos2d.h"
#include "monster.h"
#include <cmath>
#define FIREBALL_TAG 11
#define MOVE_ACTION_TAG 1
#define SMALL_MOVE_ACTION_TAG 2

int getdirection(cocos2d::Vec2 diff) {
	//0->�Ϸ�  1->��  2->��   3->��
	if (diff.y <= 0 && -diff.y >= fabs(diff.x)) {
		//�����ƶ�
		return  1;
	}
	else if (diff.y >= 0 && diff.y >= fabs(diff.x)) {
		//�����ƶ�
		return 0;
	}
	else if (diff.x >= 0 && diff.x >= fabs(diff.y)) {
		//�����ƶ�
		return 3;
	}
	else {
		//�����ƶ�
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
	//0->��  1->��  2->��   3->��
	//�������
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
	//�ƶ�
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(3);                  //-1����ѭ��
	//��������
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	seq->setTag(MOVE_ACTION_TAG);
	this->runAction(seq);
}

void BigMonster::attack(cocos2d::Vec2 diff) {
	//��ʬ���򹥻��ķ���
	int dir = getdirection(diff);
	stopActionByTag(MOVE_ACTION_TAG);	//ͣ���ƶ��Ķ���
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	//��������
	auto animate = cocos2d::Animate::create(animation);
	auto seq = cocos2d::Spawn::create(animate, nullptr);
	this->runAction(seq);
	
	if (!is_under_attack) {	//������������н�ֱ���޷�����
		//��������
		auto fireball = Sprite::create("fireball.png");
		fireball->setPosition(cocos2d::Vec2(75, 75));
		auto physicsBody = cocos2d::PhysicsBody::createCircle(20.0f, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		fireball->setPhysicsBody(physicsBody);
		fireball->setTag(FIREBALL_TAG);
		//��ӵ�������
		this->addChild(fireball);
		// ��ƫ����ת��Ϊ��λ������������Ϊ1��������
		diff.normalize();
		// �������2000����ͻ����һ��ָ���û���������ĳ���Ϊ2000��������Ϊʲô��2000�أ���Ϊ����Ӧ�����Գ�����ǰ�ֱ�������Ļ�ı߽硣
		auto shootAmount = diff * 2000;
		// 5.����һ����������12�����ƶ���Ŀ��λ�ã�Ȼ�����ӳ������Ƴ���
		auto actionMove = cocos2d::MoveBy::create(12.0f, shootAmount);
		auto actionRemove = cocos2d::RemoveSelf::create();
		fireball->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));
	}
}

void BigMonster::under_attack(cocos2d::Vec2 diff,int de_hp){
	is_under_attack = true;
	this->hp -= de_hp;
	stopActionByTag(MOVE_ACTION_TAG);	//ͣ���ƶ��Ķ���
	//���ˣ��򱻹����ķ�����
	int dir = getdirection(diff);
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//�ƶ�
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	//��������
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby,animate, nullptr);
	//auto delay = cocos2d::DelayTime::create(2);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
	is_under_attack = false;
}

void BigMonster::death() {
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
	char openfile[16] = "BOSS_d.png";
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	//��������
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
	//0->��  1->��  2->��   3->��
	//�������
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
	//�ƶ�
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(3);                  //-1����ѭ��
	//��������
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	seq->setTag(SMALL_MOVE_ACTION_TAG);
	this->runAction(seq);
}

void SmallMonster::attack(cocos2d::Vec2 diff){
	//��ʬ���򹥻��ķ���
	int dir = getdirection(diff);
	stopActionByTag(SMALL_MOVE_ACTION_TAG);	//ͣ���ƶ��Ķ���
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(1);                  //
	//��������
	auto animate = cocos2d::Animate::create(animation);
	auto seq = cocos2d::Spawn::create(animate, nullptr);
	this->runAction(seq);

	if (!is_under_attack) {	//������������н�ֱ���޷�����
		//�����հ���
		auto fireball = Sprite::create("blank.png");
		fireball->setPosition(cocos2d::Vec2(75, 75));
		auto physicsBody = cocos2d::PhysicsBody::createCircle(20.0f, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		fireball->setPhysicsBody(physicsBody);
		fireball->setTag(FIREBALL_TAG);
		//��ӵ�������
		this->addChild(fireball);
		// ��ƫ����ת��Ϊ��λ������������Ϊ1��������
		diff.normalize();
		// �������2000����ͻ����һ��ָ���û���������ĳ���Ϊ2000��������Ϊʲô��2000�أ���Ϊ����Ӧ�����Գ�����ǰ�ֱ�������Ļ�ı߽硣
		auto shootAmount = diff * 75;
		// 5.����һ����������2�����ƶ���Ŀ��λ�ã�Ȼ�����ӳ������Ƴ���
		auto actionMove = cocos2d::MoveBy::create(1.0f, shootAmount);
		auto actionRemove = cocos2d::RemoveSelf::create();
		fireball->runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));
	}
}

void SmallMonster::under_attack(cocos2d::Vec2 diff,int de_hp){
	is_under_attack = true;
	this->hp -= de_hp;
	stopActionByTag(SMALL_MOVE_ACTION_TAG);	//ͣ���ƶ��Ķ���
	//���ˣ��򱻹����ķ�����
	int dir = getdirection(diff);
	int distance = 20;
	float time = 0.3;
	auto move_down = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, -distance));
	auto move_up = cocos2d::MoveBy::create(time, cocos2d::Vec2(0, distance));
	auto move_right = cocos2d::MoveBy::create(time, cocos2d::Vec2(distance, 0));
	auto move_left = cocos2d::MoveBy::create(time, cocos2d::Vec2(-distance, 0));
	auto moveby = move_up;
	//�ƶ�
	switch (dir) {
	case 0:moveby = move_up; break;
	case 1:moveby = move_down; break;
	case 2:moveby = move_left; break;
	case 3:moveby = move_right; break;
	default:
		moveby = move_up;
		break;
	}
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
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
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(0.5);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	//��������
	auto animate = cocos2d::Animate::create(animation);

	auto seq = cocos2d::Spawn::create(moveby, animate, nullptr);
	//auto delay = cocos2d::DelayTime::create(2);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
	is_under_attack = false;
}

void SmallMonster::death(){
	//��Ӧ���ƶ�����
	auto animation = cocos2d::Animation::create();
	//���ͼƬ��Դ
	char openfile[16] = "Small_d.png";
	animation->cocos2d::Animation::addSpriteFrameWithFile(openfile);
	//��������
	animation->setRestoreOriginalFrame(false); //��ԭ��һ֡
	animation->setDelayPerUnit(1);     //��λ֡���
	animation->setLoops(0);                  //��ֹ��ͼƬ
	//��������
	auto animate = cocos2d::Animate::create(animation);
	auto fade = cocos2d::FadeTo::create(2, 0);
	auto seq = cocos2d::Spawn::create(fade, animate, nullptr);
	this->runAction(cocos2d::Sequence::create(seq, nullptr));
}
