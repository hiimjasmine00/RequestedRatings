#include "RequestedRatings.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

static std::unordered_map<std::string_view, BoolSettingV3*> settings = [] {
    std::unordered_map<std::string_view, BoolSettingV3*> settings;
    auto msm = ModSettingsManager::from(getMod());
    constexpr std::array keys = {
        "enable-auto",
        "enable-easy",
        "enable-normal",
        "enable-hard",
        "enable-harder",
        "enable-insane",
        "enable-demon",
        "na-override",
    };
    for (auto key : keys) {
        if (auto setting = std::static_pointer_cast<BoolSettingV3>(msm->get(key))) {
            settings.emplace(key, setting.get());
        }
    }
    return settings;
}();

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
        if (auto it = settings.find(key); it != settings.end() && it->second->getValue()) {
            sprite->updateDifficultyFrame(difficulty, GJDifficultyName::Short);
        }
    }
}
