#include "RequestedRatings.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>

using namespace geode::prelude;

bool RequestedRatings::enabled(Mod* mod) {
    return get<"enable-auto", bool>(mod) || get<"enable-easy", bool>(mod) || get<"enable-normal", bool>(mod) || get<"enable-hard", bool>(mod) ||
        get<"enable-harder", bool>(mod) || get<"enable-insane", bool>(mod) || get<"enable-demon", bool>(mod) || get<"na-override", bool>(mod);
}

void RequestedRatings::updateDifficultySprite(GJDifficultySprite* sprite, GJGameLevel* level) {
    if (level->m_stars.value() == 0 && level->m_demon.value() == 0 && level->m_featured == 0 && level->m_isEpic == 0 && level->m_starsRequested > 0) {
        auto difficulty = 0;
        auto update = false;
        auto mod = Mod::get();
        switch (level->m_starsRequested) {
            case 1: // Auto
                difficulty = -1;
                update = get<"enable-auto", bool>(mod);
                break;
            case 2: // Easy
                difficulty = 1;
                update = get<"enable-easy", bool>(mod);
                break;
            case 3: // Normal
                difficulty = 2;
                update = get<"enable-normal", bool>(mod);
                break;
            case 4: case 5: // Hard
                difficulty = 3;
                update = get<"enable-hard", bool>(mod);
                break;
            case 6: case 7: // Harder
                difficulty = 4;
                update = get<"enable-harder", bool>(mod);
                break;
            case 8: case 9: // Insane
                difficulty = 5;
                update = get<"enable-insane", bool>(mod);
                break;
            case 10: // Demon
                difficulty = 6;
                update = get<"enable-demon", bool>(mod);
                break;
            default: // NA
                difficulty = 0;
                update = get<"na-override", bool>(mod);
                break;
        }

        if (update) sprite->updateDifficultyFrame(difficulty, GJDifficultyName::Short);
    } 
}
