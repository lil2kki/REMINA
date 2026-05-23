#include <.hpp>

#include <user95401.game-objects-factory/include/main.hpp>
#include <user95401.game-objects-factory/include/impl.hpp>

// DIALOGUE TRIGGER EXTENSIONS

#include <Geode/modify/CCActionInterval.hpp>
class $modify(DialogTextAnimExt, CCActionInterval) {
	$override void startWithTarget(CCNode * p0) {
		//log::debug("{}->{}({})", this, __FUNCTION__, p0);
		//CCFadeIn, ::startWithTarget({ CCFontSprite, 
		if (typeinfo_cast<CCFadeIn*>(this)) if (typeinfo_cast<CCFontSprite*>(p0)) {
			Ref fade = typeinfo_cast<CCFadeIn*>(this);
			Ref sprite = typeinfo_cast<CCFontSprite*>(p0);
			if (sprite) sprite->runAction(CCSequence::createWithTwoActions(
				CCDelayTime::create(fade ? fade->getDuration() : 0.1f), CallFuncExt::create(
					[sprite] {
						if (!sprite) return;
						sprite->setVisible(1);
						sprite->setOpacity(255);
						if (not sprite->getContentSize().isZero()) {
							FMODAudioEngine::get()->playEffect("_text.ogg");
						};
					}
				)
			));
			return;
		}
		return CCActionInterval::startWithTarget(p0);
	}
};

#include <Geode/modify/TextArea.hpp>
class $modify(DialogTextAreaExt, TextArea) {
	static inline auto ForceWidth = 0.f;
	static TextArea* create(
		gd::string str, char const* font, float scale,
		float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor
	) {
		return TextArea::create(str, font, scale, ForceWidth ? ForceWidth : width, anchor, lineHeight, disableColor);
	};
};

#include <Geode/modify/DialogLayer.hpp>
class $modify(DialogTrigger, DialogLayer) {

	class Delegate : public DialogDelegate, public CCNode {
	public:
		inline static Delegate* s_pForNextDialogLayer;
		CREATE_FUNC(Delegate);

		Ref<DialogLayer> m_dialogLayer;
		Ref<GJBaseGameLayer> m_game;
		std::string m_replacedTextures = "";
		virtual void dialogClosed(DialogLayer* p0) {
			m_dialogLayer = nullptr;
			if (m_game) m_game->resumeSchedulerAndActions();
			if (m_game) m_game->setKeyboardEnabled(true);
			if (m_game) m_game->setTouchEnabled(true);
			for (auto name : string::split(m_replacedTextures, ",")) {
				CCFileUtils::get()->m_fullPathCache.erase(name.c_str());
				auto result = CCTextureCache::get()->reloadTexture(name.c_str());
			}
		};

		DialogChatPlacement placement = DialogChatPlacement::Center;
		bool hide = false;
		bool no_pause = false;
		bool unskipable = false;
		std::string character = ("");
		int characterFrame = 0;
		bool hadCharacterFrame = false;
		std::optional<GLuint> opacity = std::nullopt;
		;; std::optional<float> scale = std::nullopt;
		;;;;; std::optional<float> px = std::nullopt;
		;;;;; std::optional<float> py = std::nullopt;
		;; std::optional<int> animate = std::nullopt;
	};

	inline static GameObjectsFactory::GameObjectConfig* conf;
	static void setup() {

		GameObjectsFactory::registerGameObject(GameObjectsFactory::createTriggerConfig(
			UNIQ_ID("dialog-trigger"),
			"dialog-trigger.png",
			[](EffectGameObject* trigger, GJBaseGameLayer* game, int p1, gd::vector<int> const* p2)
			{
				if (!trigger) return;
				if (!game) return;

				auto sharedDialogTriggerDelegate = typeinfo_cast<Delegate*>(
					trigger->getUserObject("dialog-delegate")
				);
				if (!sharedDialogTriggerDelegate) return;

				auto DialogTriggerDataNode = typeinfo_cast<CCNode*>(
					trigger->getUserObject("data"_spr)
				);
				if (!DialogTriggerDataNode) return;

				sharedDialogTriggerDelegate->m_game = game;
				auto raw_data = "[" + DialogTriggerDataNode->getID() + "]";
				auto parse = matjson::parse(raw_data);
				matjson::Value data = parse.err()
					? matjson::parse("[ \"<cr>err: " + parse.err().value().message + "</c>\" ]").unwrapOrDefault()
					: parse.unwrapOrDefault();

				auto dialogObjectsArr = CCArrayExt<DialogObject>();

				auto& placement = sharedDialogTriggerDelegate->placement;
				auto& hide = sharedDialogTriggerDelegate->hide;
				auto& no_pause = sharedDialogTriggerDelegate->no_pause;
				auto& unskipable = sharedDialogTriggerDelegate->unskipable;
				auto& character = sharedDialogTriggerDelegate->character;
				auto& characterFrame = sharedDialogTriggerDelegate->characterFrame;
				auto& hadCharacterFrame = sharedDialogTriggerDelegate->hadCharacterFrame;
				auto& opacity = sharedDialogTriggerDelegate->opacity;
				auto& scale = sharedDialogTriggerDelegate->scale;
				auto& px = sharedDialogTriggerDelegate->px;
				auto& py = sharedDialogTriggerDelegate->py;
				auto& animate = sharedDialogTriggerDelegate->animate;

				for (auto& val : data) {
					if (val.isNumber()) {
						characterFrame = val.asInt().unwrapOrDefault();
						hadCharacterFrame = true;
					}
					if (val.isString()) {
						auto text = val.asString().unwrapOrDefault();

						if (string::contains(text, "->")) {
							auto val = string::split(text, "->");
							if (val.size() == 2) if (fileExistsInSearchPaths(val[0].c_str())) { //replace texture
								CCFileUtils::get()->m_fullPathCache.erase(val[0].c_str());
								CCFileUtils::get()->m_fullPathCache.erase(val[1].c_str());
								CCFileUtils::get()->m_fullPathCache[val[0].c_str()] = CCFileUtils::get()->fullPathForFilename(
									val[1].c_str(), 0
								);
								CCTextureCache::get()->reloadTexture(val[0].c_str());
								sharedDialogTriggerDelegate->m_replacedTextures += val[0] + ",";
								continue;
							}
						}

						bool idle = true;
						idle = game->m_player1->m_isOnGround ? idle : false;
						idle = fabs(game->m_player1->m_platformerXVelocity) < 0.01f ? idle : false;
						idle = fabs(game->m_player1->m_yVelocity) < 0.01f ? idle : false;
						if (string::startsWith(text, "!if_idle")) {
							if (!idle) {
								dialogObjectsArr.inner()->removeAllObjects();
								break;
							}
							continue;
						}

						if (string::startsWith(text, "!no_pause")) { no_pause = true; continue; }
						if (string::startsWith(text, "!hide")) { hide = true; continue; }
						if (text == "!") { unskipable = !unskipable; continue; }

						if (auto a = "!op:"; string::startsWith(text, a)) {
							opacity = utils::numFromString<int>(
								string::replace(text, a, "")
							).unwrapOrDefault();
							continue;
						}
						if (auto a = "!s:"; string::startsWith(text, a)) {
							scale = utils::numFromString<float>(
								string::replace(text, a, "")
							).unwrapOrDefault();
							continue;
						}
						if (auto a = "!px:"; string::startsWith(text, a)) {
							px = utils::numFromString<float>(
								string::replace(text, a, "")
							).unwrapOrDefault();
							continue;
						}
						if (auto a = "!py:"; string::startsWith(text, a)) {
							py = utils::numFromString<float>(
								string::replace(text, a, "")
							).unwrapOrDefault();
							continue;
						}
						if (auto a = "!anim:"; string::startsWith(text, a)) {
							animate = utils::numFromString<int>(
								string::replace(text, a, "")
							).unwrapOrDefault();
							continue;
						}

						text = string::replace(text, "!place:", "!p:");
						if (string::startsWith(text, "!p:")) {
							auto place = string::replace(text, "!p:", "");
							if (place == "t") placement = DialogChatPlacement::Top;
							if (place == "c") placement = DialogChatPlacement::Center;
							if (place == "b") placement = DialogChatPlacement::Bottom;
							continue;
						}

						text = string::replace(text, "!char:", "!c:");
						if (string::startsWith(text, "!c:")) {
							character = string::replace(text, "!c:", "");
							continue;
						}

						dialogObjectsArr.push_back(DialogObject::create(
							character, text, characterFrame, 1.f, unskipable, ccWHITE
						));
					}
				}

				if (false) log::debug("placement {}", static_cast<int>(placement));

				auto& dialog = sharedDialogTriggerDelegate->m_dialogLayer;
				if (dialogObjectsArr.size()) {
					if (dialog) dialog->removeFromParent();
					Delegate::s_pForNextDialogLayer = sharedDialogTriggerDelegate;
					dialog = DialogLayer::createDialogLayer(
						dialogObjectsArr[0], dialogObjectsArr.inner(), 1
					);
					dialog->updateChatPlacement(placement);
					if (animate.has_value()) dialog->animateIn(
						(DialogAnimationType)animate.value()
					); else dialog->animateInRandomSide();
					if (game and game->isRunning()) {
						auto scene = CCDirector::get()->m_pRunningScene;
						CCDirector::get()->m_pRunningScene = (CCScene*)game->m_uiLayer;
						CCDirector::get()->m_pRunningScene->setVisible(1);
						dialog->addToMainScene();
						CCDirector::get()->m_pRunningScene = scene;
					}
					dialog->runAction(CCRepeatForever::create(CCSequence::create(CallFuncExt::create(
						[dialog = Ref(dialog), l = Ref(dialog->m_mainLayer), opacity, scale, px, py]() {
							auto someSprite = l->getChildByType<CCSprite*>(0); //2 is icon, 3 is continue mark
							auto hasIcon = !someSprite ? false : someSprite->getZOrder() == 2;
							if (Ref a = l->getChildByType<TextArea*>(0)) {
								a->setPositionX(hasIcon ? -92.000f : -174.000f);
							}
							if (Ref a = l->getChildByType<CCLabelBMFont*>(0)) {
								a->setPositionX(hasIcon ? -93.000f : -176.000f);
							}

							if (Ref a = l->getChildByType<CCScale9Sprite*>(1)) a->setOpacity(0);

							if (opacity.has_value()) dialog->setOpacity(opacity.value());
							if (scale.has_value()) dialog->m_mainLayer->setScale(scale.value());

							auto ptmp = dialog->m_mainLayer->getAnchorPoint();
							if (px.has_value()) ptmp.x = px.value();
							if (py.has_value()) ptmp.y = py.value();
							dialog->m_mainLayer->setAnchorPoint(ptmp);
						}
					), nullptr)));

					if (game and not no_pause) {
						game->setKeyboardEnabled(false);
						game->setTouchEnabled(false);
						game->pauseSchedulerAndActions();
					}
				}
				if (dialog and hide) dialog->removeFromParent();

			},
			[](EditTriggersPopup* popup, EffectGameObject* trigger, CCArray* objects)
			{
				if (!popup) return;
				if (!trigger) return;
				if (!objects) return;
				if (auto data = typeinfo_cast<CCNode*>(trigger->getUserObject("data"_spr))) {
					if (auto title = popup->getChildByType<CCLabelBMFont*>(0)) {
						title->setString("");
						title->setAnchorPoint(CCPointMake(0.5f, 0.3f));
					}
					if (auto inf = popup->m_buttonMenu->getChildByType<InfoAlertButton*>(0)) {
						inf->setVisible(false);
					}
					if (auto a = popup->m_mainLayer->getChildByType<CCScale9Sprite*>(0)) {
						a->setAnchorPoint(CCPointMake(0.5f, 0.900f));
					}

					auto input = TextInput::create(422.f, "Dialog data string...", "chatFont.fnt");
					input->setFilter(" !\"#$ * &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
					input->getInputNode()->m_allowedChars = " !\"#$ * &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
					input->getInputNode()->m_textLabel->setWidth(input->getContentWidth() - 28.f);
					input->setString(data->getID());
					input->setPositionY(174.000f);
					input->getBGSprite()->setContentHeight(350.000f);
					input->setCallback(
						[data = Ref(data)](const std::string& p0) {
							data->setID(p0);
						}
					);
					popup->m_buttonMenu->addChild(input);
					popup->m_mainLayer->setPositionY(-92.000f);

					auto editor = CCMenuItemExt::createSpriteExtra(
						ButtonSprite::create("String Guide"), [input = Ref(input)](CCMenuItem*) {
							web::openLinkInBrowser("https://github.com/lil2kki/REMINA/wiki/Dialog-Trigger-String-Guide");
						}
					);
					editor->setPosition(CCPointMake(100.f, 28.f));
					editor->setScale(0.55f);
					editor->m_baseScale = (0.55f);
					editor->m_scaleMultiplier = (1.0f + (0.61f - 0.55f));
					popup->m_buttonMenu->addChild(editor);

					auto Run = CCMenuItemExt::createSpriteExtra(
						ButtonSprite::create("Run"), [trigger = Ref(trigger)](CCMenuItem*) {
							trigger->triggerObject(GameManager::get()->m_gameLayer, 0, nullptr);
						}
					);
					Run->setPosition(CCPointMake(144.f, 64.f));
					popup->m_buttonMenu->addChild(Run);
				}
			}
		)->customSetup(
			[](GameObject* object)
			{
				if (!object) return object;
				object->m_addToNodeContainer = true;
				object->setUserObject("dialog-delegate", Delegate::create());
				auto data = CCNode::create();
				object->setUserObject("data"_spr, data);
				object->m_objectType = GameObjectType::CustomRing;
				object->m_hasNoEffects = true;
				((RingObject*)object)->RingObject::m_claimTouch = true; //evil
				return object;
			}
		)->saveString(
			[](std::string str, GameObject* object, GJBaseGameLayer* level)
			{
				if (!object) return gd::string(str.c_str());
				if (!level) return gd::string(str.c_str());
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
				if (data) {
					data->setID(ZipUtils::base64URLDecode(p0[228].c_str()).c_str());
				};

				return object;
			}
		)->activatedByPlayer(
			[](EnhancedGameObject* asd, PlayerObject* lsd) {
				asd->triggerObject(lsd->m_gameLayer, 0, nullptr);
			}
		));

	};
	static void onModify(auto&) { setup(); }

	void skip(bool close = false) {
		close ? queueInMainThread(
			[xd = Ref(this)] { if (xd) xd->onClose(); }
		) : queueInMainThread(
			[xd = Ref(this)] { if (xd) xd->handleDialogTap(); }
		);
	};

	bool processDialogObject(DialogObject * object) {

		Ref del = typeinfo_cast<Delegate*>(m_delegate);
		if (!del) return false;

		if (!del->m_game) return false;

		auto& placement = del->placement;
		auto& hide = del->hide;
		auto& no_pause = del->no_pause;
		auto& unskipable = del->unskipable;
		auto& character = del->character;
		auto& characterFrame = del->characterFrame;
		auto& hadCharacterFrame = del->hadCharacterFrame;
		auto& opacity = del->opacity;
		auto& scale = del->scale;
		auto& px = del->px;
		auto& py = del->py;
		auto& animate = del->animate;

		std::string text = object->m_text.c_str();
		//log::warn("{}", text.c_str());

		if (string::contains(text, "->")) {
			auto val = string::split(text, "->");
			if (val.size() == 2) if (fileExistsInSearchPaths(val[0].c_str())) { //replace texture
				CCFileUtils::get()->m_fullPathCache.erase(val[0].c_str());
				CCFileUtils::get()->m_fullPathCache.erase(val[1].c_str());
				CCFileUtils::get()->m_fullPathCache[val[0].c_str()] = CCFileUtils::get()->fullPathForFilename(
					val[1].c_str(), 0
				);
				CCTextureCache::get()->reloadTexture(val[0].c_str());
				del->m_replacedTextures += val[0] + ",";
				return true;
			}
		}

		if (string::startsWith(text, "!exit")) {
			Ref playlayer = typeinfo_cast<PlayLayer*>(del->m_game.data());
			if (playlayer and playlayer->isRunning()) {
				playlayer->pauseGame(0);
				CCScene::get()->getChildByType<PauseLayer>(0)->onQuit(0);
			}
			return true;
		}

		if (auto a = "!activate:"; string::startsWith(text, a)) {
			auto id = utils::numFromString<int>(string::replace(text, a, "")).unwrapOrDefault();
			if (del->m_game) del->m_game->spawnGroup(id, false, 0, gd::vector<int>(), -1, -1);
			return true;
		}
		if (auto a = "!toggle:"; string::startsWith(text, a)) {
			auto id = utils::numFromString<float>(string::replace(text, a, "")).unwrapOrDefault();
			if (del->m_game) del->m_game->toggleGroup(id, id > (int)id); //123,0 is on and 123,1 is off
			return true;
		}

		if (auto a = "!ntfy:"; string::startsWith(text, a)) {
			auto args = string::split(string::replace(text, a, ""), "//");

			auto str = args[0];
			auto icon = NotificationIcon::None;
			auto sprite = (CCSprite*)nullptr;
			auto time = NOTIFICATION_DEFAULT_TIME;

			if (args.size() > 1) {
				auto id = utils::numFromString<int>(args[1]);
				if (id.isOk()) icon = (NotificationIcon)id.unwrapOrDefault();
				else {
					sprite = CCSprite::createWithSpriteFrameName(args[1].c_str());
					if (auto a = CCSprite::create(args[1].c_str())) sprite = a;
				}
			}
			if (args.size() > 2) time = utils::numFromString<float>(
				args[2]
			).unwrapOrDefault();

			if (sprite) Notification::create(str, sprite, time)->show();
			else Notification::create(str, icon, time)->show();

			return true;
		}

		if (auto a = "!popup:"; string::startsWith(text, a)) {
			auto args = string::split(string::replace(text, a, ""), "//");

			auto title = args[0];
			auto cap = std::string("");
			auto btn1 = std::string("OK");
			auto btn2 = std::string();
			auto group1 = 0;
			auto group2 = 0;

			if (args.size() > 1) cap = args[1];
			if (args.size() > 2) {
				auto spl = string::split(args[2], "->");
				btn1 = spl[0];
				if (spl.size() > 1) group1 = utils::numFromString<int>(spl[1]).unwrapOrDefault();
			}
			if (args.size() > 3) {
				auto spl = string::split(args[3], "->");
				btn2 = spl[0];
				if (spl.size() > 1) group2 = utils::numFromString<int>(spl[1]).unwrapOrDefault();
			}

			if (del->m_game and del->m_game->isRunning()) {
				auto scene = CCDirector::get()->m_pRunningScene;
				CCDirector::get()->m_pRunningScene = (CCScene*)del->m_game->m_uiLayer;
				CCDirector::get()->m_pRunningScene->setVisible(1);
				if (not no_pause) {
					del->m_game->setKeyboardEnabled(false);
					del->m_game->setTouchEnabled(false);
					del->m_game->pauseSchedulerAndActions();
				}
				auto popup = createQuickPopup(
					title.c_str(), cap.c_str(),
					btn1.empty() ? nullptr : btn1.c_str(),
					btn2.empty() ? nullptr : btn2.c_str(),
					[_ = Ref(this), object = Ref(object),
					game = Ref(del->m_game), del = Ref(del),
					group1, group2](CCNode* a, bool btn2) {
						del->dialogClosed(nullptr);
						if (game) game->spawnGroup(
							btn2 ? group2 : group1, false, 0, gd::vector<int>(), -1, -1
						);
						if (_) {
							a->getParent()->addChild(_);
							_->setUserObject("call-org-display", object);
							_->skip();
						}
					}
				);
				CCDirector::get()->m_pRunningScene = scene;

				if (placement != DialogChatPlacement::Center) {
					popup->m_mainLayer->ignoreAnchorPointForPosition(false);
					popup->m_mainLayer->setAnchorPoint(
						{ 0.f, [](DialogChatPlacement placement) -> float {
							if (placement == DialogChatPlacement::Top) return -0.250f;
							if (placement == DialogChatPlacement::Bottom) return 0.250f;
							return 0.f;
						}(placement) }
					);
				}
				if (opacity.has_value()) popup->setOpacity(opacity.value());
				if (scale.has_value()) popup->setScale(scale.value());

				auto ptmp = popup->m_mainLayer->getAnchorPoint();
				if (px.has_value()) ptmp.x = px.value();
				if (py.has_value()) ptmp.y = py.value();
				popup->m_mainLayer->setAnchorPoint(ptmp);

			}

			setUserObject("dont-skip", object);
			return true;
		}

		if (string::startsWith(text, "!close")) {
			queueInMainThread([xd = Ref(this)] { xd->onClose(); });
		}
		if (string::startsWith(text, "!tap")) {
			queueInMainThread([xd = Ref(this)] { xd->handleDialogTap(); });
		}

		return false;
	}

	void displayDialogObject(DialogObject * object) {
		if (typeinfo_cast<Delegate*>(m_delegate)) DialogTextAreaExt::ForceWidth = 1200.f;
		if (getUserObject("call-org-display") == object) {
			return DialogLayer::displayDialogObject(object);
		}
		if (processDialogObject(object)) {
			if (getUserObject("dont-skip") == object) queueInMainThread(
				[xd = Ref(this)] { xd->removeFromParentAndCleanup(false); }
			); else skip();
		}
		else {
			DialogLayer::displayDialogObject(object);
		}
		DialogTextAreaExt::ForceWidth = false;
	};

	bool init(DialogObject * object, cocos2d::CCArray * objects, int background) {
		m_delegate = m_delegate ? m_delegate : Delegate::s_pForNextDialogLayer;
		if (!DialogLayer::init(object, objects, background)) return false;
		if (this) this->runAction(CCSequence::createWithTwoActions(
			CCDelayTime::create(0.1f), CallFuncExt::create(
				[_this = Ref(this)] { _this->m_handleTap = (1); }
			)));
		return true;
	};

	void displayNextObject_() {};

};