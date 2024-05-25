#include "../layers/DTManageLevelsLayer.hpp"
#include "../layers/LinkLevelCell.hpp"

DTManageLevelsLayer* DTManageLevelsLayer::create(DTLayer* const& layer) {
    auto ret = new DTManageLevelsLayer();
    if (ret && ret->init(380, 280, layer, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DTManageLevelsLayer::setup(DTLayer* const& layer) {
    //create trackerLayer
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_DTLayer = layer;

    auto overallInfoBS = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    overallInfoBS->setScale(0.8f);
    auto overallInfoButton = CCMenuItemSpriteExtra::create(
        overallInfoBS,
        nullptr,
        this,
        menu_selector(DTManageLevelsLayer::onOverallInfo)
    );
    overallInfoButton->setPosition(m_size.width / 2 - 8.5f, m_size.height / 2 - 8.5f);
    this->m_buttonMenu->addChild(overallInfoButton);

    alighmentNode = CCNode::create();
    alighmentNode->setPosition(m_buttonMenu->getPosition());
    m_mainLayer->addChild(alighmentNode);

    m_AllLevels = StatsManager::getAllLevels();

    seartchInput = InputNode::create(225, "Search");
    seartchInput->getInput()->setDelegate(this);
    seartchInput->setPosition({0, 115});
    seartchInput->setScale(0.6f);
    alighmentNode->addChild(seartchInput);

    #ifdef GEODE_IS_MACOS
    #else
        auto downloadBS = cocos2d::CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        downloadBS->setScale(.8f);
        auto downloadButton = CCMenuItemSpriteExtra::create(
            downloadBS,
            nullptr,
            this,
            menu_selector(DTManageLevelsLayer::onDownload)
        );
        downloadButton->setPosition(-m_size.width / 2 + 3.f, -m_size.height / 2 + 3.f);
        m_buttonMenu->addChild(downloadButton);
    #endif
    refreshLists(false);

    scheduleUpdate();

    return true;
}

void DTManageLevelsLayer::onDownload(CCObject*){
    if (dInfo) return;
    dInfo = true;

    std::vector<int> levelIDs;

    for (int i = 0; i < m_AllLevels.size(); i++)
    {
        auto splittedLevelKey = StatsManager::splitLevelKey(m_AllLevels[i].first);
        if (splittedLevelKey.second == "online"){
            int id = std::stoi(splittedLevelKey.first);

            levelIDs.push_back(id);
        }
    }

    auto list = GJLevelList::create();
	list->m_listName = "";
	list->m_levels = levelIDs;

    #ifdef GEODE_IS_MACOS
    #else
        m_LoadLevels = LevelListLayer::create(list);
    #endif
    this->addChild(m_LoadLevels);

    CCObject* child;

    CCARRAY_FOREACH(m_LoadLevels->getChildren(), child){
        auto loadingC = dynamic_cast<LoadingCircle*>(child);
        if (!loadingC)
            static_cast<CCNode*>(child)->setVisible(false);
        else
            m_LoadLevelsCircle2 = loadingC;
    }
}

void DTManageLevelsLayer::update(float delta){

    if (dInfo && m_LoadLevelsCircle2){
        if (dProg == 0 && m_LoadLevelsCircle2->isVisible()){
            dProg = 1;
        }
        if (dProg == 1 && !m_LoadLevelsCircle2->isVisible()){
            dProg = 2;
        }
        else if (dProg == 2){
            dProg = 0;
            CCObject* child;

            CCARRAY_FOREACH(m_LoadLevels->m_list->m_listView->m_tableView->m_cellArray, child){
                auto level = static_cast<LevelCell*>(child)->m_level;
                if (ghc::filesystem::exists(StatsManager::getLevelSaveFilePath(level))){
                    auto stats = StatsManager::getLevelStats(level);

                    if (stats.currentBest != -1){
                        stats.attempts = level->m_attempts;
                        stats.levelName = level->m_levelName;
                        stats.difficulty = StatsManager::getDifficulty(level);

                        StatsManager::saveData(stats, level);
                    }
                }
            }

            dInfo = false;

            m_AllLevels = StatsManager::getAllLevels();
            refreshLists(true);

            m_LoadLevels->removeMeAndCleanup();
            m_LoadLevels = nullptr;
            m_LoadLevelsCircle2 = nullptr;
        }
    }
}

void DTManageLevelsLayer::textChanged(CCTextInputNode* input){
    if (seartchInput->getInput() == input){
        std::string filterText = "";
        if (input->getString() != "")
            filterText = input->getString();
        
        m_filterText = filterText;
        refreshLists(false);
    }
}

void DTManageLevelsLayer::refreshLists(bool SavePos){
    int prevY = 1;
    if (m_LevelsList){
        prevY = m_LevelsList->m_listView->m_tableView->m_contentLayer->getPositionY();
        m_LevelsList->removeMeAndCleanup();
    }

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto myKey = StatsManager::getLevelKey(m_DTLayer->m_Level);
    auto levelsListItems = CCArray::create();

    for (int i = 0; i < m_AllLevels.size(); i++)
    {
        std::string filterTextLower = "";
        for (char ch : m_filterText) { 
            filterTextLower += std::tolower(ch); 
        }

        std::string levelNameLower = "";
        for (char ch : m_AllLevels[i].second.levelName) { 
            levelNameLower += std::tolower(ch); 
        }
        if (m_AllLevels[i].second.levelName == "-1")
            levelNameLower = "unknown name";

        if (m_AllLevels[i].first != myKey && StatsManager::ContainsAtIndex(0, m_filterText, levelNameLower)){
            levelsListItems->addObject(LinkLevelCell::create(this, m_AllLevels[i].first, m_AllLevels[i].second, false));
        }
    }

    auto levelsListView = ListView::create(levelsListItems, 40, CellsWidth, 220);

    m_LevelsList = GJListLayer::create(levelsListView, "", {0,0,0,75}, CellsWidth, 220, 1);
    m_LevelsList->setPosition({-155, -118});
    if (prevY != 1 && SavePos){
        m_LevelsList->m_listView->m_tableView->m_contentLayer->setPositionY(prevY);
    }
    alighmentNode->addChild(m_LevelsList);

    SpacialEditList(m_LevelsList, {m_LevelsList->getContentSize().width / 2, 234}, 0.7f);
}

void DTManageLevelsLayer::SpacialEditList(GJListLayer* list, CCPoint titlePos, float titleSize){
    CCObject* child;

    CCARRAY_FOREACH(list->m_listView->m_tableView->m_cellArray, child){
        auto childCell = dynamic_cast<GenericListCell*>(child);
        if (childCell)
            childCell->m_backgroundLayer->setOpacity(30);
    }

    std::vector<CCSprite*> spritesToRemove;
    CCLabelBMFont* title;

    CCARRAY_FOREACH(list->getChildren(), child){
        auto childSprite = dynamic_cast<CCSprite*>(child);
        auto childLabel = dynamic_cast<CCLabelBMFont*>(child);
        if (childSprite)
            spritesToRemove.push_back(childSprite);
        if (childLabel)
            title = childLabel;
    }

    for (int i = 0; i < spritesToRemove.size(); i++)
    {
        spritesToRemove[i]->removeMeAndCleanup();
    }

    title->setScale(titleSize);
    if (titlePos.x != -2000)
        title->setPosition({titlePos.x, titlePos.y});
}

void DTManageLevelsLayer::onOverallInfo(CCObject*){
    auto alert = FLAlertLayer::create("Help", "Click the <cr>trash</c> button to <ca>COMPLETELY</c> delete your progress on any level.\n \nClick the <cg>view</c> button to be sent to that level page.", "Ok");
    alert->setZOrder(150);
    this->addChild(alert);
}