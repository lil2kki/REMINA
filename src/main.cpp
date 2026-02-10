#include <Geode/Geode.hpp>

using namespace geode::prelude;


#include <Geode/modify/GameManager.hpp>
class $modify(GameManagerSetsForGV, GameManager) {
	bool getGameVariable(char const* tag) {
		if (std::string(tag) == "0024") return "Show Cursor In-Game";
		if (std::string(tag) == "0128") return not "Lock Cursor In-Game";
		return GameManager::getGameVariable(tag);
	};
};
#include <Geode/modify/MoreOptionsLayer.hpp>
class $modify(MoreOptionsLayerExt, MoreOptionsLayer) {
	void addToggle(char const* name, char const* tag, char const* desc) {
		if (std::string(tag) == "0024") return; // hide "Show Cursor In-Game"
		if (std::string(tag) == "0128") return; // hide "Lock Cursor In-Game"
		MoreOptionsLayer::addToggle(name, tag, desc);
	};
};

#include <Geode/modify/CCMenuItemSprite.hpp>
class $modify(CCMenuItemSpriteSoundExt, CCMenuItemSprite) {
	$override void selected() {
		FMODAudioEngine::get()->playEffect("btnClick.ogg");
		return CCMenuItemSprite::selected();
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
		return true;
	};
};

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayerExt, LoadingLayer) {
	bool init(bool refresh) {
		if (!LoadingLayer::init(refresh)) return false;

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("bg-texture"))) {
			a->setDisplayFrame(CCSprite::create("edit_barBG_001.png")->displayFrame());
			a->setScaleX(this->getContentSize().width / a->getContentSize().width);
			a->setScaleY(this->getContentSize().height / a->getContentSize().height);
			a->setColor(ccWHITE);
		}

		return true;
	}
};