#include "ExplosionEffect.h"

#include "EffectSprite/LightEffect.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace cocos2d;

ExplosionEffect *ExplosionEffect::create(bool blend) {
	auto exp = new ExplosionEffect();

	if (exp && exp->initialize(blend)) {
		exp->autorelease();
		return exp;
	}
	CC_SAFE_RELEASE(exp);
	return nullptr;
}
bool ExplosionEffect::initialize(bool blend) {
	if (!Sprite::init())
		return false;

	int chunks = rand() % 5 + 10;

	for (int i = 0; i < chunks; i++) {
		char frameName[256];

		sprintf(frameName, "block_sky_dust_%d.png", rand() % 3 + 1);

		//auto sprite = Sprite::createWithSpriteFrameName(frameName);
		auto sprite = Sprite::create("dust.png");

		sprite->runAction(
			SpriteAnimation::createNonLooped("dust"));

		auto angle = rand() % 360;
		float x = rand() % 120 - 60;//sin(3.14 / 180 * angle) * 50;
		float y = rand() % 120 - 72;//cos(3.14 / 180 * angle) * 50;

		BlendFunc add;
		add.dst = GL_ONE;
		add.src = GL_SRC_ALPHA;

		sprite->setBlendFunc(add);

		if(blend)
			sprite->setBlendFunc(add);

		sprite->setScale(2.2);
		sprite->setOpacity(64);

		sprite->setPosition(0, 0);
		sprite->runAction(
			EaseSineInOut::create(
			MoveBy::create(0.4f, Vec2(x, y))));
		sprite->runAction(
			ScaleBy::create(0.4, 1.6));
		sprite->runAction(
			Sequence::create(
				DelayTime::create(0.07),
				FadeTo::create(0.2, 156),
				FadeTo::create(0.1, 16),
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