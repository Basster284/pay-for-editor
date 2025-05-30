#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

bool m_openedWarning = false;
bool isEditorAvaliable = false;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) 
            return false;

        if (!m_openedWarning) {
            auto alert = FLAlertLayer::create(
                "Warning",
                "This is 'pay for editor' mod. By default, you will not lose mana orbs when you buying an editor, "
                "but you can turn it off in mod settings.",
                "ok"
            );
            alert->m_scene = this;
            alert->show();
            m_openedWarning = true;
        }
        return true;
    }
};

class $modify(Layer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) 
            return false;

            auto menu = this->getChildByID("creator-buttons-menu");
            auto creator_button = this->getChildByIDRecursive("create-button");
            
            if (creator_button) {
                if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(creator_button)) {
                    if (auto sprite = typeinfo_cast<CCSprite*>(button->getNormalImage())) {
                        sprite->setColor({128, 128, 128}); 
                    }
                }
                else if (auto sprite = typeinfo_cast<CCSprite*>(creator_button)) {
                    sprite->setColor({128, 128, 128});
                }
                
                auto lock = CCMenuItemSpriteExtra::create(
                    CCSprite::createWithSpriteFrameName("GJLargeLock_001.png"),
                    this,
                    menu_selector(Layer::onMyLevels)
                );
                lock->setID("lock-button");
                lock->setPosition(40, 40);
                lock->setScale(0.500);
                creator_button->addChild(lock);
                
                auto priceLabel = CCLabelBMFont::create("10,000", "bigFont.fnt");
                priceLabel->setScale(0.3f);
                priceLabel->setColor({0, 255, 255});
                priceLabel->setPosition(39.500, 17.500);
                priceLabel->setID("price-label");
                creator_button->addChild(priceLabel);
                
                if (isEditorAvaliable) {
                    creator_button->removeChildByID("lock-button");
                    creator_button->removeChildByID("price-label");
                    
                    if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(creator_button)) {
                        if (auto sprite = typeinfo_cast<CCSprite*>(button->getNormalImage())) {
                            sprite->setColor({255, 255, 255}); 
                        }
                    }
                    else if (auto sprite = typeinfo_cast<CCSprite*>(creator_button)) {
                        sprite->setColor({255, 255, 255});
                    }
                }
            }
        
        return true;
    }

    void onMyLevels(CCObject*) {
        if (isEditorAvaliable) {
            auto searchObj = GJSearchObject::create(SearchType::MyLevels);
            auto layer = LevelBrowserLayer::scene(searchObj);
            auto transition = CCTransitionFade::create(0.5f, layer);
            CCDirector::sharedDirector()->pushScene(transition);
            return;
        }
		else{
			geode::createQuickPopup(
            "Editor",
            "Do you want to <cg>buy</c> this <co>Editor</c> for <cy>10,000</c> <cb>Mana Orbs?</c>",
            "No", 
            "Yes",
            [this](auto, bool btn2) {
                if (btn2) {
                    isEditorAvaliable = true;
                    
                    auto searchObj = GJSearchObject::create(SearchType::MyLevels);
                    auto layer = LevelBrowserLayer::scene(searchObj);
                    auto transition = CCTransitionFade::create(0.5f, layer);
                    CCDirector::sharedDirector()->pushScene(transition);
                }
            }
        );
		}
        
        
    }
};