/*
  ==============================================================================

    BluxSequence.h
    Created: 5 Oct 2020 2:42:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ChainViz/ChainVizTarget.h"

class Object;
class ObjectComponent;
class Effect;

class BluxSequence :
    public Sequence,
    public ChainVizTarget
{
public:
    BluxSequence();
    ~BluxSequence();

    bool manualStartAtLoad;

    bool isAffectingObject(Object* o);
    Array<ChainVizTarget *> getChainVizTargetsForObject(Object* o);

    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f);

    void endLoadFile() override;

    ChainVizComponent* createVizComponent(Object* o, ChainVizTarget::ChainVizType type) override;

    InspectableEditor* getEditor(bool isRoot) override;
    String getTypeString() const override { return "Blux Sequence"; }
};