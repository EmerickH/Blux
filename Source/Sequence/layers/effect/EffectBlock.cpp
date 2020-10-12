/*
  ==============================================================================

    EffectBlock.cpp
    Created: 10 Oct 2020 10:46:50am
    Author:  bkupe

  ==============================================================================
*/

#include "EffectBlock.h"
#include "Effect/EffectManager.h"
#include "Effect/effects/time/TimedEffect.h"

EffectBlock::EffectBlock(var params) :
    LayerBlock(params.getProperty("effectType",var()).toString()),
    effectType(params.getProperty("effectType", var()).toString()),
    effectBlockNotifier(5)
{
    fadeIn = addFloatParameter("Fade In", "Fade in time", 0, 0, getTotalLength(), false);
    fadeIn->defaultUI = FloatParameter::TIME;
    fadeIn->canBeDisabledByUser = true;
    fadeOut = addFloatParameter("Fade Out", "Fade out time", 0, 0, getTotalLength(), false);
    fadeOut->defaultUI = FloatParameter::TIME;
    fadeOut->canBeDisabledByUser = true;

    effect.reset(EffectFactory::getInstance()->create(effectType));
    if (effect != nullptr)
    {
        effect->editorCanBeCollapsed = false;
        effect->canBeDisabled = false;
        //effect->enabled->setValue(true); //this way, timeEffect won't start their timers.

        if (TimedEffect* te = dynamic_cast<TimedEffect*>(effect.get()))
        {
            te->speed->hideInEditor = true; //hide speed since it cannot be used alongside time
        }

        addChildControllableContainer(effect.get());
    }
}

EffectBlock::~EffectBlock()
{

}

void EffectBlock::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier, int id, float absoluteTime)
{
    if (effect == nullptr) return;

	float factor = 1;
	double relTimeTotal = absoluteTime - time->floatValue();
	if (fadeIn->floatValue() > 0) factor *= jmin<double>(relTimeTotal / fadeIn->floatValue(), 1.f);
	if (fadeOut->floatValue() > 0) factor *= jmin<double>((getTotalLength() - relTimeTotal) / fadeOut->floatValue(), 1.f);
	factor = jmax(factor, 0.f);

    float targetWeight = factor * weightMultiplier;

	double relTimeLooped = getRelativeTime(absoluteTime, true);
    effect->processComponentValues(o, c, values, targetWeight, id, relTimeLooped);
}

void EffectBlock::onContainerParameterChangedInternal(Parameter* p)
{
    LayerBlock::onContainerParameterChangedInternal(p);

    if (p == coreLength || p == loopLength)
	{
		fadeIn->setRange(0, getTotalLength());
		fadeOut->setRange(0, getTotalLength());
    }
    else if (p == fadeIn || p == fadeOut)
    {
        effectBlockListeners.call(&EffectBlockListener::effectBlockFadesChanged, this);
        effectBlockNotifier.addMessage(new EffectBlockEvent(EffectBlockEvent::FADES_CHANGED, this));
    }
}

void EffectBlock::controllableStateChanged(Controllable* c)
{
    LayerBlock::controllableStateChanged(c);
    if (c == fadeIn || c == fadeOut)
    {
        effectBlockListeners.call(&EffectBlockListener::effectBlockFadesChanged, this);
        effectBlockNotifier.addMessage(new EffectBlockEvent(EffectBlockEvent::FADES_CHANGED, this));
    }
}