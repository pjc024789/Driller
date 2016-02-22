#include "Block.h"

#include "EffectSprite/LightEffect.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace cocos2d;

Block *Block::create(Type type) {
	auto block = new Block();

	if (block && block->initialize(type)) {
		block->autorelease();
		return block;
	}
	CC_SAFE_RELEASE(block);
	return nullptr;
}
bool Block::initialize(Type type) {
	string frameName;

	switch (type) {
	case Type::None: frameName = "none"; break;
	case Type::Brown: frameName = "brown"; break;
	case Type::Yellow: frameName = "yellow"; break;
	case Type::Ice: frameName = "sky"; break;
	}
	
	if (!Sprite::initWithSpriteFrameName("block_" + frameName + ".png"))
		return false;

	if (type == Type::None) {
		runAction(SpriteAnimation::create("block_none"));
	}

	setEffect(LightEffect::sharedInstance(), "character_n.png");
	setCascadeOpacityEnabled(true);

	this->hp = 2;
	this->type = type;

	return true;
}

Block::Type Block::getType() {
	return type;
}

void Block::explode() {
	runAction(
		ScaleTo::create(0.25, 0));

	if (overlay) {
		overlay->runAction(
			ScaleTo::create(0.25, 0));
	}
}
bool Block::damage() {
	hp--;
	if (hp == 0) {
		explode();
		return true;
	}

	string frameName;

	switch (type) {
	case Type::None: frameName = "none"; break;
	case Type::Brown: frameName = "brown"; break;
	case Type::Yellow: frameName = "yellow"; break;
	case Type::Ice: frameName = "sky"; break;
	}

	overlay = EffectSprite::createWithSpriteFrame(
		SpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName("block_" + frameName + "_2.png"));

	if (overlay) {
		overlay->setPosition(Vec2(32, 32));
		overlay->setOpacity(0);
		overlay->setScale(1.35);
		overlay->runAction(FadeIn::create(0.32));
		overlay->runAction(ScaleTo::create(0.22, 1));
		overlay->setEffect(LightEffect::sharedInstance(), "character_n.png");
		addChild(overlay);
	}
	//setSpriteFrame("block_" + frameName + "_2.png");

	return false;
}