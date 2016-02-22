#include "SpriteAnimation.h"

using namespace std;
using namespace cocos2d;

ActionInterval *SpriteAnimation::create(const string &prefix, float interval) {
	return RepeatForever::create(createNonLooped(prefix, interval));
}
ActionInterval *SpriteAnimation::createNonLooped(const string &prefix, float interval) {
	auto cache = SpriteFrameCache::sharedSpriteFrameCache();
	auto animation = Animation::create();

	for (int i = 0;; i++) {
		char path[256];
		sprintf(path, "%s_%d.png", prefix.c_str(), i);

		auto frame = cache->getSpriteFrameByName(path);
		if (frame == nullptr && i != 0)
			break;
		if(frame)
			animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(interval);

	return Animate::create(animation);
}