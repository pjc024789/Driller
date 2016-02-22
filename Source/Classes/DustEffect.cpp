#include "DustEffect.h"

#include "EffectSprite/LightEffect.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace cocos2d;

DustEffect *DustEffect::create() {
	auto exp = new DustEffect();

	if (exp && exp->initialize()) {
		exp->autorelease();
		return exp;
	}
	CC_SAFE_RELEASE(exp);
	return nullptr;
}
bool DustEffect::initialize() {
	if (!Sprite::init())
		return false;

	int chunks = rand() % 5 + 10;

	for (int i = 0; i < chunks; i++) {
		char frameName[256];

		sprintf(frameName, "block_sky_dust_%d.png", rand() % 3 + 1);

		auto sprite = Sprite::createWithSpriteFrameName(frameName);
		//auto sprite = Sprite::create("dust.png");
		auto angle = rand() % 360;
		float x = rand() % 220 - 120;//sin(3.14 / 180 * angle) * 50;
		float y = rand() % 220 - 152;//cos(3.14 / 180 * angle) * 50;

		sprite->setScale(2.0);

		sprite->setPosition(0, 0);
		sprite->runAction(
			EaseSineInOut::create(
			MoveBy::create(0.3f, Vec2(x, y))));
		sprite->runAction(
			ScaleBy::create(0.33, 0.6));
		sprite->runAction(
			Sequence::create(
				DelayTime::create(0.2),
				FadeOut::create(0.2),
				RemoveSelf::create(),
				nullptr));
		sprite->setRotation(rand() % 360);

		runAction(
			Sequence::create(
				DelayTime::create(0.4),
				RemoveSelf::create(),
				nullptr));

		addChild(sprite);
	}
	

	return true;
}