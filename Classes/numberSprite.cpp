#include "numberSprite.h"

numberSprite::numberSprite() {
	inBoard = false;
	num = -1;
    currentColumn = currentRow = -1;
}

numberSprite::~numberSprite() {}

numberSprite* numberSprite::create(const string &filename) {
    numberSprite* pSprite = new numberSprite();

    if (pSprite->initWithFile(filename)) {
        pSprite->autorelease();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return nullptr;
}