#include <Geode/modify/PlayLayer.hpp>
#include "../managers/StatsManager.hpp"
#include "../utils/Settings.hpp"
#include "../managers/DTPopupManager.hpp"

using namespace geode::prelude;

class $modify(DTPlayLayer, PlayLayer) {
    struct Fields {
        Session* prevSession = nullptr;
        bool isSessionExpired = false;
        bool hasRespawned = false;
        Run currentRun;

        std::vector<int> fzeroToSave{};
        std::vector<Run> runsToSave{};
    };

    bool disableCompletedLevelTracking();

    void updateSessionLastPlayed();

    /* hooks
    ========== */

    bool init(GJGameLevel* level, bool p1, bool p2);

    void resetLevel();

    void destroyPlayer(PlayerObject* player, GameObject* p1);

    void levelComplete();

    void removeAllCheckpoints();

    #if defined(GEODE_IS_MACOS)

    void checkpointActivated(CheckpointGameObject* checkpt);
    
    #endif

    void onQuit();
};
