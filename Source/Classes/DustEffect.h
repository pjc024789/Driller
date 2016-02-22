#include "cocos2d.h"

#include "EffectSprite/EffectSprite.h"

class DustEffect : public EffectSprite {
public:
	static DustEffect *create();

	void idle();
	void drillDown();

private:
	bool initialize();
};