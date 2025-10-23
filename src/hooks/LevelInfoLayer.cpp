#include "../RequestedRatings.hpp"
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(RRLevelInfoLayer, LevelInfoLayer) {
    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();

        RequestedRatings::updateDifficultySprite(m_difficultySprite, m_level);
    }
};
