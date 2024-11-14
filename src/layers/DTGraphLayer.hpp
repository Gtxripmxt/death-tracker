#pragma once

#include <Geode/Geode.hpp>
#include "../layers/DTLayer.hpp"
#include "../layers/GraphPoint.hpp"
#include "Geode/ui/TextArea.hpp"
#include "../layers/GraphPointDelegate.hpp"

using namespace geode::prelude;

class DTGraphLayer : public Popup<DTLayer* const&>, public TextInputDelegate, public GraphPointDelegate {
    protected:
        bool setup(DTLayer* const& layer) override;

        void update(float delta);
    public:
        static DTGraphLayer* create(DTLayer* const& layer);

        DTLayer* m_DTLayer;

        CCNode* alighmentNode;

        enum GraphType{
            PassRate,
            ReachRate
        };

        GraphType currentType = GraphType::PassRate;

        CCNode* CreateGraph(const std::vector<DeathInfo>& deathsString, int bestRun, ccColor3B color, CCPoint Scaling, ccColor4B graphBoxOutlineColor, ccColor4B graphBoxFillColor, float graphBoxOutlineThickness, ccColor4B labelLineColor, ccColor4B labelColor, int labelEvery, ccColor4B gridColor, int gridLineEvery, GraphType type);
        CCNode* CreateRunGraph(const std::vector<DeathInfo>& deathsString, int bestRun, ccColor3B color, CCPoint Scaling, ccColor4B graphBoxOutlineColor, ccColor4B graphBoxFillColor, float graphBoxOutlineThickness, ccColor4B labelLineColor, ccColor4B labelColor, int labelEvery, ccColor4B gridColor, int gridLineEvery, GraphType type);
        static int GetBestRun(NewBests bests);
        static int GetBestRunDeathS(const std::vector<DeathInfo>& selectedPrecentDeathsInfo);
        static int GetBestRun(const std::vector<DeathInfo>& selectedPrecentRunInfo);

        std::vector<GraphPoint*> pointToDisplay;
        CCLabelBMFont* npsLabel;
        SimpleTextArea* PointInfoLabel;

        void OnPointSelected(cocos2d::CCNode* point);

        void OnPointDeselected(cocos2d::CCNode* point);

        void switchedSessionRight(CCObject*);
        void switchedSessionLeft(CCObject*);
        TextInput* m_SessionSelectionInput;
        void textChanged(CCTextInputNode* input);
        void textInputClosed(CCTextInputNode* input);
        void textInputOpened(CCTextInputNode* input);

        void onClose(cocos2d::CCObject*);

        ButtonSprite* viewModeButtonS;
        ButtonSprite* runViewModeButtonS;
        ButtonSprite* typeViewModeButtonS;
        void onViewModeButton(CCObject*);
        void onRunViewModeButton(CCObject*);
        void onTypeViewModeButton(CCObject*);
        bool ViewModeNormal = true;
        bool RunViewModeFromZero = true;

        void refreshGraph();
        CCNode* m_graph = nullptr;
        CCLabelBMFont* noGraphLabel;

        TextInput* m_RunSelectInput;
        int m_SelectedRunPrecent;
        GJListLayer* m_RunsList;
        void RunChosen(int run);

        //info
        void onOverallInfo(CCObject*);
};