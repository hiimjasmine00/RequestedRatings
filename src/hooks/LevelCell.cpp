#include "../RequestedRatings.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(RRLevelCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        if (auto container = m_mainLayer->getChildByID("difficulty-container")) {
            RequestedRatings::updateDifficultySprite(static_cast<GJDifficultySprite*>(container->getChildByID("difficulty-sprite")), level);
        }
    }
};
