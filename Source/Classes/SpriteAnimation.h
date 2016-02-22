#include "cocos2d.h"

class SpriteAnimation {
public:

	static cocos2d::ActionInterval *create(const std::string &prefix, float interval=0.10);
	static cocos2d::ActionInterval *createNonLooped(const std::string &prefix, float interval=0.10);
};