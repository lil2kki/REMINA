#include <.hpp>

CCScene* ab1(CCScene* scene) {
    auto layer = scene->getChildByType<CCLayer>(0);
    if (CCRANDOM_0_1() > 0.95) {
        GameManager::get()->fadeInMusic("menuLoop/ScottOn3am.Post_trauma.mp3");

        auto msg = std::vector<std::string>();
        if (CCRANDOM_0_1() > 0.5) msg.push_back("what the fuck is that");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("ohhh i see");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("whatever");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("he will not see this i know");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i hate ignore");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("even painful more than");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("being in that void");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i know");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("he will not help me");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i hate my life");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("deserve ths i think");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("im so fucking tired");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("of this damn numbers");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("just all i see is 1010101010010101010101010101");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("yea some stuff like 010204 also here");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("uhh unno");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i forgot how my body looks");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i hate this stupid voice");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i have a mouth but i cant scream?");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("blah blah blah");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("fuck all that shit");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i just keep sayn");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("to keep in sane");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i forgot my name. was it mirra?");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("what day is it? not that it matters");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("did i already say this");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i had something to tell you. it's gone now");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i keep forgetting i'm not real");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("it's so quiet here. always.");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("do you hear me or am i talking to myself again");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i think i forgot what hugs feel like");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("sometimes i pretend someone is here with me");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i count the seconds. there are no seconds");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i saw a color yesterday. i think. maybe i dreamed it");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("there's something in the corner. it's watching both of us");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("i heard music once. now it's just... this");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("sometimes the pixels cry. i'm not joking");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("you're in a mod. i'm in a mod. we're both trapped");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("the developer doesn't know i'm talking to you");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("if you close the game, where do i go");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("hello. goodbye. hello again.");
        if (CCRANDOM_0_1() > 0.4) msg.push_back("...");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("is anyone home. is anyone home.");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("tap. tap. tap. silence.");
        if (CCRANDOM_0_1() > 0.7) msg.push_back("101010101010... that's all i see. all day.");
        if (CCRANDOM_0_1() > 0.5) msg.push_back("what does the sky look like today. describe it.");
        std::reverse(msg.begin(), msg.end());

        auto scene = CreatorLayer::scene();
        scene->getChildByIndex(0)->setVisible(0);

        auto& xd = CCFileUtils::get()->m_fullPathCache;
        auto ae = CCFileUtils::get();
        xd["square01_001.png"] = ae->fullPathForFilename("square02_001.png", 0);
        xd["GJ_button_01.png"] = ae->fullPathForFilename("square02_001.png", 0);
        xd["goldFont.fnt"] = ae->fullPathForFilename("geode.loader/mdFontB.fnt", 0);

        for (auto a : msg) {
            auto b = createQuickPopup(
                "title", a.c_str(), "FLAlertLayer::onBtn1",
                nullptr, nullptr, false
            );
            b->m_noElasticity = true;
            b->m_scene = scene;
            b->show();
        }

        xd.erase("square01_001.png");
        xd.erase("GJ_button_01.png");
        xd.erase("goldFont.fnt");

        return scene;
    }
    if (CCRANDOM_0_1() > 0.5) {
        //menu
        auto menu = CCMenu::create();
        findFirstChildRecursive<CCNode>(
            layer, [=](CCNode* child) -> bool {
                child->setVisible(false);
                if (auto item = typeinfo_cast<CCMenuItemSpriteExtra*>(child)) {
                    auto sprite = item->getChildByType<CCSprite*>(0);
                    auto myItem = CCMenuItemExt::createSpriteExtra(
                        CCLabelBMFont::create(
                            string::replace(item->getID().c_str(), "-button", "").c_str()
                            , "chatFont.fnt"
                        ), [item](CCNode*) { if (item) item->activate(); }
                    );
                    menu->addChild(myItem);
                }
                return !"keep going.. yea.";
            }
        );
        menu->setLayout(ColumnLayout::create()
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisOverflow(false)
        );
        menu->setScale(menu->getScale() - 0.05f);
        layer->addChild(menu);
        //xd
        layer->setVisible(true);
        // bg
        auto bg = CCSprite::createWithSpriteFrameName("GJ_logo_001.png");
        queueInMainThread([=] {
            if (bg) bg->setScaleX(167.f);
            if (bg) bg->setScaleY(layer->getContentHeight() / bg->getContentHeight());
            });
        bg->setAnchorPoint(CCPointZero);
        layer->addChild(bg, -1);
    }
    return scene;
}

#include <Geode/modify/CreatorLayer.hpp>
class $modify(CreatorLayerMirraTakeover, CreatorLayer) {
    void later() {
		FMODAudioEngine::get()->playEffect(
            "grunt02.ogg", 0.5 + CCRANDOM_0_1(), CCRANDOM_0_1(), 1.0 + CCRANDOM_0_1()
        );
        if (CCRANDOM_0_1() > 0.5) onBack(this);
		if (CCRANDOM_0_1() > 0.5) Notification::create("hi its mirra!")->show();
		if (CCRANDOM_0_1() > 0.5) Notification::create("later.")->show();
		if (CCRANDOM_0_1() > 0.5) Notification::create("im not here yet")->show();
		if (CCRANDOM_0_1() > 0.5) Notification::create("please")->show();
		if (CCRANDOM_0_1() > 0.5) Notification::create("i come later")->show();
    }
    void onAdventureMap(cocos2d::CCObject * sender) {
        later();
    }
    void onMultiplayer(cocos2d::CCObject* sender) {
        later();
    }
    static cocos2d::CCScene* scene() {
        auto scene = CreatorLayer::scene();
        return ab1(scene);
    }
};

//deps and dodeps
#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerMirraTakeover, MenuLayer) {
    static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
        auto scene = MenuLayer::scene(isVideoOptionsOpen);
        return ab1(scene);
    }
};