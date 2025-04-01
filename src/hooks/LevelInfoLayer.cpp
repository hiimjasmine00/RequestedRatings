#include "../RequestedRatings.hpp"
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(RRLevelInfoLayer, LevelInfoLayer) {
    static void onModify(ModifyBase<ModifyDerive<RRLevelInfoLayer, LevelInfoLayer>>& self) {
        auto mod = Mod::get();
        auto enabled = RequestedRatings::enabled(mod);

        auto initHook = self.getHook("LevelInfoLayer::init").map([enabled](Hook* hook) {
            return hook->setAutoEnable(enabled), hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get LevelInfoLayer::init hook: {}", err), err;
        }).unwrapOr(nullptr);
        
        auto levelDownloadFinishedHook = self.getHook("LevelInfoLayer::levelDownloadFinished").map([enabled](Hook* hook) {
            return hook->setAutoEnable(enabled), hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get LevelInfoLayer::levelDownloadFinished hook: {}", err), err;
        }).unwrapOr(nullptr);

        auto likedItemHook = self.getHook("LevelInfoLayer::likedItem").map([enabled](Hook* hook) {
            return hook->setAutoEnable(enabled), hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get LevelInfoLayer::likedItem hook: {}", err), err;
        }).unwrapOr(nullptr);
        
        listenForAllSettingChangesV3([initHook, levelDownloadFinishedHook, likedItemHook](std::shared_ptr<SettingV3>) {
            if (initHook) (void)(RequestedRatings::enabled() ? initHook->enable().mapErr([](const std::string& err) {
                return log::error("Failed to enable LevelInfoLayer::init hook: {}", err), err;
            }) : initHook->disable().mapErr([](const std::string& err) {
                return log::error("Failed to disable LevelInfoLayer::init hook: {}", err), err;
            }));
            if (levelDownloadFinishedHook) (void)(RequestedRatings::enabled() ? levelDownloadFinishedHook->enable().mapErr([](const std::string& err) {
                return log::error("Failed to enable LevelInfoLayer::levelDownloadFinished hook: {}", err), err;
            }) : levelDownloadFinishedHook->disable().mapErr([](const std::string& err) {
                return log::error("Failed to disable LevelInfoLayer::levelDownloadFinished hook: {}", err), err;
            }));
            if (likedItemHook) (void)(RequestedRatings::enabled() ? likedItemHook->enable().mapErr([](const std::string& err) {
                return log::error("Failed to enable LevelInfoLayer::likedItem hook: {}", err), err;
            }) : likedItemHook->disable().mapErr([](const std::string& err) {
                return log::error("Failed to disable LevelInfoLayer::likedItem hook: {}", err), err;
            }));
        }, mod);
    }

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        RequestedRatings::updateDifficultySprite(m_difficultySprite, level);

        return true;
    }

    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        RequestedRatings::updateDifficultySprite(m_difficultySprite, level);
    }

    void likedItem(LikeItemType type, int id, bool liked) {
        LevelInfoLayer::likedItem(type, id, liked);

        RequestedRatings::updateDifficultySprite(m_difficultySprite, m_level);
    }
};

#ifdef GEODE_IS_MACOS // Stupid likedItem inline expansion hook
static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update this hook for the current GD version");

#define LevelInfoLayer_likedItem_432 base::get() + GEODE_ARM_MAC(0x256f58) GEODE_INTEL_MAC(0x2b1040)
void LevelInfoLayer_likedItem(void* self, LikeItemType type, int id, bool liked) {
    reinterpret_cast<void(*)(void*, LikeItemType, int, bool)>(LevelInfoLayer_likedItem_432)(self, type, id, liked);
    auto infoLayer = reinterpret_cast<LevelInfoLayer*>((reinterpret_cast<uintptr_t>(self) - 0x1b0));
    RequestedRatings::updateDifficultySprite(infoLayer->m_difficultySprite, infoLayer->m_level);
}

$execute {
    auto likedItemHook = Hook::create(
        reinterpret_cast<void*>(LevelInfoLayer_likedItem_432),
        &LevelInfoLayer_likedItem,
        "LevelInfoLayer::likedItem (+0x1b0)",
        tulip::hook::TulipConvention::Thiscall
    );
    likedItemHook->setAutoEnable(RequestedRatings::enabled());

    auto mod = Mod::get();
    (void)mod->claimHook(std::move(likedItemHook)).map([mod](Hook* hook) {
        listenForAllSettingChangesV3([hook](std::shared_ptr<SettingV3>) {
            (void)(RequestedRatings::enabled() ? hook->enable().mapErr([](const std::string& err) {
                return log::error("Failed to enable LevelInfoLayer::likedItem hook: {}", err), err;
            }) : hook->disable().mapErr([](const std::string& err) {
                return log::error("Failed to disable LevelInfoLayer::likedItem hook: {}", err), err;
            }));
        }, mod);
        return hook;
    }).mapErr([](const std::string& err) {
        return log::error("Failed to claim LevelInfoLayer::likedItem hook: {}", err), err;
    });
};
#endif
