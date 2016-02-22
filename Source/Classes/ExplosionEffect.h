#include "cocos2d.h"

#include "EffectSprite/EffectSprite.h"

class ExplosionEffect : public EffectSprite {
public:
	static ExplosionEffect *create(bool blend = true);

	void idle();
	void drillDown();

private:
	bool initialize(bool blend = true);
};