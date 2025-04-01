#include "../RequestedRatings.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(RRLevelCell, LevelCell) {
    static void onModify(ModifyBase<ModifyDerive<RRLevelCell, LevelCell>>& self) {
        auto mod = Mod::get();

        auto initHook = self.getHook("LevelCell::loadFromLevel").map([mod](Hook* hook) {
            return hook->setAutoEnable(RequestedRatings::enabled(mod)), hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get LevelCell::loadFromLevel hook: {}", err), err;
        }).unwrapOr(nullptr);

        listenForAllSettingChangesV3([initHook](std::shared_ptr<SettingV3>) {
            if (initHook) (void)(RequestedRatings::enabled() ? initHook->enable().mapErr([](const std::string& err) {
                return log::error("Failed to enable LevelCell::loadFromLevel hook: {}", err), err;
            }) : initHook->disable().mapErr([](const std::string& err) {
                return log::error("Failed to disable LevelCell::loadFromLevel hook: {}", err), err;
            }));
        }, mod);
    }

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        if (auto difficultyContainer = m_mainLayer->getChildByID("difficulty-container"))
            RequestedRatings::updateDifficultySprite(static_cast<GJDifficultySprite*>(difficultyContainer->getChildByID("difficulty-sprite")), level);
    }
};
