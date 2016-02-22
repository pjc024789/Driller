#include "IntroLayer.h"

#include "EffectSprite/LightEffect.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace cocos2d;

IntroLayer *IntroLayer::create() {
	auto layer = new IntroLayer();

	if (layer && layer->initialize()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_RELEASE(layer);
	return nullptr;
}
bool IntroLayer::initialize() {
	if (!Sprite::init())
		return false;

	setContentSize(Size(320, 640));
	setCascadeOpacityEnabled(true);

	auto black = LayerColor::create(Color4B::GRAY);
	black->setOpacity(0);
	black->runAction(FadeTo::create(1.0, 96));
	addChild(black);

	auto title = Sprite::create("title.png");
	title->setPosition(160, 500);
	title->setScale(0);
	title->setOpacity(0);
	title->runAction(
		FadeIn::create(1.0));
	title->runAction(
		EaseElasticOut::create(ScaleTo::create(1.0, 1.0)));
	addChild(title);

	auto press = Sprite::create("press_start.png");
	press->setPosition(160, 430);
	press->setOpacity(0);
	press->runAction(
		FadeIn::create(2.0));
	press->runAction(
		RepeatForever::create(
			Sequence::create(
				MoveBy::create(0.05, Vec2(-2, -1)),
				MoveBy::create(0.07, Vec2(2, 1)),
				nullptr)));
	addChild(press);


	auto listerner = EventListenerTouchAllAtOnce::create();
	listerner->onTouchesBegan = [this](const std::vector<Touch*> &touches, cocos2d::Event *) mutable {
		dismiss();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listerner, this);

	return true;
}

void IntroLayer::dismiss() {
	runAction(
		Sequence::create(
			FadeTo::create(0.15, 0),
			RemoveSelf::create(),
			nullptr));
	runAction(
		ScaleTo::create(0.25, 3));
}