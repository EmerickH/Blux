/*
  ==============================================================================

    TimedEffect.h
    Created: 9 Oct 2020 9:16:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include  "../../Effect.h"

class TimedEffect :
    public Effect,
    public Timer
{
public:
    TimedEffect(const String &name, var params = var());
    virtual ~TimedEffect();

    FloatParameter* speed;
    FloatParameter* timeOffset;

    FloatParameter* offsetByID;
    FloatParameter* offsetByValue;


    double timeAtLastUpdate;
    double curTime;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c,var values, int id, float time = -1) override;
    virtual var getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c,var value, int id, float time) { return value;  }

    virtual float getCurrentTime(float timeOverride = -1);

    virtual void timerCallback() override;  
    virtual void addTime();

    virtual void onContainerParameterChangedInternal(Parameter* p) override;
};