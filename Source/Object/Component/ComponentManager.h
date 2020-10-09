/*
  ==============================================================================

    ComponentManager.h
    Created: 26 Sep 2020 10:02:54am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ObjectComponent.h"

class ComponentFactory :
    public Factory<ObjectComponent>
{
public:
    juce_DeclareSingleton(ComponentFactory, true);
    ComponentFactory();
    ~ComponentFactory() {}
};

class ComponentManager :
    public BaseManager<ObjectComponent>
{
public:
    ComponentManager();
    ~ComponentManager();

    void addComponentFromDefinition(StringRef type, var definition, bool canBeRemoved = false);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    var getJSONData() override;
    void loadJSONDataInternal(var data) override;
};