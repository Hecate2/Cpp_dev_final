#include "StartScene.h"
#include "EndScene.h"
#include "MainScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* End::scene()
{
	return End::create();
}


// on "init" you need to initialize your instance
bool End::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);

	auto bg = Sprite::create("end.png");  //±³¾°Í¼
	if (bg)
	{
		bg->setPosition(Vec2(origin.x + CENTER_X, origin.y + CENTER_Y));

		bg->setScale(1.6f);
		this->addChild(bg);
	}

	highest_score_label = Label::createWithTTF("Highest Score: 0000", "fonts/score.ttf", 24);
	highest_score_label->setPosition(Vec2(winSize.width * 0.5 - 60, winSize.height * 0.9));
	this->addChild(highest_score_label);
	highest_score_label->setGlobalZOrder(1);
	highest_score_label->setString("Highest Score: " + std::to_string(MainScene::read_highest_score_from_file()));

	auto beginbutton = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(End::menuCloseCallback, this));

	float x = origin.x + winSize.width / 2 - beginbutton->getContentSize().width / 2 + 5;
	float y = origin.y + winSize.height / 2 + beginbutton->getContentSize().height / 2 - 40;
	beginbutton->setPosition(Vec2(x, y));

	auto to_start_scene_button = MenuItemImage::create(
		"bang.png",
		"bang.png",
		CC_CALLBACK_1(End::toStartScene, this));

	float x_to_start_scene_button = origin.x + winSize.width / 2 - to_start_scene_button->getContentSize().width / 2 - 415;
	float y_to_start_scene_button = origin.y + winSize.height / 2 + to_start_scene_button->getContentSize().height / 2 + 297;
	to_start_scene_button->setPosition(Vec2(x_to_start_scene_button, y_to_start_scene_button));

	auto menu = Menu::create(beginbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	auto to_start_scene_button_menu = Menu::create(to_start_scene_button, NULL);
	to_start_scene_button_menu->setPosition(Vec2::ZERO);
	this->addChild(to_start_scene_button_menu, 1);

	return true;
}


void End::menuCloseCallback(Ref* sender)
{
	auto soundEffectID = AudioEngine::play2d("sound/enter_game.mp3", false);
	Director::getInstance()->replaceScene(TransitionSlideInT::create(2.0f, MainScene::scene()));
}

void End::toStartScene(Ref* sender) {
	auto soundEffectID = AudioEngine::play2d("sound/enter_game.mp3", false);
	Director::getInstance()->replaceScene(TransitionSlideInT::create(2.0f, Start::scene()));
}