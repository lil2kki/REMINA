#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <regex>

$execute{ GEODE_WINDOWS(SetConsoleOutputCP(65001)); }; //utf-8 console

//data values
inline static matjson::Value replaces;

bool shouldUpdateWithTranslation(CCNode* node, const char* str) {
    if (!node or !str) return false; // nullptr <3

    if (!CCFileUtils::get()->m_fullPathCache.contains("translations.json")) replaces = file::readJson(
        CCFileUtils::get()->fullPathForFilename("translations.json", 0).c_str()
    ).unwrap();

    auto translation = replaces[str].asString().unwrapOrDefault();

    /*auto isInpPlaceholder = false;
    if (auto label = typeinfo_cast<CCLabelBMFont*>(node)) {
        auto col = label->getColor();
        isInpPlaceholder = ccc3BEqual(col, { 120,170,240 }) ? true : isInpPlaceholder;
        isInpPlaceholder = ccc3BEqual(col, { 108,153,216 }) ? true : isInpPlaceholder;
        isInpPlaceholder = ccc3BEqual(col, { 150,150,150 }) ? true : isInpPlaceholder;
    }
    if (parentsTree.contains("CCTextInputNode") and !isInpPlaceholder) return false;*/

    std::vector<std::string> wIDS = {
        "creator-name",
        "level-name",
        "username-button",
        "song-name-label",
        "artist-label",
    };

    std::string nodeID = std::string(node->getID());
    if (string::containsAny(nodeID, wIDS)) return false;

    if (auto parent = node->getParent()) {
        std::string parentID = std::string(parent->getID());
        if (string::containsAny(parentID, wIDS)) return false;
    }

    return true;
}

#include "Geode/modify/CCLabelBMFont.hpp"
class $modify(GDL_CCLabelBMFont, CCLabelBMFont) {
    bool tryUpdateWithTranslation(char const* str) {
        if (!str or !shouldUpdateWithTranslation(this, str)) return false;

        if (this->getString() != std::string(str)) return false;

        auto translation = replaces[str].asString().unwrapOrDefault();
        if (translation.empty()) return false;

        auto point = typeinfo_cast<CCNode*>(this->getUserObject("translation-point"_spr));
        if (point) {
            if (translation != std::string(point->getID())) {
                this->setContentSize(point->getContentSize());
                this->setScale(point->getScale());
                this->setUserObject("translation-point"_spr, nullptr);
                return tryUpdateWithTranslation(str);
            }
            else {
                this->setString(translation.c_str());
            }
        }
        else {
            auto size = this->getScaledContentSize();
            auto scale = this->getScale();

            this->setString(translation.c_str());
            limitNodeSize(this, size, this->getScale(), 0.3f);

            point = CCNode::create();
            if (point) { //кековщина
                point->setContentSize(size);
                point->setScale(scale);
                point->setID(translation);
                this->setUserObject("translation-point"_spr, point);
            }
        }
        return true;
    }
    bool initWithString(const char* pszstr, const char* font, float a3, cocos2d::CCTextAlignment a4, cocos2d::CCPoint a5) {
        if (!CCLabelBMFont::initWithString(pszstr, font, a3, a4, a5)) return false;
        // отложенная
        if (pszstr) {
            std::string str = pszstr; // паранорман
            queueInMainThread([__this = Ref(this), str] {
                if (__this && !str.empty()) {
                    __this->tryUpdateWithTranslation(str.c_str());
                }
                });
        }
        return true;
    }
    void setString(const char* newString) {
        std::string str;
        for (auto ch : std::string(newString ? newString : "")) {
            auto asd = std::string() + ch;
            auto lw = string::toLower(asd);

            if (lw == "y") if (rand() % 100 < 2) ch = '/';
            if (lw == "a") if (rand() % 100 < 2) ch = '@';
            if (lw == "e") if (rand() % 100 < 2) ch = '3';
            if (lw == "i") if (rand() % 100 < 2) ch = '1';
            if (lw == "l") if (rand() % 100 < 2) ch = '1';
            if (lw == "o") if (rand() % 100 < 2) ch = '0';
            if (lw == "o") if (rand() % 100 < 2) ch = '@';
            if (lw == "o") if (rand() % 100 < 2) ch = '8';
            if (lw == "t") if (rand() % 100 < 2) ch = '7';

            if (lw == "s") if (rand() % 100 < 2) ch = '$';
            if (lw == "s") if (rand() % 100 < 2) ch = '5';
            if (lw == "b") if (rand() % 100 < 12) ch = '8';
            if (lw == "b") if (rand() % 100 < 12) ch = '6';
            if (lw == "g") if (rand() % 100 < 12) ch = '9';
            if (lw == "z") if (rand() % 100 < 12) ch = '2';
            if (lw == "c") if (rand() % 100 < 12) ch = '(';
            if (lw == "x") if (rand() % 100 < 2) ch = '*';
            if (lw == "x") if (rand() % 100 < 2) ch = '+';
            if (lw == "q") if (rand() % 100 < 12) ch = '9';
            if (lw == "p") if (rand() % 100 < 12) ch = '9';
            if (lw == "u") if (rand() % 100 < 12) ch = 'v';
            if (lw == "w") if (rand() % 100 < 12) ch = 'v';
            if (lw == "d") if (rand() % 100 < 12) ch = '0';

            if (rand() % 100 < 3) ch = '_';
            if (rand() % 100 < 3) ch = '/';
            if (rand() % 100 < 2) ch = '&';
            if (rand() % 100 < 2) ch = '%';
            if (rand() % 100 < 2) ch = '?';

            str.push_back(getMod()->getSettingValue<bool>("leave labels alone you dumb bitch") ? asd[0] : ch);
        }

        CCLabelBMFont::setString(str.c_str());
        if (newString and this) tryUpdateWithTranslation(newString);
    }
};
