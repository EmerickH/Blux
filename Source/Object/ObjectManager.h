/*
  ==============================================================================

    ObjectManager.h
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object.h"

class SubObjectManager :
    public BaseManager<Object>
{
public:
    SubObjectManager();
    ~SubObjectManager();

};

class ObjectManager :
    public BaseManager<Object>,
    public Object::ObjectListener,
    public Thread
{
public:
    juce_DeclareSingleton(ObjectManager, true);

    ObjectManager();
    ~ObjectManager();

    Factory<Object> factory;

    BoolParameter* blackOut;

    //ui
    IntParameter* gridThumbSize;
    FloatParameter* defaultFlashValue;
    BoolParameter* filterActiveInScene;

    void updateFactoryDefinitions();
    void addItemInternal(Object* o, var data) override;
    void removeItemInternal(Object* o) override;

    int getFirstAvailableObjectID(Object* excludeObject = nullptr);
    Object* getObjectWithID(int id, Object* excludeObject = nullptr);

    void objectIDChanged(Object* o, int previousID) override;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    void run() override;
};