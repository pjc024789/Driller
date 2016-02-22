#include "cocos2d.h"

#include "EffectSprite/EffectSprite.h"

class Block : public EffectSprite {
public:
	enum Type {
		None=0, Ice, Brown, Yellow
	};

public:
	static Block *create(Type type);

	void explode();
	bool damage();

	Type getType();

private:
	bool initialize(Type type);

private:
	Type type;
	int hp;

	EffectSprite *overlay;
};