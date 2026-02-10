#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <alphalaneous.alphas_geode_utils/include/Utils.h>
namespace geode::сocos { using namespace AlphaUtils::Cocos; }

#include <Geode/modify/CCSpriteFrameCache.hpp>
class $modify(CCSpriteFrameCache) {
	void addSpriteFramesWithFile(const char* plist) {
		if (string::contains(plist, Mod::get()->getID())) {
			auto path = CCFileUtils::get()->fullPathForFilename(plist, 0);
			if (!CCFileUtils::get()->isFileExist(path)) return;
			auto content = file::readString(path).unwrapOrDefault();
			content = string::replace(content, "..", "/");
			content = string::replace(content, Mod::get()->getID() + "/", "");
			file::writeStringSafe(path, content).isOk();
			//fuuuck
			removeSpriteFramesFromFile(plist);
		}
		CCSpriteFrameCache::addSpriteFramesWithFile(plist);
	}
};

#include <Geode/modify/CCString.hpp>
class $modify(CCString) {
	const char* getCString() {
		//log::debug("{}(int:{})->{}", this, (int)this, __func__);
		if ((int)this == 0) log::error("{}(int:{})->{}", this, (int)this, __func__);
		return (int)this != 0 ? CCString::getCString() : CCString::createWithFormat("")->getCString();
	}
};

#include <Geode/modify/CCNode.hpp>
class $modify(NodeVisitController, CCNode) {
	auto replaceColors(float = 0.f) {
		if (Ref node = typeinfo_cast<GJListLayer*>(this)) {
			if (node->getOpacity() == 180) node->setOpacity(255);// list-bg
		}
		if (Ref<SetupTriggerPopup> a = typeinfo_cast<UIOptionsLayer*>(this); a = a ? a : typeinfo_cast<UIPOptionsLayer*>(this)) {
			a->setOpacity(160);
			a->setColor(ccBLACK);
		}
		/*Ref game = GameManager::get()->m_gameLayer;
		if (game and game->isRunning()) return;*/
		auto hexcfix = [] (std::string hex) { return std::string(hex.begin() + 1, hex.begin() + 7); };
#define repl(org, tar) if (node->getColor() == cc3bFromHexString(hexcfix(org)).unwrapOr(ccWHITE)) node->setColor(cc3bFromHexString(hexcfix(tar)).unwrapOr(ccWHITE));
		if (auto node = typeinfo_cast<CCRGBAProtocol*>(this)) {
			repl("#0066FFFF", "#686868FF");//background
			repl("#334499FF", "#161218FF");//chest_glow_bg_001.png
			repl("#00388DFF", "#272727FF");//level-search-bg
			repl("#002762FF", "#000000FF");//level-search-bar-bg
			repl("#002E75FF", "#000000FF");//quick-search-bg
			repl("#00245BFF", "#000000FF");//difficulty-filters-bg
			repl("#001F4FFF", "#121212FF");//length-filters-bg
			repl("#824021FF", "#362424FF");//desc-background
			repl("#203182FF", "#BDB0B0FF");//GJCommentListLayer outline
			repl("#824020FF", "#BDB0B0FF");//GJCommentListLayer outline
			repl("#203182FF", "#37373764");//CommentCell CCScale9Sprite
			repl("#002E73FF", "#000000FF");//play-button1
			repl("#0057DAFF", "#000000FF");//play-button11
			repl("#002E73FF", "#000000FF");//play-button111
			repl("#BF723EFF", "#171717FF");//GJListLayer
			repl("#A1582CFF", "#1F1F1FFF");//LevelCell1
			repl("#C2723EFF", "#231F1FFF");//LevelCell2
			repl("#6C99D8FF", "#7A7A7AFF");//inputplaceholder
		};
		if (Ref node = typeinfo_cast<CCLabelBMFont*>(this)) {
			repl("#6C99D8FF", "#7A7A7AFF");//inputplaceholder
			repl("#78AAF0FF", "#7A7A7AFF");//inputplaceholder-LevelListLayer
		}
		if (Ref node = typeinfo_cast<CCLayerColor*>(this)) {
			repl("#BF723EFF", "#171717FF");//GJListLayer
			repl("#A1582CFF", "#1F1F1FFF");//LevelCell1
			repl("#C2723EFF", "#231F1FFF");//LevelCell2
			//repl("", "");
		};
#undef repl
	}
	$override void addChild(CCNode * child, int zOrder, int tag) {
		if (!child) return;
		CCNode::addChild(child, zOrder, tag);
		Ref((NodeVisitController*)this)->replaceColors();
		Ref((NodeVisitController*)child)->replaceColors();
		if (Ref node = typeinfo_cast<CCLabelBMFont*>(this)) {
			node->schedule(schedule_selector(NodeVisitController::replaceColors), 0.f);
			if (Ref ch = typeinfo_cast<CCFontSprite*>(child)) queueInMainThread(
				[ch]
				{
					auto orgc = ch->getColor();
					auto lght = lighten3B(orgc, 80);
					if (ch->getColor() != ccWHITE) ch->runAction(CCRepeatForever::create(
#define roll_2893450 CCEaseSineInOut::create(CCTintTo::create(\
					1.f + CCRANDOM_0_1(),\
						lght.r, lght.g, lght.b\
						)),\
						CCEaseSineInOut::create(CCTintTo::create(\
							1.f + CCRANDOM_0_1(),\
							orgc.r, orgc.g, orgc.b\
						)),
						CCSequence::create(roll_2893450 roll_2893450 roll_2893450 nullptr)
					));
					if (ch->getColor() != ccWHITE) ch->setBlendFunc({GL_SRC_ALPHA, GL_ONE}); //additive
				}
			);
		}
	}
	$override void visit() {
		Ref(this)->replaceColors();
		CCNode::visit();
	}
};

#include <Geode/modify/CCFileUtils.hpp>
class $modify(CCFileUtilsResourcesExt, CCFileUtils) {
	inline static std::unordered_set<std::string> s_checkedDirs;
	inline static std::unordered_map<std::string, std::vector<std::string>> s_randomDirs;

	virtual gd::string fullPathForFilename(const char* pszFileName, bool skipSuffix) {
		
		std::string_view fileName(pszFileName);

		auto lastDot = fileName.find_last_of('.');
		if (lastDot != std::string_view::npos && lastDot > 2) {
			std::string noExtName(fileName.substr(0, lastDot));

			// Проверяем кеш: уже искали эту директорию?
			auto it = s_randomDirs.find(noExtName);
			if (it != s_randomDirs.end()) {
				// Директория найдена - берём случайный файл
				if (!it->second.empty()) {
					size_t idx = std::rand() % it->second.size();
					return CCFileUtils::fullPathForFilename(it->second[idx].c_str(), skipSuffix); // или верни it->second[idx] если нужен прямой путь
				}
			}
			else if (s_checkedDirs.find(noExtName) == s_checkedDirs.end()) {
				// Ещё не проверяли - ищем ОДИН РАЗ
				s_checkedDirs.insert(noExtName);

				auto& searchPaths = getSearchPaths();
				for (auto& p : searchPaths) {
					std::string dirPath = p + noExtName + "/";
					auto varList = file::readDirectory(dirPath.c_str()).unwrapOrDefault();

					if (!varList.empty()) {
						// Сохраняем в кеш
						auto& cached = s_randomDirs[noExtName];
						cached.reserve(varList.size());
						for (auto& entry : varList) {
							cached.push_back(entry.string());
						}

						log::debug("{} -> {} files", noExtName, cached.size());
						size_t idx = std::rand() % cached.size();
						return CCFileUtils::fullPathForFilename(cached[idx].c_str(), skipSuffix); // заменить на правильный путь если нужно
					}
				}
			}
		}

		if (fileName.find('/') != std::string_view::npos) {
			std::string noSub;
			noSub.reserve(fileName.size() * 2);

			for (char ch : fileName) {
				if (ch == '/') noSub += "..";
				else noSub += ch;
			}

			if (fileExistsInSearchPaths(noSub.c_str())) {
				auto* texCache = CCTextureCache::get();
				if (auto* tex = texCache->textureForKey(noSub.c_str())) {
					texCache->reloadTexture(noSub.c_str());
				}
				return CCFileUtils::fullPathForFilename(noSub.c_str(), skipSuffix);
			}
		}

		return CCFileUtils::fullPathForFilename(pszFileName, skipSuffix);
	}
};

#include <Geode/modify/CCSprite.hpp>
class $modify(CCSpriteExt, CCSprite) {
	void updateShader(float dt) {
		static float time = 0.0f;
		time += dt;
		if (auto* program = this->getShaderProgram()) {
			program->use();
			GLuint timeUniform = glGetUniformLocation(program->getProgram(), "u_time");
			if (timeUniform != GL_INVALID_INDEX) {
				glUniform1f(timeUniform, time);
			}
		}
	}

	static auto tryApplyShader(CCSprite * spr, std::string pszName) {
		if (!spr) {
			//log::warn("sprite from {} is {}", pszName, spr);
			return spr;
		}

		bool isAvailabilityCheck = ((int)spr == "isAvailable"_h);

		std::string path = CCFileUtils::get()->fullPathForFilename(pszName.c_str(), 0);
		if (!string::contains(path, ".png")) {
			path = CCFileUtils::get()->fullPathForFilename((pszName + ".fsh").c_str(), 0);
		}
		else {
			path += ".fsh";
		}

		bool shaderExists = CCFileUtils::get()->isFileExist(path);

		if (isAvailabilityCheck) return (CCSprite*)shaderExists;

		if (shaderExists) {
			Ref<CCGLProgram> program = new CCGLProgram();
			if (program->initWithVertexShaderFilename(
				(pszName + ".vsh").c_str(),
				(pszName + ".fsh").c_str()
			)) {
				program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
				program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
				program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
				program->link();
				program->updateUniforms();
				spr->setShaderProgram(program);
				spr->schedule(schedule_selector(CCSpriteExt::updateShader));
			}
		}

		return spr;
	}

	static CCSprite* create(const char* pszName) {
		auto* spr = CCSprite::create(pszName);

		if (!spr) {
			auto lastDot = std::string(pszName).find_last_of('.');
			if (lastDot != std::string::npos) {
				std::string name = std::string(pszName).substr(0, lastDot);
				std::string ext = std::string(pszName).substr(lastDot + 1);
				spr = CCSprite::create(fmt::format("{}-FALLBACK.{}", name, ext).c_str());
			}
		}

		spr = tryApplyShader(spr, pszName);

		return spr;
	}

	static CCSprite* createWithSpriteFrameName(const char* pszName) {
		if (strcmp(pszName, "GJ_logo_001.png") == 0) {
			if (auto* spr = CCSprite::create(pszName)) {
				queueInMainThread([spr = Ref(spr)] {
					if (spr) limitNodeHeight(spr, 44.750f, 99.f, 0.1f);
					});
				return spr;
			}
		}

		auto* spr = CCSprite::createWithSpriteFrameName(pszName);

		/*if (auto xd = AlphaUtils::Cocos::getSprite(pszName).value_or(nullptr)) {
			spr = xd;
		}*/

		if (!spr) {
			auto lastDot = std::string(pszName).find_last_of('.');
			if (lastDot != std::string::npos) {
				std::string name = std::string(pszName).substr(0, lastDot);
				std::string ext = std::string(pszName).substr(lastDot + 1);
				spr = CCSprite::createWithSpriteFrameName(
					fmt::format("{}-FALLBACK.{}", name, ext).c_str()
				);
			}
		}

		//spr = tryApplyShader(spr, pszName);

		return spr;
	}
};

#include <Geode/modify/CCLayer.hpp>
class $modify(CCLayerExt, CCLayer) {
	virtual void onEnter() {
		CCLayer::onEnter();

		if (!CCSpriteExt::tryApplyShader(
			(CCSprite*)"isAvailable"_h,
			сocos::getClassName(this)
		)) {
			return;
		}

		auto contentSize = this->getContentSize();
		Ref<CCRenderTexture> render = CCRenderTexture::create(
			contentSize.width,
			contentSize.height
		);

		if (!render) return;

		render->beginWithClear(0, 0, 0, 0);
		this->visit();
		render->end();

		auto* sprite = render->getSprite();
		sprite->setPosition(contentSize / 2);
		this->addChild(sprite, INT_MAX, 0);

		CCSpriteExt::tryApplyShader(sprite, сocos::getClassName(this));

		this->runAction(CCRepeatForever::create(
			CCSequence::create(
				CallFuncExt::create([render = Ref(render), layer = Ref(this)] {
					auto* spr = render->getSprite();
					spr->setVisible(false);
					render->beginWithClear(0, 0, 0, 0);
					layer->visit();
					render->end();
					spr->setVisible(true);
					}),
				nullptr
			)
		));
	}
};

//deps and dodeps
#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
	static auto onModify(auto) {
		//texture pack
		CCTexturePack xd;
		xd.m_id = Mod::get()->getID();
		xd.m_paths.push_back(R"(C:\Users\user95401\source\repos\REMINA\resources\)");
		xd.m_paths.push_back(string::pathToString(Mod::get()->getResourcesDir().parent_path()).c_str());
		xd.m_paths.push_back(string::pathToString(Mod::get()->getResourcesDir()).c_str());
		log::debug("Adding texture pack: \"{}\"", xd.m_paths[0]);
		CCFileUtils::get()->addTexturePack(xd);
		//add sub files
		for (auto path : file::readDirectory(getMod()->getResourcesDir()).unwrapOrDefault()) {
			auto str = string::pathToString(path);
			auto name = string::pathToString(path.filename());
			auto nsub = string::replace(name, "..", "/");
			if (string::contains(str, "..")) CCFileUtils::get()->m_fullPathCache[nsub] = CCFileUtils::get()->fullPathForFilename(
				name.c_str(), 0
			);
		}
	}
	static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
		if (!isVideoOptionsOpen) {

			static int notJustLaunched = false;
			if (notJustLaunched++) return MenuLayer::scene(isVideoOptionsOpen);
			FMODAudioEngine::get()->playEffect("achievement_01.ogg");

			auto issues = std::vector<std::string>();
			for (auto dep : getMod()->getMetadataRef().getDependencies()) {
				if (not Loader::get()->isModLoaded(dep.id)) {
					issues.push_back(dep.id);
				}
			};
			if (!issues.size()) return MenuLayer::scene(isVideoOptionsOpen);

			GameManager::get()->fadeInMusic(".aw");

			auto stream = std::stringstream() <<
				"Make sure that "
				"the following mods will be loaded "
				"in order to play "
				<< fmt::format(
					"[{}](mod:{})",
					getMod()->getID(), getMod()->getID()
				) << ":\n\n";
			for (auto dep : issues) {
				stream << "- " << fmt::format("[{}](mod:{})\n", dep, dep);
			}
			//popup
			Ref popup = MDPopup::create(
				"THE DEPENDENCIES...", stream.str(), "Restart", nullptr, [](bool) {
					game::restart(true);
				}
			);
			//sets
			popup->setOpacity(0);
			if (Ref a = popup->m_mainLayer->getChildByType<CCScale9Sprite>(0))
				a->setOpacity(0);
			//bg
			Ref bg = geode::createLayerBG();
			bg->setColor(ccWHITE);
			popup->addChild(bg, -1);
			//glitterEffect
			for (auto a : { 1,2,3,4,5 }) {
				popup->addChildAtPosition(
					CCParticleSystemQuad::create("glitterEffect.plist", 0)
					, Anchor::Center, {}, !"NO LAYOUT"
				);
				popup->getChildByType<CCParticleSystemQuad>(
					-1
				)->setScaleX(1.0f + (3.f / CCRANDOM_0_1()));
			}
			//addSideArt
			addSideArt(popup);
			//mouse particle
			Ref sc = CCScene::create();
			sc->addChild(popup, 1, "popup"_h);
			sc->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
				CCDelayTime::create(.01f), CallFuncExt::create(
					[sc, bg, popup] {
						auto particle = sc->getChildByTag("mouse-particle"_h);
						if (!particle) {
							particle = CCParticleSystemQuad::create("keyEffect.plist", 0);
							particle->setScale(41.150f, 0.325f);
							sc->addChild(particle, INT_MAX, "mouse-particle"_h);
						}
						particle->setPosition(getMousePos());
						if (!sc->getChildByTag("popup"_h)) game::restart(true);
						//bg
						auto color = CCRANDOM_0_1();
						color = color < .7f ? .7f : color;
						bg->setColor(ccc3(color * 120, color * 120, color * 120));
						//
						popup->getChildByType<CCParticleSystemQuad>(-1)->setScaleX(1.0f + (3.f / CCRANDOM_0_1()));
						popup->getChildByType<CCParticleSystemQuad>(-2)->setScaleX(1.0f + (2.f / CCRANDOM_0_1()));
					}
				)
			)));
			//xd
			CCScheduler::get()->setTimeScale(40.f);
			return sc;
		} //!isVideoOptionsOpen
		return MenuLayer::scene(isVideoOptionsOpen);
	};
};