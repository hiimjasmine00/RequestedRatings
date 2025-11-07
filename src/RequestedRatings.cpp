#include "RequestedRatings.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <jasmine/setting.hpp>

using namespace geode::prelude;

void RequestedRatings::updateDifficultySprite(GJDifficultySprite* sprite, GJGameLevel* level) {
    auto starsRequested = level->m_starsRequested;
    if (level->m_stars.value() == 0 && level->m_demon.value() == 0 && level->m_featured == 0 && level->m_isEpic == 0 && starsRequested > 0) {
        constexpr std::array difficulties = {
            std::make_pair(0, "na-override"),
            std::make_pair(-1, "enable-auto"),
            std::make_pair(1, "enable-easy"),
            std::make_pair(2, "enable-normal"),
            std::make_pair(3, "enable-hard"),
            std::make_pair(3, "enable-hard"),
            std::make_pair(4, "enable-harder"),
            std::make_pair(4, "enable-harder"),
            std::make_pair(5, "enable-insane"),
            std::make_pair(5, "enable-insane"),
            std::make_pair(6, "enable-demon")
        };

        auto [difficulty, key] = difficulties[starsRequested < difficulties.size() ? starsRequested : 0];
        if (jasmine::setting::getValue<bool>(key)) {
            sprite->updateDifficultyFrame(difficulty, GJDifficultyName::Short);
        }
    }
}
