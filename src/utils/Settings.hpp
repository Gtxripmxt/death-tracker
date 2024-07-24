#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Settings {
public:
    static int64_t getMaxSessionLength();
    static bool isCompletedLevelTrackingDisabled();
    static bool isPracticeZeroDeathsEnabled();
    static bool getPauseMenuEnabled();
    static bool getLateSaveEnabled();
    static std::filesystem::path getSavePath();
    static bool getSafeModeEnabled();
};