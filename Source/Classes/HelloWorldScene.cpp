#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "EffectSprite/LightEffect.h"
#include "EffectSprite/EffectSprite.h"

#include "IntroLayer.h"
#include "Block.h"
#include "Player.h"
#include "ExplosionEffect.h"
#include "DustEffect.h"
#include "SpriteAnimation.h"

USING_NS_CC;

using namespace cocostudio::timeline;

HelloWorld *HelloWorld::instance = nullptr;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld *HelloWorld::sharedInstance() {
	return instance;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	instance = this;

	scheduleOnce([this](float dt) {
		auto intro = IntroLayer::create();
		intro->setPosition(160, 320);
		addChild(intro, 9999);
	}, 3.4, "show_intro");

	auto background = LayerColor::create(Color4B(140,140,140,255));
	//background->setContentSize(Size(640, 1280));
	background->setScale(2);
	background->setCascadeColorEnabled(true);
	addChild(background, -2);
	parallax = Layer::create();

	auto cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("character.plist");
	cache->addSpriteFramesWithFile("3pang.plist");
	cache->addSpriteFramesWithFile("pang.plist");

	auto sprite = EffectSprite::createWithSpriteFrame(cache->getSpriteFrameByName("idle_0.png"));

	sprite->setPosition(400, 200);

	addChild(sprite);

	light = LightEffect::create();
	light->retain();

	Vec3 lightPos(200, 200, 200);
	light->setLightPos(lightPos);
	light->setLightCutoffRadius(150);
	light->setLightHalfRadius(0.41f);
	light->setBrightness(6.0);

	sprite->setEffect(light, "character_n.png");

	auto listerner = EventListenerTouchAllAtOnce::create();
	listerner->onTouchesBegan = [background, &lightPos, this](const std::vector<Touch*> &touches, cocos2d::Event *) mutable {
		player->drillDown();

		unschedule("key");

		background->runAction(
			Sequence::create(
				TintTo::create(0.2, Color3B(20,20,15)),
				TintTo::create(0.1, Color3B(30,30,30)),
				nullptr
				));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listerner, this);
	
	buildBlocks();

	player = Player::create();
	player->setPosition(64*3 - 32, 64 * 5 + 32 + 1000);
	player->runAction(
		Sequence::create(
			DelayTime::create(2.0),
			MoveBy::create(1.0, Vec2(0, -1000)),
			nullptr));
	
	setScale(0);
	setOpacity(0);
	setCascadeOpacityEnabled(true);
	runAction(
		FadeIn::create(0.6));
	runAction(
		ScaleTo::create(0.6, 1));
	runAction(RotateTo::create(0.6, 360));

	/*
	schedule([this](float dt) mutable {
		
	}, 0.1f);
	*/

	parallax->setPosition(0, 0);
	addChild(parallax);
	addChild(player, 10);

	scheduleUpdate();

	schedule([background, this](float dt) {
		for (int i = 0; i < 5; i++) {
			auto arrow = Sprite::create("arrow.jpg");
			auto scale = rand() % 70 * 0.01 + 0.3;

			scale *= 0.25;

			arrow->setPosition(Vec2(i*48 + 16, 640 + 128));
			arrow->setScale(scale);
			arrow->runAction(
				Sequence::create(
					MoveBy::create(4, Vec2(0, -640)),
					RemoveSelf::create(),
					nullptr
					));
			arrow->runAction(
				RepeatForever::create(
					Sequence::create(
						EaseBounceOut::create(ScaleBy::create(0.2, 1.4)),
						ScaleTo::create(0.1, scale),
						DelayTime::create(0.5),
						nullptr
						)));

			background->addChild(arrow);
		}
		

	}, 0.35, "key");

    return true;
}

void HelloWorld::update(float dt) {
	auto pos = player->getPosition();

	light->setLightPos(Vec3(pos.x, pos.y, sin(clock() * 0.000001) * 80 - 40));

	int combo = player->getComboCount();;
	light->setBrightness(4.0 + combo * 1.8);
	light->setLightCutoffRadius(150 + combo * 1);
	light->setLightHalfRadius(0.41 + combo * 0.01);
	light->setAmbientLightColor(Color3B(127, 127 - combo * 2, 127 - combo * 2));
}

void HelloWorld::buildBlocks() {
	auto cache = SpriteFrameCache::sharedSpriteFrameCache();

	int maxDepth = 100;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < maxDepth; j++) {
			auto types = 
				(j >= maxDepth - 30 && j <= maxDepth - 24) ? 1 : 4;

			auto block = Block::create((Block::Type)(rand() % types));
			auto pos = Vec2(i * 64 + 32, j * 64 + 32 - ((maxDepth-5) * 64));
			auto aniDepth = j - maxDepth + 10;

			block->setPosition(pos);

			blocks[i][j] = block;

			if (j >= maxDepth - 10) {
				block->setRotation(rand() % 360);
				block->setOpacity(0);
				block->setPosition(i * 64 + 32, 1000);
				block->setScale(0);
				block->runAction(
					Sequence::create(
						DelayTime::create((i % rand()%4) * 0.3 + (aniDepth * 0.1)),
						MoveTo::create(0.5 + (aniDepth * 0.05), pos),
						nullptr));
				
				block->runAction(
					Sequence::create(
						DelayTime::create((i % rand() % 4) * 0.3 + (aniDepth * 0.15)),
						EaseElasticOut::create(ScaleTo::create(1.5, 1.0)),
						nullptr));
				block->runAction(
					Sequence::create(
						DelayTime::create((i % rand() % 4) * 0.3 + (aniDepth * 0.1)),
						RotateTo::create(0.7, 720*1.5),
						nullptr));
				block->runAction(
					FadeIn::create(3.0));
			}

			parallax->addChild(block);
		}
	}
}


Block *HelloWorld::getBlock(int x, int y) {
	return blocks[x][100-y];
}