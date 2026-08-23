#include <Geode/Geode.hpp>

using namespace geode::prelude;

auto static LOADING_SONG = [] { srand(time(nullptr)); return std::vector<const char*>{
	"""truth.mp3"
	, "truth_pt1.mp3"
	, "truth_pt2.mp3"
	, "truth_pt3.mp3"
	, "truth_pt4.mp3"
	, "truth_pt5.mp3"
}[rand() % 6]; };

#define mod(id) (Loader::get()->getInstalledMod(id) ? Loader::get()->getInstalledMod(id) : getMod())

$on_game(TexturesLoaded) {
    static int done;
	if (done++) return;
	//lr70.main-levels-editor REMOVE_UI
	/*
    listenForSettingChanges<bool>("REMOVE_UI", [](bool value) {
        Notification::create("I'm sorry, but I won't let you change that.")->show();
	    if(!mod("lr70.main-levels-editor")->getSavedValue<bool>("REMOVE_UI")) mod("lr70.main-levels-editor")->setSettingValue<bool>("REMOVE_UI", true);
    }, mod("lr70.main-levels-editor"));
	mod("lr70.main-levels-editor")->setSettingValue<bool>("REMOVE_UI", true);
	*/
	//mirrasovers
	Ref mirrasovers = CCSprite::create();
	mirrasovers->setAnchorPoint({ 0, 0 });
	mirrasovers->setID("mirrasovers");
	//add
	OverlayManager::get()->addChild(mirrasovers);
	//vars
	auto name = fmt::format("mirrasovers{}.png", ++mirrasovers->m_nTag);
	while (fileExistsInSearchPaths(name.c_str())) {
		auto entry = CCSprite::create(name.c_str());
		entry->CCNode::setScale(
			CCDirector::get()->getWinSize().width / entry->getContentSize().width,
			CCDirector::get()->getWinSize().height / entry->getContentSize().height
		);
		entry->setAnchorPoint({ 0, 0 });
		mirrasovers->addChild(entry);
		name = fmt::format("mirrasovers{}.png", ++mirrasovers->m_nTag);
	}
	//actions
	mirrasovers->runAction(CCRepeatForever::create(CCSequence::create(
		CCDelayTime::create(0.1f), CallFuncExt::create(
			[mirrasovers] {
				auto game = GameManager::get()->m_gameLayer;
				mirrasovers->setVisible(!game or (game and not game->isRunning()));
			}
		), nullptr
	)));
	mirrasovers->setCascadeOpacityEnabled(true);
	mirrasovers->runAction(CCRepeatForever::create(CCSequence::create(
		CCDelayTime::create(0.1f), CallFuncExt::create(
			[mirrasovers] {
				if (rand() % 100 < 10) return;
				mirrasovers->setOpacity(0 + (rand() % 20));
				//hide all children, then show a random one
				for (auto child : mirrasovers->getChildrenExt()) 
					child->setVisible(false);
				//here 
				auto hi = mirrasovers->getChildByType<CCSprite>(
					rand() % mirrasovers->getChildrenCount()
				); 
				hi->setVisible(true);
				hi->setFlipX(rand() % 2);
				hi->setFlipY(rand() % 2);
			}
		), nullptr
	)));
};

#include <Geode/modify/CCTextInputNode.hpp>
class $modify(CCTextInputNodeExt, CCTextInputNode) {
	virtual void textChanged() {
		if (CCKeyboardDispatcher::get()->getControlKeyPressed()) return CCTextInputNode::textChanged();
		if (getUserObject("no-parse")) return CCTextInputNode::textChanged();
		std::string a = getString().c_str();
		auto parse = matjson::parse(
			string::replace(matjson::Value(a.c_str()).dump(), "\\\\", "\\")
		).unwrapOr(a.c_str()).asString().unwrapOr(a.c_str());
		if (parse != a) setString(parse);
		CCTextInputNode::textChanged();
	};
	bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
		queueInMainThread(
			[__this = Ref(this)] {
				if (!__this) return;
				if (!typeinfo_cast<CCTextInputNode*>(__this.data())) return;

				__this->m_filterSwearWords = false;
				if (std::string(__this->m_allowedChars.c_str()).find("a") != std::string::npos) {
					__this->m_allowedChars = " !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
				}
			}
		);
		return CCTextInputNode::init(
			width, height, placeholder, textFont, fontSize, labelFont
		);
	};
};

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
	cocos2d::ccColor3B getBGColor(int index) {
		return GameManager::get()->colorForIdx(index);
	}
	virtual void update(float dt) {
		MenuGameLayer::update(rand() % 44 > 1 ? dt : -(dt * 3));
	};
};

#include <Geode/modify/CCDirector.hpp>
class $modify(voices, CCDirector) {
	static void replay() {
		if ((rand() % 100) < 25) return;
		//vars
		auto vec = std::vector<std::string>();
		auto name = fmt::format("Recording {}.ogg", 1);
		while (fileExistsInSearchPaths(name.c_str())) {
			vec.push_back(name);
			name = fmt::format("Recording {}.ogg", 1 + vec.size());
		}
		//play
		FMODAudioEngine::get()->playEffect(vec[rand() % vec.size()].c_str());
	}
	void willSwitchToScene(CCScene* pScene) {
		replay();
		if (auto a = OverlayManager::get()->getChildByID("mirrasovers")) a->runAction(CCFadeOut::create(0.5f));
		return CCDirector::willSwitchToScene(pScene);
	}
};

#include <Geode/modify/GameManager.hpp>
class $modify(IconHackExt, GameManager) {
	void checkUsedIcons() { 
		"do nothing here :3";
	}
	bool isIconUnlocked(int id, IconType type) { 
		if (rand() % 2) return true;
		return GameManager::isIconUnlocked(id, type);
	}
	bool isColorUnlocked(int id, UnlockType type) {
		if (rand() % 2) return true;
		return GameManager::isColorUnlocked(id, type);
	}
};

#include <Geode/modify/GameManager.hpp>
class $modify(GameManagerSetsForGV, GameManager) {
	bool getGameVariable(char const* tag) {
		if (std::string(tag) == "0024") return "Show Cursor In-Game";
		if (std::string(tag) == "0128") return not "Lock Cursor In-Game";
		if (std::string(tag) == "0095") return "Do Not...";
		return GameManager::getGameVariable(tag);
	};
};
#include <Geode/modify/MoreOptionsLayer.hpp>
class $modify(MoreOptionsLayerExt, MoreOptionsLayer) {
	void addToggle(char const* name, char const* tag, char const* desc) {
		if (std::string(tag) == "0024") return; // hide "Show Cursor In-Game"
		if (std::string(tag) == "0128") return; // hide "Lock Cursor In-Game"
		if (std::string(tag) == "0095") return; // hide "Do Not..."
		MoreOptionsLayer::addToggle(name, tag, desc);
	};
};

#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(CCMenuItemSpriteExtraExt, CCMenuItemSpriteExtra) {
	void selected() {
		if (getMod()->getSettingValue<bool>("add button sounds")) {
			if (m_selectSound.empty()) this->m_selectSound = "menuItemSelectSound.ogg";
			if (m_activateSound.empty()) this->m_activateSound = "menuItemActivateSound.ogg";
		}
		return CCMenuItemSpriteExtra::selected();
	}
};

#include <Geode/modify/FLAlertLayer.hpp>
class $modify(FLAlertLayerExt, FLAlertLayer) {
	bool init(
		FLAlertLayerProtocol * delegate, char const* title, gd::string desc, 
		char const* btn1, char const* btn2, float width, 
		bool scroll, float height, float textScale
	) {
		if (!FLAlertLayer::init(
			delegate, title, desc, btn1, btn2, width, scroll, height, textScale
		)) return false;
		//nothing
		return true;
	};
	void show() {
		//put popup in next scene, not transition
		m_scene = m_scene ? m_scene : CCDirector::get()->getRunningScene();
		if (auto trans = typeinfo_cast<CCTransitionScene*>(m_scene)) {
			m_scene = trans->m_pInScene;
		}

		FLAlertLayer::show();
	}
};

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayerExt, LoadingLayer) {
	bool init(bool refresh) {
		if (!LoadingLayer::init(refresh)) return false;

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("bg-texture"))) {
			a->setDisplayFrame(CCSprite::create("spritesheet.png"_spr)->displayFrame());
			a->setScaleX(this->getContentSize().width / a->getContentSize().width);
			a->setScaleY(this->getContentSize().height / a->getContentSize().height);
			a->setColor(ccWHITE);
			a->getTexture()->setAliasTexParameters();
		}

		auto dialogIcon_6893 = CCFileUtils::get()->fullPathForFilename(
			"dialogIcon_6893.png", 0
		);
		for (int a = 29; a <= 36; a++) {//036
			CCFileUtils::get()->m_fullPathCache[
				fmt::format("dialogIcon_0{}.png", a)
			] = dialogIcon_6893;
		}
		CCFileUtils::get()->m_fullPathCache["dialogIcon_005.png"] = dialogIcon_6893;
		CCFileUtils::get()->m_fullPathCache["dialogIcon_006.png"] = dialogIcon_6893;

		auto walls = CCFileUtils::get()->fullPathForFilename(
			"menuLoop/1465577_Walls-are-moving-but-I-can.mp3", 0
		);
		CCFileUtils::get()->m_fullPathCache["shop.mp3"] = walls;
		CCFileUtils::get()->m_fullPathCache["shop3.mp3"] = walls;
		CCFileUtils::get()->m_fullPathCache["shop4.mp3"] = walls;
		CCFileUtils::get()->m_fullPathCache["shop5.mp3"] = walls;

		FMODAudioEngine::get()->setBackgroundMusicVolume(GameManager::get()->m_bgVolume);
		FMODAudioEngine::get()->setEffectsVolume(GameManager::get()->m_sfxVolume);

		GameManager::get()->fadeInMusic(LOADING_SONG());

		return true;
	}
};


#include <Geode/modify/FLAlertLayer.hpp>
class $modify(EditAccountID_FLAlertLayer, FLAlertLayer) {
	void trySaveValue(float) {
		if (Ref ew = GameManager::get()) ew->save();
		if (Ref aw = Notification::create("Saved!")) aw->show();
	}
	void show() {
		FLAlertLayer::show();
		
		if (Ref casted = typeinfo_cast<AccountLoginLayer*>(this)) {
			// input
			if (Ref accm = GJAccountManager::get()) if (Ref inp = TextInput::create(62.f, fmt::format("{}", accm->m_accountID).c_str())) {
				// preset value
				inp->setString(fmt::format("{}", accm->m_accountID).c_str());
				// callback
				inp->setCallback(
					[inp = Ref(inp)](const std::string& p0) {
						if (Ref manager = GJAccountManager::get()) manager->m_accountID = utils::numFromString<int>(
							p0
						).unwrapOr(manager->m_accountID);
						if (inp) inp->unschedule(schedule_selector(EditAccountID_FLAlertLayer::trySaveValue));
						if (inp) inp->scheduleOnce(schedule_selector(EditAccountID_FLAlertLayer::trySaveValue), 0.5f);
					}
				);
				// position
				inp->setPosition({ 145.000f, -86.000f });
				// label
				if (Ref inpInner = inp->getInputNode()) {
					Ref lbl = CCLabelBMFont::create("Account ID:\n \n \n \n ", "bigFont.fnt");
					if (lbl) lbl->setScale(0.400f);
					if (lbl) inpInner->addChild(lbl, 1, "inp-label"_h);
				}
				// add to menu
				if (Ref menu = this->m_buttonMenu) menu->addChild(inp);
			};
		}
	};
};
