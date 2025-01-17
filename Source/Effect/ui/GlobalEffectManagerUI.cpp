/*
  ==============================================================================

    GlobalEffectManagerUI.cpp
    Created: 30 Sep 2020 1:37:01pm
    Author:  bkupe

  ==============================================================================
*/

GlobalEffectManagerUI::GlobalEffectManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, GlobalEffectManager::getInstance())
{
    setShowSearchBar(true);
    addExistingItems();
}

GlobalEffectManagerUI::~GlobalEffectManagerUI()
{
}
