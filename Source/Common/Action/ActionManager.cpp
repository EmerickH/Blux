/*
  ==============================================================================

    ActionManager.cpp
    Created: 12 Oct 2020 10:55:26am
    Author:  bkupe

  ==============================================================================
*/

#include "ActionManager.h"

#include "Scene/actions/SceneAction.h"
#include "Object/actions/ObjectAction.h"
#include "Effect/actions/EffectAction.h"
#include "Sequence/actions/SequenceAction.h"
#include "Engine/GenericAction.h"

juce_ImplementSingleton(ActionFactory);

ActionFactory::ActionFactory()
{
    defs.add(Factory<Action>::Definition::createDef("Scene", "Load Scene", &SceneAction::create)->addParam("actionType", SceneAction::LOAD_SCENE));
    defs.add(Factory<Action>::Definition::createDef("Scene", "Load Next Scene", &SceneAction::create)->addParam("actionType", SceneAction::LOAD_NEXT_SCENE));

    defs.add(Factory<Action>::Definition::createDef("Effect", "Enable Effect", &EffectAction::create)->addParam("actionType", EffectAction::ENABLE_EFFECT));
    defs.add(Factory<Action>::Definition::createDef("Effect", "Disable Effect", &EffectAction::create)->addParam("actionType", EffectAction::DISABLE_EFFECT));
    defs.add(Factory<Action>::Definition::createDef("Effect", "Set Effect Weight", &EffectAction::create)->addParam("actionType", EffectAction::SET_EFFECT_WEIGHT));

    defs.add(Factory<Action>::Definition::createDef("Sequence", "Play Sequence", &SequenceAction::create)->addParam("actionType", SequenceAction::PLAY_SEQUENCE));
    defs.add(Factory<Action>::Definition::createDef("Sequence", "Stop Sequence", &SequenceAction::create)->addParam("actionType", SequenceAction::STOP_SEQUENCE));

    defs.add(Factory<Action>::Definition::createDef("Generic", "Set Parameter Value", &GenericAction::create)->addParam("actionType", GenericAction::SET_VALUE));
    defs.add(Factory<Action>::Definition::createDef("Generic", "Trigger a control", &GenericAction::create)->addParam("actionType", GenericAction::TRIGGER));

}

ActionManager::ActionManager() :
    BaseManager("Actions")
{
    managerFactory = ActionFactory::getInstance();
}

ActionManager::~ActionManager()
{
}

void ActionManager::triggerAll()
{
    for (auto& i : items) i->trigger();
}

void ActionManager::setValueAll(var value)
{
    for (auto& i : items) i->setValue(value);
}
