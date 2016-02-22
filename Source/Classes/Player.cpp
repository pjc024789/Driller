#include "Player.h"

#include "EffectSprite/LightEffect.h"
#include "SpriteAnimation.h"

#include "HelloWorldScene.h"
#include "Block.h"

#include "DustEffect.h"
#include "ExplosionEffect.h"

using namespace std;
using namespace cocos2d;

Player *Player::create() {
	auto player = new Player();

	if (player && player->initialize()) {
		player->autorelease();
		return player;
	}
	CC_SAFE_RELEASE(player);
	return nullptr;
}
bool Player::initialize() {
	if (!Sprite::initWithSpriteFrameName("idle_0.png"))
		return false;

	setEffect(LightEffect::sharedInstance(), "character_n.png");

	idle();
	setScale(1.5f);

	return true;
}
void Player::idle() {
	runAction(SpriteAnimation::create("idle"))->setTag(1);

	isMovable = true;
}
void Player::drillDown(bool recursive) {
	if (isMovable == false && recursive == false)
		return;

	auto scene = HelloWorld::sharedInstance();
	auto currentBlock = HelloWorld::sharedInstance()->getBlock(getCol(), getRow());

	auto movable = currentBlock->damage();

	stopActionByTag(1);

	double speedFactor = recursive ? 0.2 : 1.0;
	auto downAction = SpriteAnimation::createNonLooped("down", 0.065 * speedFactor);

	if (recursive) {
		runAction(ScaleTo::create(0.1, 2.0));
		runAction(
			Sequence::create(
				RotateTo::create(0.08, -12),
				RotateTo::create(0.08, 12),
				RotateTo::create(0.08, 0),
				nullptr));

		if (movable) {
			combo++;

			auto effect = Sprite::create();
			effect->runAction(
				Sequence::create(
					SpriteAnimation::createNonLooped("pang", 0.04),
					RemoveSelf::create(),
					nullptr));
			effect->runAction(FadeTo::create(0.3, 32));
			effect->setOpacity(128);
			effect->setPosition(getPosition());
			scene->addChild(effect, 100);

			if (prevCombo)
			{
				prevCombo->stopAllActions();
				prevCombo->setOpacity(32);
				prevCombo->runAction(
					Sequence::create(
						ScaleTo::create(0.2, 3),
						RemoveSelf::create(),
						nullptr));
			}

			Device::vibrate(0.09);

			char path[256];

			sprintf(path, "combo_%d.png", combo + 1);

			auto comboText = Sprite::createWithSpriteFrameName("combo.png");
			auto comboCount = Sprite::createWithSpriteFrameName(path);

			comboCount->setPosition(Vec2(comboText->getContentSize().width/2, 60));

			comboText->addChild(comboCount, 1);
			comboText->setCascadeOpacityEnabled(true);
			comboText->setPosition(getPosition() + Vec2(0, 100));
			comboText->setScale(0);
			comboText->runAction(
				EaseElasticOut::create(
					ScaleTo::create(0.45, 1.0 + combo * 0.1)));
			comboText->runAction(
				Sequence::create(
					DelayTime::create(0.8),
					FadeOut::create(0.3),
					nullptr));
			scene->addChild(comboText, 100);

			prevCombo = comboText;
		}
	}

	runAction(
		Sequence::create(
			downAction,
			CallFunc::create([this, currentBlock, recursive]() {
				auto nextBlock = HelloWorld::sharedInstance()->getBlock(getCol(), getRow());

				// 아래가 타입 같은 블럭이면 한번 더팜
				if ((recursive || nextBlock != currentBlock) &&
					nextBlock->getType() == currentBlock->getType())
					drillDown(true);
				else {
					if (recursive) {
						runAction(ScaleTo::create(0.23, 1.5));
					}

					combo = 0;
					idle();
				}
			}),
			nullptr)
		)->setTag(1);
	
	runAction(
		EaseBounceOut::create(
			Sequence::create(
				MoveBy::create(0.35 * speedFactor, Vec2(0, -20)),
				MoveBy::create(0.28 * speedFactor, Vec2(0, 20)),
				nullptr)));
		
	auto vibe = rand() % 20 - 10 + 5;
	

	if (movable == false) {
		scene->runAction(
			Sequence::create(
				ScaleTo::create(0.2 * speedFactor, 1.03),
				ScaleTo::create(0.25 * speedFactor, 1.00),
				nullptr));
	}

	scene->runAction(
		Sequence::create(
			MoveBy::create(0.05 * speedFactor, Vec2(vibe*(!movable), 25)),
			MoveBy::create(0.10 * speedFactor, Vec2(-vibe*(!movable), -25)),
			MoveBy::create(0.05 * speedFactor, Vec2(vibe*(!movable), 25)),
			MoveBy::create(0.10 * speedFactor, Vec2(-vibe*(!movable), -25)),
			nullptr
			));
	runAction(
		Sequence::create(
			CallFunc::create([speedFactor, scene, this]() {
			
			Device::vibrate(0.05 * speedFactor);

			auto dust = DustEffect::create();
			dust->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(dust, 100);
			auto exp = ExplosionEffect::create(false);
			exp->setPosition(getPosition() + Vec2(0, -30));
			exp->setScale(1.2);
			scene->addChild(exp, 100);
		}),
				DelayTime::create(0.1 * speedFactor),
			CallFunc::create([speedFactor, scene, this]() {

			Device::vibrate(0.07 * speedFactor);

			auto dust = DustEffect::create();
			dust->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(dust, 100);
			auto exp = ExplosionEffect::create();
			exp->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(exp, 100);
		}),
			DelayTime::create(0.1 * speedFactor),
			CallFunc::create([speedFactor, scene, this]() {

			Device::vibrate(0.05 * speedFactor);

			auto dust = DustEffect::create();
			dust->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(dust, 100);
			auto exp = ExplosionEffect::create();
			exp->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(exp, 100);
		}),
			DelayTime::create(0.04 * speedFactor),
			CallFunc::create([speedFactor, scene, this]() {

			Device::vibrate(0.02 * speedFactor);

			auto exp = ExplosionEffect::create();
			exp->setPosition(getPosition() + Vec2(0, -30));
			scene->addChild(exp, 100);
		}),
			nullptr
			));

	isMovable = false;
	

	if (movable) {
		depth += 1;
		moveCamera();
	}
		
}
void Player::moveCamera() {
	auto scene = HelloWorld::sharedInstance();

	scene->parallax->runAction(
		MoveBy::create(0.4, Vec2(0, 64)));
}
int Player::getCol() {
	return 2;
}
int Player::getRow() {
	return depth + 1;
}
int Player::getComboCount() {
	return combo;
}