#include <.hpp>

#include <user95401.game-objects-factory/include/main.hpp>
#include <user95401.game-objects-factory/include/impl.hpp>

#include <Geode/modify/GJGameLoadingLayer.hpp>
class $modify(GJGameLoadingLayerFuckYouuu, GJGameLoadingLayer) {
	static GJGameLoadingLayer* transitionToLoadingLayer(GJGameLevel * level, bool editor) {
		Ref layer = EditLevelLayer::create(level);
		switchToScene(layer);
		return nullptr;
	}
};

void SetupObjects();
$on_mod(Loaded) { SetupObjects(); }
inline void SetupObjects() {
	static auto plrinputtrigger = GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("plr-input-crtl"), "pd-plr-input-crtl.png",
		[](EffectGameObject* object, GJBaseGameLayer* game, int, gd::vector<int> const*) {
			GameOptionsTrigger* options = typeinfo_cast<GameOptionsTrigger*>(object);
			if (!options) return log::error("options object cast == {} from {}", options, object);
			//option assignments
			typedef GameOptionsSetting Is;
			auto player = options->m_streakAdditive;
			auto jump = options->m_hideGround;
			auto left = options->m_hideMG;
			auto right = options->m_hideP1;
			//affected players
			std::vector<Ref<PlayerObject>> ps = { game->m_player1, game->m_player2 };
			if (player != Is::Disabled) ps = {
				player == Is::On ? game->m_player1 : game->m_player2
			};
			auto pb = &PlayerObject::pushButton;
			typedef PlayerButton For;
			//jump
			if (jump != Is::Disabled) for (auto p : ps) if (p) jump == Is::On ? p->pushButton(For::Jump) : p->releaseButton(For::Jump);
			//left
			if (left != Is::Disabled) for (auto p : ps) if (p) left == Is::On ? p->pushButton(For::Left) : p->releaseButton(For::Left);
			//right
			if (right != Is::Disabled) for (auto p : ps) if (p) right == Is::On ? p->pushButton(For::Right) : p->releaseButton(For::Right);
		}
	)->refID(2899)->insertIndex((12 * 7) + 1)->onEditObject(
		[](EditorUI* a, GameObject* aa) -> bool {
			queueInMainThread(
				[a = Ref(a), aa = Ref(aa)] {
					if (!CCScene::get()) return log::error("CCScene::get() == {}", CCScene::get());
					auto popup = CCScene::get()->getChildByType<SetupOptionsTriggerPopup>(0);
					if (!popup) return log::error("popup == {}", popup);
					auto object = typeinfo_cast<EffectGameObject*>(aa.data());
					if (!object) return log::error("object == {} ({})", object, aa);

					auto main = popup->m_mainLayer;
					auto menu = popup->m_buttonMenu;

					if (auto aaa = main->getChildByType<CCLabelBMFont>(0)) aaa->setString("Extended Player Control");

					//xd
					if (auto aaa = main->getChildByType<CCLabelBMFont>(6 - 2)) aaa->setString(R"(Only For)");
					if (auto aaa = main->getChildByType<CCLabelBMFont>(7 - 2)) aaa->setString(R"(P1)");
					if (auto aaa = main->getChildByType<CCLabelBMFont>(8 - 2)) aaa->setString(R"(P2)");
					//jump buffer
					if (auto aaa = main->getChildByType<CCLabelBMFont>(9 - 2)) aaa->setString(R"(jump buffer)");
					//holding left
					if (auto aaa = main->getChildByType<CCLabelBMFont>(12 - 2)) aaa->setString(R"(holding left)");
					//holding right
					if (auto aaa = main->getChildByType<CCLabelBMFont>(15 - 2)) aaa->setString(R"(holding right)");

					//other shit
					{
						auto low_iq = 18;
						while (auto aaa = main->getChildByType<CCNode>(low_iq++)) aaa->setVisible(false);
					};

					//other shit in menu
					{
						auto low_iq = 13;
						while (auto aaa = menu->getChildByType<CCNode>(low_iq++)) aaa->setVisible(false);
					};
				}
			);
			return false;
		}
	)->customSetup([](GameObject* a) { if (a) a->m_addToNodeContainer = true; });
	plrinputtrigger->registerMe();

	static GameObjectsFactory::GameObjectConfig* svcondtrigger = GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("pd-sv-cond-toggle"), "edit_eItemCompBtn_001.png",
		[](EffectGameObject* object, GJBaseGameLayer* game, int p0, gd::vector<int> const* p1) {
			if (!object) return;
			if (!game) return;

			//set:key:value (setups value)
			auto data = typeinfo_cast<CCNode*>(object->getUserObject("data"_spr));
			if (!data) return log::error("data == {}", data);
			auto split = string::split(data->getID(), ":");
			if (split.size() == 3 and split[0] == "set") {
				getMod()->getSaveContainer()[GameManager::get()->m_playerName]
					[split[1]] = matjson::parse(split[2]).unwrapOrDefault();
			}

			object->m_objectID = svcondtrigger->m_refObjectID;
			object->triggerObject(game, p0, p1);
			object->m_objectID = svcondtrigger->m_objectID;
		}
	)->refID(1049)->insertIndex(7)->onEditObject(
		[](EditorUI* a, GameObject* aa) -> bool {
			if (!a) return false;
			if (!aa) return false;
			queueInMainThread(
				[a = Ref(a), aa = Ref(aa)] {
					if (!CCScene::get()) return log::error("CCScene::get() == {}", CCScene::get());
					auto popup = CCScene::get()->getChildByType<SetupObjectTogglePopup>(0);
					if (!popup) return log::error("popup == {}", popup);
					auto object = typeinfo_cast<EffectGameObject*>(aa.data());
					if (!object) return log::error("object == {} ({})", object, aa);
					auto data = typeinfo_cast<CCNode*>(object->getUserObject("data"_spr));
					if (!data) return log::error("data == {}", data);

					if (popup->getUserObject("got-custom-setup-for-sv-cond-toggle")) return;
					popup->setUserObject("got-custom-setup-for-sv-cond-toggle", aa);

					auto main = popup->m_mainLayer;
					auto menu = popup->m_buttonMenu;

					if (auto aaa = main->getChildByType<CCLabelBMFont>(0)) aaa->setString(" \nSave Value Based\n   Toggle Group");

					if (auto aaa = main->getChildByType<CCLabelBMFont>(-1)) aaa->setVisible(false);
					if (auto aaa = menu->getChildByType<CCMenuItem>(-1)) aaa->setVisible(false);

					auto input = TextInput::create(228.700f, "asd:=true (key:[!][=,<,>,*][value])\nset:key:value (setups value)", "chatFont.fnt");
					input->setFilter(" !\"#$ * &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
					input->getInputNode()->m_allowedChars = " !\"#$ * &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
					if (!data->getID().empty()) input->setString(data->getID());
					input->setPositionY(76.000);
					input->setCallback(
						[data = Ref(data)](const std::string& p0) {
							data->setID(p0);
						}
					);
					popup->m_buttonMenu->addChild(input);

					auto dmpinf = CCMenuItemExt::createSpriteExtra(
						ButtonSprite::create("dump"), [](void*) {
							MDPopup::create(
								"Save container dump",
								"```\n" + getMod()->getSaveContainer()[GameManager::get()->m_playerName]
								.dump() + "\n```",
								"oh wow ok, fk..."
							)->show();
						}
					);
					dmpinf->setPosition({ 116.000f, 196.000f });
					popup->m_buttonMenu->addChild(dmpinf);
				}
			);
			return false;
		}
	)->customSetup(
		[](GameObject* object)
		{
			if (!object) return object;
			Ref<CCRepeatForever> action;
			action = CCRepeatForever::create(CCSequence::create(CallFuncExt::create(
				[__this = Ref(object), action] {
					Ref object = typeinfo_cast<EffectGameObject*>(__this.data());
					if (!object) return GameManager::get()->stopAction(action);
					Ref data = typeinfo_cast<CCNode*>(object->getUserObject("data"_spr));
					if (!data) return GameManager::get()->stopAction(action);
					//data str
					auto str = data->getID();
					//update basic stuff
					if (Ref sub = object->getChildByType<CCSprite>(0)) {
						sub->setZOrder(1);
						sub->setScale(0.675f);
						sub->setPositionY(25.5f);
						sub->setPositionX(object->getContentSize().width / 2.f);
						sub->setColor(object->m_activateGroup ?
							cc3bFromHexString("#00FF28").unwrapOrDefault()
							: cc3bFromHexString("#FF0049").unwrapOrDefault()
						); 
						if (string::contains(str, "set:")) sub->setColor(
							cc3bFromHexString("#0067FF").unwrapOrDefault()
						);
					}
					//"asd:=true (key:[!][=,<,>,*][value])"
					if (str.empty()) return;
					auto split = string::split(str, ":");
					if (split.size() != 2) return;
					if (split[0].empty()) return void(); // log::error("split[0].empty()");
					if (split[1].empty()) return void(); // log::error("split[1].empty()");
					;;;; std::string key = split[0];
					;; std::string cond = &split[1].at(0);
					matjson::Value value = matjson::parse(split[1].substr(1)).unwrapOrDefault();
					if (key.empty() or cond.empty()) return void(); // log::error("key == {}, cond == {}", key, cond);
					//log::debug("key == {}, cond == {}, value == {}", key, cond, value.dump());
					auto sv = getMod()->getSaveContainer()[GameManager::get()->m_playerName]
						[key];
					//log::debug("sv == {}", sv.dump());
					auto inv = string::contains(cond, "!");
					auto& v = object->m_activateGroup;
					namespace s = string;
					if (s::contains(cond, "=")) v = (sv == value) - inv;
					if (s::contains(cond, "<")) v = (sv < value) - inv;
					if (s::contains(cond, ">")) v = (sv > value) - inv;
					if (s::contains(cond, "*")) v = s::contains(sv.dump(), value.dump()) - inv;
				}), nullptr
			));
			if (Ref a = GameManager::get()->m_gameLayer) a->runAction(action);
			object->setUserObject("data"_spr, CCNode::create());
			return object;
		}
	)->saveString(
		[](std::string str, GameObject* object, GJBaseGameLayer* level)
		{
			if (!object) return gd::string(str.c_str());
			if (!level) return gd::string(str.c_str());
			object->m_objectID = svcondtrigger->m_refObjectID;
			str = string::replace(
				object->getSaveString(level).c_str(),
				fmt::format("{},", svcondtrigger->m_refObjectID).c_str(),
				fmt::format("{},", svcondtrigger->m_objectID).c_str()
			).c_str();
			object->m_objectID = svcondtrigger->m_objectID;
			if (auto data = typeinfo_cast<CCNode*>(object->getUserObject("data"_spr))) {
				str += ",228,";
				str += ZipUtils::base64URLEncode(data->getID().c_str()).c_str();
			}
			return gd::string(str.c_str());
		}
	)->objectFromVector(
		[](GameObject* object, gd::vector<gd::string>& p0, gd::vector<void*>&, void*, bool)
		{
			if (!object) return object;
			auto data = typeinfo_cast<CCNode*>(object->getUserObject("data"_spr));
			if (data) data->setID(ZipUtils::base64URLDecode(p0[228].c_str()).c_str());
			return object;
		}
	);
	svcondtrigger->registerMe();

	GameObjectsFactory::createTriggerConfig(UNIQ_ID("plr-tw-upd"), "plr-tw-upd.png")
		->refID(1935)->insertIndex((12 * 5) + 5)->triggerObject(
			[](EffectGameObject* ob, GJBaseGameLayer* g, int, gd::vector<int> const*) {
				auto id = ob->m_objectID;
				ob->m_objectID = 1935;
				auto sVal = string::split(ob->getSaveString(g), ",120,")[1];
				ob->m_objectID = id;
				auto a = utils::numFromString<float>(sVal).unwrapOr(1.0f);
				for (auto p : { g->m_player1, g->m_player2 }) {
					if (p) p->m_customScaleY = a;
				}
			}
		)->saveString(
			[](std::string str, GameObject* ob, GJBaseGameLayer* game) {
				auto id = ob->m_objectID;
				ob->m_objectID = 1935;
				str = ob->getSaveString(game);
				ob->m_objectID = id;
				log::debug("{}", str);
				str = string::replace(str, "1,1935", fmt::format("1,{}", id)).c_str();
				//120
				return str;
			}
		)->registerMe();

	GameObjectsFactory::createTriggerConfig(UNIQ_ID("plr-tw-rot"), "plr-tw-rot.png")
		->refID(1935)->insertIndex((12 * 5) + 5)->triggerObject(
			[](EffectGameObject* ob, GJBaseGameLayer* g, int, gd::vector<int> const*) {
				auto id = ob->m_objectID;
				ob->m_objectID = 1935;
				auto sVal = string::split(ob->getSaveString(g), ",120,")[1];
				ob->m_objectID = id;
				auto a = utils::numFromString<float>(sVal).unwrapOr(1.0f);
				for (auto p : { g->m_player1, g->m_player2 }) {
					if (p) p->m_customScaleX = a;
				}
			}
		)->saveString(
			[](std::string str, GameObject* ob, GJBaseGameLayer* game) {
				auto id = ob->m_objectID;
				ob->m_objectID = 1935;
				str = ob->getSaveString(game);
				ob->m_objectID = id;
				log::debug("{}", str);
				str = string::replace(str, "1,1935", fmt::format("1,{}", id)).c_str();
				//120
				return str;
			}
		)->registerMe();

	GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("openURL"), "openURL.png",
		[](EffectGameObject* trigger, GJBaseGameLayer* game, int p1, gd::vector<int> const* p2)
		{
			auto xd = MDTextArea::create(fmt::format(
				"[{0}]({0})", trigger->m_particleString.c_str()
			), { 122,122 });
			auto item = findFirstChildRecursive<CCMenuItem>(
				xd, [](void*) { return true; }
			);
			if (item) item->activate();
		},
		[](EditTriggersPopup* popup, EffectGameObject* trigger, CCArray* objects)
		{
			if (auto title = popup->getChildByType<CCLabelBMFont*>(0)) {
				title->setString("Open URL");
				title->setAnchorPoint(CCPointMake(0.5f, 0.3f));
			}
			if (auto inf = popup->m_buttonMenu->getChildByType<InfoAlertButton*>(0)) {
				//inf->setVisible(false);
				inf->m_description = ""
					"Links have some special protocols!" "\n"
					"Use <cg>user:{accountID}</c> to link to a GD account;" "\n"
					"<cg>level:{id}</c> to link to a GD level and" "\n"
					"<cg>mod:{id}</c> to link to another Geode mod.";
			}

			auto input = TextInput::create(312.f, "", "chatFont.fnt");
			input->setFilter(" !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
			input->getInputNode()->m_allowedChars = " !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
			input->setString(trigger->m_particleString.c_str());
			input->setPositionY(55.000f);
			input->setCallback(
				[trigger = Ref(trigger)](const std::string& p0) {
					trigger->m_particleString = p0.c_str();
				}
			);
			input->getBGSprite()->setContentHeight(40.000f);
			input->getBGSprite()->setAnchorPoint({ 0.5f, 0.550f });
			popup->m_buttonMenu->addChild(input);
		}
	)->saveString(
		[](std::string str, GameObject* object, GJBaseGameLayer* level)
		{
			str += ",228,";
			str += ZipUtils::base64URLEncode(object->m_particleString).c_str();
			return str;
		}
	)->objectFromVector(
		[](GameObject* object, gd::vector<gd::string>& p0, gd::vector<void*>&, void*, bool)
		{
			object->m_particleString = ZipUtils::base64URLDecode(p0[228]).c_str();
			return object;
		}
	)->customSetup([](auto a) { a->m_addToNodeContainer = true; })->registerMe();

	GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("plr-normal-mode"), "plr-normal-mode.png",
		[](EffectGameObject* trigger, GJBaseGameLayer* game, int p1, gd::vector<int> const* p2)
		{
			if (!game) return;
			if (auto a = game->m_uiLayer) a->togglePlatformerMode(false);
			if (auto a = game->m_player1) a->m_isPlatformer = false;
			if (auto a = game->m_player2) a->m_isPlatformer = false;
		}
	)->customSetup([](auto a) { a->m_addToNodeContainer = true; })->registerMe();

	GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("plr-platformer-mode"), "plr-platformer-mode.png",
		[](EffectGameObject* trigger, GJBaseGameLayer* game, int p1, gd::vector<int> const* p2)
		{
			if (!game) return;
			if (auto a = game->m_uiLayer) a->togglePlatformerMode(true);
			if (auto a = game->m_player1) a->m_isPlatformer = true;
			if (auto a = game->m_player2) a->m_isPlatformer = true;
		}
	)->customSetup([](auto a) { a->m_addToNodeContainer = true; })->registerMe();


	GameObjectsFactory::createTriggerConfig(
		UNIQ_ID("custom-shader"), "edit_eShaderBtn_001.png",
		[](EffectGameObject* trigger, GJBaseGameLayer* game, int p1, gd::vector<int> const* p2)
		{
			auto url = trigger->m_particleString.c_str();
			Ref program = CCShaderCache::sharedShaderCache()->programForKey(url);
			if (program) {
				if (Ref shaderLayer = game->m_shaderLayer) {
					shaderLayer->m_sprite->setShaderProgram(program);
				}
				else log::error("game->m_shaderLayer = {}", game->m_shaderLayer);
				program->updateUniforms();
			}
			else log::error("shader program ({}) = {}", url, game->m_shaderLayer);
		},
		[](EditTriggersPopup* popup, EffectGameObject* trigger, CCArray* objects)
		{
			if (auto title = popup->getChildByType<CCLabelBMFont*>(0)) {
				title->setString("\n \nApply Shader From URL\n (UNFINISHED)");
				title->setAnchorPoint(CCPointMake(0.5f, 0.3f));
			}
			if (auto inf = popup->m_buttonMenu->getChildByType<InfoAlertButton*>(0)) {
				//inf->setVisible(false);
				inf->m_description = ""
					"Activate this trigger at active shader to apply custom shader program from url on it.";
			}

			auto input = TextInput::create(312.f, "fragment-shader.txt link", "chatFont.fnt");
			input->setFilter(" !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
			input->getInputNode()->m_allowedChars = " !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
			input->setString(trigger->m_particleString.c_str());
			input->setPositionY(55.000f);
			input->setCallback(
				[trigger = Ref(trigger)](const std::string& p0) {
					trigger->m_particleString = p0.c_str();
				}
			);
			input->getBGSprite()->setContentHeight(40.000f);
			input->getBGSprite()->setAnchorPoint({ 0.5f, 0.550f });
			popup->m_buttonMenu->addChild(input);
		}
	)->saveString(
		[](std::string str, GameObject* object, GJBaseGameLayer* level)
		{
			str += ",228,";
			str += ZipUtils::base64URLEncode(object->m_particleString).c_str();
			return str;
		}
	)->objectFromVector(
		[](GameObject* object, gd::vector<gd::string>& p0, gd::vector<void*>&, void*, bool)
		{
			object->m_particleString = ZipUtils::base64URLDecode(p0[228]).c_str();
			return object;
		}
	)->resetObject(
		[](GameObject* object) {
			std::string url = object->m_particleString.c_str();
			if (CCShaderCache::sharedShaderCache()->programForKey(url.c_str())) return;
			std::smatch matches;
			if (std::regex_match(url, matches, std::regex(R"(^(https?)://([^/]+)(.*)$)"))) {
				std::string scheme = matches[1];
				std::string host = matches[2];
				std::string path = matches[3].str();
				if (path.empty()) path = "/";

				log::info("Downloading: {}://{}{}", scheme, host, path);

				std::shared_ptr<httplib::Client> cli;

				cli = std::make_shared<httplib::Client>(host);

				cli->set_follow_location(true);
				cli->set_connection_timeout(30);
				cli->set_read_timeout(30);

				auto res = cli->Get(path.c_str());

				if (!res) {
					createQuickPopup(
						"Failed to download:", httplib::to_string(res.error()), 
						"OK", nullptr, nullptr
					);
					return log::error("Request failed: {}", httplib::to_string(res.error()));
				}
				if (res->status != 200) return log::error("HTTP error: {}", res->status);

				log::info("Downloaded {} bytes", res->body.size());

				Ref<CCGLProgram> program = new CCGLProgram();
				program->initWithVertexShaderByteArray(R"(
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
void main() {
	gl_Position = CC_MVPMatrix * a_position;
	v_fragmentColor = a_color;
	v_texCoord = a_texCoord;
})", res->body.c_str());
				program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
				program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
				program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
				program->link();
				program->updateUniforms();

				CCShaderCache::sharedShaderCache()->addProgram(program, url.c_str());
			}
		}
	)->registerMe();
}

#include <Geode/modify/EffectGameObject.hpp>
class $modify(MenuItemGameObject, EffectGameObject) {

	class CCMenuItem : public CCMenuItemSpriteExtra {
	public:
		CREATE_FUNC(CCMenuItem);
		virtual bool init() {
			CCMenuItemSpriteExtra::init(CCNode::create(), CCNode::create(), nullptr, nullptr);
			this->setAnchorPoint({ 0.5f, 0.5f });
			this->setEnabled(true);
			m_animationEnabled = false;
			m_colorEnabled = false;
			m_activateSound = "no sound";
			m_selectSound = "no sound";
			return true;
		};
		std::function<void(void)> m_onActivate = []() {};
		std::function<void(void)> m_onSelected = []() {};
		std::function<void(void)> m_onUnselected = []() {};
		virtual void activate() { if (m_onActivate) m_onActivate(); };
		virtual void selected() { if (m_onSelected) m_onSelected(); };
		virtual void unselected() { if (m_onUnselected) m_onUnselected(); };
		auto onActivate(std::function<void(void)> onActivate) { m_onActivate = onActivate; return this; }
		auto onSelected(std::function<void(void)> onSelected) { m_onSelected = onSelected; return this; }
		auto onUnselected(std::function<void(void)> onUnselected) { m_onUnselected = onUnselected; return this; }
	};

#define MenuItemObjectData(ring) DataNode::at(ring, "menu-item-data")
	inline static GameObjectsFactory::GameObjectConfig* conf;

	static void setupMenuItemPopup(EditorUI*, EffectGameObject * obj, SetupCollisionStateTriggerPopup * popup) {

		if (popup->getUserObject("got-custom-setup-for-menu-item")) return;
		popup->setUserObject("got-custom-setup-for-menu-item", obj);

		auto main = popup->m_mainLayer;
		auto menu = popup->m_buttonMenu;
		if (auto aaa = main->getChildByType<CCLabelBMFont>(0)) aaa->setString("Menu Item");

		if (auto aaa = main->getChildByType<CCLabelBMFont>(1)) aaa->setVisible(false);
		if (auto aaa = main->getChildByType<CCLabelBMFont>(2)) aaa->setVisible(false);
		if (auto aaa = main->getChildByType<CCScale9Sprite>(1)) aaa->setVisible(false);
		if (auto aaa = main->getChildByType<CCScale9Sprite>(2)) aaa->setVisible(false);
		if (auto aaa = main->getChildByType<CCTextInputNode>(0)) aaa->setVisible(false);
		if (auto aaa = main->getChildByType<CCTextInputNode>(1)) aaa->setVisible(false);

		while (auto aaa = menu->getChildByTag(51)) aaa->removeFromParentAndCleanup(false);
		while (auto aaa = menu->getChildByTag(71)) aaa->removeFromParentAndCleanup(false);

		auto data = MenuItemObjectData(obj);

		//activate
		auto activate = TextInput::create(52.f, "ID");
		activate->setFilter("0123456789");
		activate->getInputNode()->m_allowedChars = "0123456789";
		activate->setString(utils::numToString(data->get("activate").asInt().unwrapOr(0)));
		activate->setPositionY(95.000f);
		activate->setCallback(
			[data = Ref(MenuItemObjectData(obj))](const std::string& p0) {
				data->set("activate", utils::numFromString<int>(p0).unwrapOr(0));
			}
		);
		menu->addChild(activate);
		auto activateLabel = CCLabelBMFont::create("Activate:\n \n \n \n ", "goldFont.fnt");
		activateLabel->setScale(0.5f);
		activate->getInputNode()->addChild(activateLabel);

		//selected
		auto selected = TextInput::create(54.f, "ID");
		selected->setFilter("0123456789");
		selected->getInputNode()->m_allowedChars = "0123456789";
		selected->setString(utils::numToString(data->get("selected").asInt().unwrapOr(0)));
		selected->setPosition(-95.000f, 77.f);
		selected->setCallback(
			[data = Ref(MenuItemObjectData(obj))](const std::string& p0) {
				data->set("selected", utils::numFromString<int>(p0).unwrapOr(0));
			}
		);
		menu->addChild(selected);
		auto selectedLabel = CCLabelBMFont::create("Selected:\n \n \n \n ", "goldFont.fnt");
		selectedLabel->setScale(0.5f);
		selected->getInputNode()->addChild(selectedLabel);

		//unselected
		auto unselected = TextInput::create(48.f, "ID");
		unselected->setFilter("0123456789");
		unselected->getInputNode()->m_allowedChars = "0123456789";
		unselected->setString(utils::numToString(data->get("unselected").asInt().unwrapOr(0)));
		unselected->setPosition(95.000f, 77.f);
		unselected->setCallback(
			[data = Ref(MenuItemObjectData(obj))](const std::string& p0) {
				data->set("unselected", utils::numFromString<int>(p0).unwrapOr(0));
			}
		);
		menu->addChild(unselected);
		auto unselectedLabel = CCLabelBMFont::create("Unselected:\n \n \n \n ", "goldFont.fnt");
		unselectedLabel->setScale(0.5f);
		unselected->getInputNode()->addChild(unselectedLabel);
	}

	static void setup() {
		conf = GameObjectsFactory::createRingConfig(
			UNIQ_ID("menu-item"), "menu-item.png"
		)->refID(3640)->tab(12)->insertIndex((12 * 6) + 3)->onEditObject(
			[](EditorUI* a, GameObject* aa) -> bool {
				queueInMainThread(
					[a = Ref(a), aa = Ref(aa)] {
						if (!CCScene::get()) return log::error("CCScene::get() == {}", CCScene::get());
						auto popup = CCScene::get()->getChildByType<SetupCollisionStateTriggerPopup>(0);
						if (!popup) return log::error("popup == {}", popup);
						auto object = typeinfo_cast<EffectGameObject*>(aa.data());
						if (!object) return log::error("object == {} ({})", object, aa);
						setupMenuItemPopup(a, object, popup);
					}
				);
				return false;
			}
		)->saveString(
			[](std::string str, GameObject* object, GJBaseGameLayer* level)
			{
				str += ",228,";
				str += ZipUtils::base64URLEncode(MenuItemObjectData(object)->_json_str.c_str()).c_str();
				return str;
			}
		)->objectFromVector(
			[](GameObject* object, gd::vector<gd::string>& p0, gd::vector<void*>& p1, GJBaseGameLayer* p2, bool p3)
			{
				auto parsed = matjson::parse(
					ZipUtils::base64URLDecode(p0[228].c_str()).c_str()
				).unwrapOrDefault();
				for (auto& [key, value] : parsed) MenuItemObjectData(object)->set(key, value);
				return object;
			}
		)->customSetup(
			[](GameObject* object) {
				object->m_addToNodeContainer = true;
				object->m_outerSectionIndex = -1;
				object->m_isInvisible = false;
				object->setDisplayFrame(object->m_editorEnabled ?
					object->displayFrame() : CCSprite::create()->displayFrame()
				);
			}
		)->resetObject(
			[](GameObject* pObj) {
				if (!GameManager::get()->m_gameLayer) return;
				Ref game(GameManager::get()->m_gameLayer);

				Ref object(pObj);

				int uid = hash(object->getSaveString(game).c_str());
				object->setTag(uid);

				Ref menu = typeinfo_cast<CCMenu*>(game->getUserObject("objects-menu"));
				if (!menu) {
					menu = CCMenu::create();
					menu->setID("objects-menu");
					menu->setPosition(CCSizeZero);
					menu->setContentSize(CCSizeZero);
					menu->setAnchorPoint(CCPointZero);
					game->setUserObject("objects-menu", menu);
					game->m_uiTriggerUI->addChild(menu);
				}

				while (menu->getChildByTag(uid)) menu->removeChildByTag(uid);

				Ref item = CCMenuItem::create();
				if (item) {
					typedef gd::vector<int> xd;
					//virtual void spawnGroup(int group, bool ordered, double delay, gd::vector<int> const& remapKeys, int triggerID, int controlID);
					item->onActivate([game = Ref(GameManager::get()->m_gameLayer), data = Ref(MenuItemObjectData(object))] {
						if (game) game->spawnGroup(data->get("activate").asInt().unwrapOr(0), false, 0, gd::vector<int>(), -1, -1);
						});
					item->onSelected([game = Ref(GameManager::get()->m_gameLayer), data = Ref(MenuItemObjectData(object))] {
						if (game) game->spawnGroup(data->get("selected").asInt().unwrapOr(0), false, 0, gd::vector<int>(), -1, -1);
						});
					item->onUnselected([game = Ref(GameManager::get()->m_gameLayer), data = Ref(MenuItemObjectData(object))] {
						if (game) game->spawnGroup(data->get("unselected").asInt().unwrapOr(0), false, 0, gd::vector<int>(), -1, -1);
						});
					item->setUserObject("menu-item-object", object);
					item->setTag(uid);
				}
				else return;
				menu->setTouchEnabled(false);
				menu->setTouchEnabled(true);

				Ref action = menu->getActionByTag(uid);
				if (!action) {
					action = CCRepeatForever::create(CCSequence::create(CallFuncExt::create(
						[object, item, menu, game] {
							if (!game) return;
							if (!object) return;
							if (!item) return;
							if (!menu) return;
							if (item->getParent() != menu) {
								item->removeFromParentAndCleanup(false);
								menu->addChild(item);
								menu->setTouchEnabled(false);
								menu->setTouchEnabled(true);
							}
							menu->setVisible(game->m_uiLayer->isVisible());
							item->setContentWidth(object->m_width);
							item->setContentHeight(object->m_height);
							item->setAnchorPoint(CCPointMake(0.5, 0.5) * not object->m_editorEnabled);
							item->setAdditionalTransform(CCAffineTransformConcat(
								object->nodeToWorldTransform(),
								CCAffineTransformInvert(menu->nodeToWorldTransform())
							));
							item->updateTransform();
						}
					), nullptr));
					action->setTag(uid);
					menu->runAction(action);
				}
			}
		);
		conf->registerMe();
	}
	static void onModify(auto&) { setup(); }

	virtual void resetObject() {
		EffectGameObject::resetObject();
	};

};

#include <Geode/modify/UILayer.hpp>
class $modify(UILayerKeysExt, UILayer) {
	void customUpdate(float) {
		this->setKeyboardEnabled(false);
		this->setKeyboardEnabled(this->isVisible());
	}
	bool init(GJBaseGameLayer * p0) {
		if (!UILayer::init(p0)) return false;
		this->schedule(schedule_selector(UILayerKeysExt::customUpdate));
		return true;
	};
	void handleKeypress(cocos2d::enumKeyCodes key, bool p1) {
		UILayer::handleKeypress(key, p1);

		auto eventID = 120000 + (int)key;
		m_gameLayer->gameEventTriggered((GJGameEvent)eventID, 0, 0);
		m_gameLayer->gameEventTriggered((GJGameEvent)eventID, 0, 1 + !p1);
	}
};

#include <Geode/modify/SelectEventLayer.hpp>
class $modify(SelectEventLayerKeysExt, SelectEventLayer) {
	bool init(SetupEventLinkPopup * p0, gd::set<int>&p1) {
		if (!SelectEventLayer::init(p0, p1)) return false;

		auto keyEventsExpandBtn = CCMenuItemExt::createToggler(
			ButtonSprite::create("Keys", "goldFont.fnt", "GJ_button_04.png", 0.6f),
			ButtonSprite::create("Keys", "goldFont.fnt", "GJ_button_02.png", 0.6f),
			[popup = Ref(this)](CCMenuItemToggler* keyEventsExpandBtn) {

				while (auto a = popup->m_buttonMenu->getChildByID("key-list-item")) a->removeFromParent();
				while (auto a = popup->getChildByType<KeyEventListener>(0)) a->removeFromParent();

				if (!keyEventsExpandBtn->isOn()) return;

				auto posY = keyEventsExpandBtn->getPositionY() + keyEventsExpandBtn->getContentSize().height + 4.f;

				for (auto eventID : popup->m_eventIDs) if (eventID >= 120000 and eventID < 130000) {
					std::string name = CCKeyboardDispatcher::get()->keyToString((enumKeyCodes)(eventID - 120000));
					auto item = CCMenuItemExt::createSpriteExtra(
						ButtonSprite::create((" " + name + " ").c_str(), "goldFont.fnt", "GJ_button_05.png", 0.5f)
						, [popup, eventID, keyEventsExpandBtn](void*) {
							popup->m_eventIDs.erase(eventID);
							popup->m_eventsChanged = true;
							keyEventsExpandBtn->activate();
							keyEventsExpandBtn->activate();
						}
					);
					item->setID("key-list-item");
					item->setPositionY(posY);
					item->setPositionX(keyEventsExpandBtn->getPositionX());
					popup->m_buttonMenu->addChild(item, 999);

					posY += item->getContentSize().height + 4.f;
				}

				auto inf = CCLabelBMFont::create(" \nPress any key...", "chatFont.fnt");
				inf->setID("key-list-item");
				inf->setScale(0.625f);
				inf->setZOrder(999);
				popup->m_buttonMenu->addChildAtPosition(
					inf, Anchor::BottomLeft, { keyEventsExpandBtn->getPositionX(), posY }, false
				);

				popup->addChild(KeyEventListener::create()->onKeyDown(
					[popup, keyEventsExpandBtn](enumKeyCodes key) {
						popup->m_eventIDs.insert(120000 + (int)key);
						popup->m_eventsChanged = true;
						keyEventsExpandBtn->activate();
						keyEventsExpandBtn->activate();
					}
				), 999);
			}
		);
		keyEventsExpandBtn->setID("key-events-expand-btn");
		keyEventsExpandBtn->setPositionX(142.000f);
		keyEventsExpandBtn->setPositionY(0.f);
		keyEventsExpandBtn->activate();
		Ref(this)->m_buttonMenu->addChild(keyEventsExpandBtn);

		return true;
	}
};

#include <Geode/modify/CustomizeObjectLayer.hpp>
class $modify(CustomizeObjectLayerExt, CustomizeObjectLayer) {
	bool init(GameObject * object, cocos2d::CCArray * objects) {
		if (!CustomizeObjectLayer::init(object, objects)) return false;
		if (auto a = m_mainLayer->getChildByType<CCScale9Sprite>(0)) a->setOpacity(0);
		if (Ref input = m_textInput) {
			input->setAllowedChars("\n\t !\"#$ * &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
			input->setMaxLabelLength(255);
			Ref tip = SimpleTextArea::create(
				"\n \n \nYou can turn this label to IMAGE!\nJust put here texture/frame name or image url."
			);
			tip->setAlignment(kCCTextAlignmentCenter);
			tip->setPositionY(-34.000f);
			tip->setScale(0.650f);
			tip->setID("image-tip");
			input->addChild(tip, 1, "image-tip"_h);
			if (auto menu = querySelector("clear-text-menu")) {
				auto item = CCMenuItemExt::createSpriteExtraWithFrameName(
					"gj_findBtn_001.png", 0.75, [input](void*) {
						auto pop = MDPopup::create("", "", "");
						pop->m_mainLayer->setContentHeight(262.000f);
						pop->m_mainLayer->updateLayout();
						pop->m_mainLayer->getChildByIndex(0)->setVisible(false);
						pop->m_buttonMenu->getChildByIndex(0)->setVisible(true);
						pop->m_buttonMenu->getChildByIndex(1)->setVisible(false);
						pop->show();
						auto scroll = ScrollLayer::create(pop->m_mainLayer->getContentSize());
						scroll->setScale(0.900f);
						pop->m_mainLayer->addChild(scroll);
						auto exmenu = CCMenu::create();
						exmenu->setContentSize(pop->m_mainLayer->getContentSize());
						for (auto frame : CCDictionaryExt<std::string, CCSpriteFrame*>(
							CCSpriteFrameCache::get()->m_pSpriteFrames
						)) {
							auto exitem = CCMenuItemExt::createSpriteExtraWithFrameName(
								frame.first.c_str(), 0.5f
								, [scroll, frame, input](void*) {
									input->setString(frame.first.c_str());
								}
							);
							exitem->setID("explorer-item");
							exitem->m_animationEnabled = 0;
							exitem->m_colorEnabled = 1;
							limitNodeSize(exitem, { 50.000, 50.000 }, 1.f, 0.1f);
							exmenu->addChild(exitem);
						}
						exmenu->setLayout(RowLayout::create()
							->setCrossAxisOverflow(true)
							->setGrowCrossAxis(true)
						);
						exmenu->setPosition(CCPointZero);
						exmenu->setAnchorPoint(CCPointZero);
						scroll->m_contentLayer->setOpacity(120);
						scroll->m_contentLayer->addChild(exmenu);
						scroll->m_contentLayer->setContentSize(exmenu->getContentSize());
						handleTouchPriority(exmenu);
						handleTouchPriority(scroll);
						auto bar = Scrollbar::create(scroll);
						bar->setAnchorPoint({ 1.f, 0.f });
						pop->m_mainLayer->addChild(bar);
					}
				);
				item->setID("explorer");
				menu->addChild(item);
			}
		}
		return true;
	}
};

#include <Geode/modify/TextGameObject.hpp>
class $modify(TextGameObjectImageExt, TextGameObject) {
	bool containsUrl(const std::string & str) {
		std::regex url_pattern(
			R"((https?://|www\.)[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)+([/?#].*)?)",
			std::regex_constants::icase
		);

		return std::regex_search(str, url_pattern);
	}
	CCSpriteFrame* tryGetSpriteFrame() {
		auto name = std::string(m_text.c_str());

		if (CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(name.c_str())) {
			auto spr = CCSprite::createWithSpriteFrameName(name.c_str());
			return (spr ? spr : CCSprite::create())->displayFrame();
		}

		if (fileExistsInSearchPaths(name.c_str())) {
			auto spr = CCSprite::create(name.c_str());
			return (spr ? spr : CCSprite::create())->displayFrame();
		}

		std::smatch matches;
		if (std::regex_match(name, matches, std::regex(R"(^(https?)://([^/]+)(.*)$)"))) {
			std::string scheme = matches[1];
			std::string host = matches[2];
			std::string path = matches[3].str();
			if (path.empty()) path = "/";

			log::info("Downloading: {}://{}{}", scheme, host, path);

			std::shared_ptr<httplib::Client> cli;

			cli = std::make_shared<httplib::Client>(host);

			cli->set_follow_location(true);
			cli->set_connection_timeout(30);
			cli->set_read_timeout(30);

			auto res = cli->Get(path.c_str());

			if (!res) {
				log::error("Request failed: {}", httplib::to_string(res.error()));
				createQuickPopup(
					"Failed to download:", httplib::to_string(res.error()),
					"OK", nullptr, nullptr
				);
				return nullptr;
			}

			if (res->status != 200) {
				log::error("HTTP error: {}", res->status);
				return nullptr;
			}

			log::info("Downloaded {} bytes", res->body.size());

			auto image = new CCImage();
			if (!image->initWithImageData((void*)res->body.data(), res->body.size())) {
				image->release();
				log::error("Failed to parse image");
				return nullptr;
			}

			auto texture = CCTextureCache::get()->addUIImage(image, name.c_str());
			image->release();

			if (!texture) {
				log::error("Failed to create texture");
				return nullptr;
			}

			auto frame = CCSpriteFrame::createWithTexture(
				texture,
				CCRect(0, 0, texture->getContentSize().width, texture->getContentSize().height)
			);

			if (frame) {
				frame->retain();
				CCSpriteFrameCache::get()->addSpriteFrame(frame, name.c_str());
				log::info("Image loaded successfully!");
				return frame;
			}

			return nullptr;
		}

		return nullptr;
	}
	void trySetupCustomSprite(float = 0.f) {
		if (!this) return;
		if (auto frame = tryGetSpriteFrame()) {
			for (auto c : getChildrenExt()) c->setVisible(false);
			removeChildByTag("image"_h);

			setContentSize({ 30.f, 30.f });

			auto image = CCSprite::createWithSpriteFrame(frame);
			limitNodeSize(image, getContentSize(), 1337.f, 0.0f);

			image->setPosition(this->getContentSize() / 2);
			image->setColor(this->getColor());
			image->setOpacity(this->getOpacity());
			this->addChild(image, 1, "image"_h);
		}
		else {
			for (auto c : getChildrenExt()) c->setVisible(true);
			removeChildByTag("image"_h);
		}
		m_width = getContentWidth();
		m_height = getContentHeight();
		updateOrientedBox();
	}
	static TextGameObject* create(cocos2d::CCTexture2D * texture) {
		auto obj = TextGameObject::create(texture);
		if (obj) {
			obj->m_addToNodeContainer = true;
		}
		return obj;
	}
	void customObjectSetup(gd::vector<gd::string>&p0, gd::vector<void*>&p1) {
		TextGameObject::customObjectSetup(p0, p1);
		trySetupCustomSprite();
	}
	void updateTextObject(gd::string p0, bool p1) {
		if (auto editor = GameManager::get()->m_levelEditorLayer) {
			if (auto ui = editor->m_editorUI)
				if (Ref sel = typeinfo_cast<TextGameObject*>(
					ui->m_selectedObject
				)) {
					if (sel.data() != this) p0 = sel->m_text;
				};
		}
		TextGameObject::updateTextObject(p0, p1);
		if (this) unschedule(schedule_selector(TextGameObjectImageExt::trySetupCustomSprite));
		if (this) scheduleOnce(schedule_selector(TextGameObjectImageExt::trySetupCustomSprite), 0.01f);
	}
};


#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(LevelEditorLayerExt, LevelEditorLayer) {
	void onPlaytest() {
		LevelEditorLayer::onPlaytest();
	}
	virtual void playerTookDamage(PlayerObject * player) {
		player->playDeathEffect();
		LevelEditorLayer::playerTookDamage(player);
	}
};


#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(GJBaseGameLayerExt, GJBaseGameLayer) {
	void resetPlayer() {
		GJBaseGameLayer::resetPlayer();
		for (auto p : { this->m_player1, this->m_player2 }) {
			if (p) p->m_customScaleX = 1.f;
			if (p) p->m_customScaleY = 1.f;
		}
	}
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObjectExt, PlayerObject) {
	bool init(int p0, int p1, GJBaseGameLayer * p2, cocos2d::CCLayer * p3, bool p4) {
		if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

		queueInMainThread(
			[_this = Ref(this), p2 = Ref(p2)] {
				if (!typeinfo_cast<LevelEditorLayer*>(p2.data())) return;
				if (_this == p2->m_player1) _this->addAllParticles();
				if (_this == p2->m_player2) _this->addAllParticles();
			}
		);

		return true;
	}
	void resetPlayerIcon() {
		PlayerObject::resetPlayerIcon();
		m_customScaleY = 1.f;
		m_customScaleX = 1.f;
	}
	void update(float p0) {
		PlayerObject::update(p0 * (m_customScaleY > 0.001 ? m_customScaleY : 1.f));
	}
	void updateRotation(float p0) {
		PlayerObject::updateRotation(p0 * (m_customScaleX > 0.001 ? m_customScaleX : 1.f));
	}
};
