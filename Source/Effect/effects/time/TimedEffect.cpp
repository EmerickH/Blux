/*
  ==============================================================================

	TimedEffect.cpp
	Created: 9 Oct 2020 9:16:14pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

TimedEffect::TimedEffect(const String& name, var params) :
	Effect(name, params),
	forceManualTime(false),
	timeAtLastUpdate(Time::getMillisecondCounterHiRes() / 1000.0)
{
	computePreviousValues = true;

	speed = effectParams.addFloatParameter("Speed", "The speed at which play this", 1);
	speed->canBeAutomated = false;

	timeOffset = effectParams.addFloatParameter("Time Offset", "This allows for offsetting the time, for manual position animation for example.", 0);
	timeOffset->defaultUI = FloatParameter::TIME;
	resetTimeTrigger = effectParams.addTrigger("Reset Time", "When triggered, this will reset this effect's internal time to 0.");
	autoResetOnNonZero = effectParams.addBoolParameter("Auto Reset on NonZero", "", false);
	offsetByID = effectParams.addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
	//offsetByValue = effectParams.addFloatParameter("Time Offset By Value", "Time Offset by parameter inside a component", 0);

	ObjectManager::getInstance()->addBaseManagerListener(this);
	ObjectManager::getInstance()->addObjectManagerListener(this);
}

TimedEffect::~TimedEffect()
{
	if (ObjectManager::getInstanceWithoutCreating() != nullptr)
	{
		ObjectManager::getInstance()->removeBaseManagerListener(this);
		ObjectManager::getInstance()->removeObjectManagerListener(this);
	}
}

void TimedEffect::onContainerTriggerTriggered(Trigger* t)
{
	Effect::onContainerTriggerTriggered(t);
	if (t == resetTimeTrigger) resetTimes();
}

void TimedEffect::updateEnabled()
{
	if (isFullyEnabled())
	{
		timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
		resetTimes();
		//startTimer(20);

		if (ObjectManager::getInstanceWithoutCreating() != nullptr) ObjectManager::getInstance()->addObjectManagerListener(this);
	}
	else
	{
		//stopTimer();
		if (ObjectManager::getInstanceWithoutCreating() != nullptr) ObjectManager::getInstance()->removeObjectManagerListener(this);
	}
}

void TimedEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	if (autoResetOnNonZero->boolValue() && c->mainParameter != nullptr) //component needs to have a reference to "main param" for this kind of purpose
	{
		if (!prevValuesMap.contains(c)) return;

		Parameter* p = c->mainParameter;
		if (prevValuesMap[c]->contains(p) && values.contains(p))
		{
			if ((float)(*prevValuesMap[c])[p] == 0 && (float)values[p] > 0) curTimes.set(c, 0);
		}
	}


	float curTime = getCurrentTime(o, c, id, time);
	float targetTime = curTime - (float)GetLinkedValue(offsetByID) * id /* - (float)GetLinkedValue(offsetByValue) * i*/ + (float)GetLinkedValue(timeOffset);


	processComponentTimeInternal(o, c, values, targetValues, id, targetTime, time);

}

float TimedEffect::getCurrentTime(Object* o, ObjectComponent* c, int id, float timeOverride)
{
	if (!curTimes.contains(c)) curTimes.set(c, 0);
	if (timeOverride == -1) return curTimes[c];

	float time = timeOverride * (float)GetLinkedValueT(speed, timeOverride); //speed should be calculated from start of the animation, if animated (area under curve for automation)

	return time;
}


void TimedEffect::resetTimes()
{
	HashMap<ObjectComponent*, float>::Iterator it(curTimes);
	while (it.next()) curTimes.set(it.getKey(), 0);
}

void TimedEffect::resetTime(Object* o)
{
	for (auto& c : o->componentManager->items) if (curTimes.contains(c)) curTimes.set(c, 0);
}

void TimedEffect::itemRemoved(Object* o)
{
	for (auto& c : o->componentManager->items)
	{
		curTimes.remove(c);

		if (prevValuesMap.contains(c))
		{
			prevValues.removeObject(prevValuesMap[c]);
			prevValuesMap.remove(c);
		}
	}
}

void TimedEffect::itemsRemoved(Array<Object*> oList)
{
	for (auto& o : oList)
	{
		for (auto& c : o->componentManager->items)
		{
			curTimes.remove(c);

			if (prevValuesMap.contains(c))
			{
				prevValues.removeObject(prevValuesMap[c]);
				prevValuesMap.remove(c);
			}
		}
	}
}

void TimedEffect::updateStart()
{
	if (enabled->boolValue() && !forceManualTime) addTime();
}

//void TimedEffect::hiResTimerCallback()
//{
//    if (!isFullyEnabled()) return;
//    addTime();
//}

void TimedEffect::addTime()
{
	double newTime = Time::getMillisecondCounterHiRes() / 1000.0;

	HashMap<ObjectComponent*, float>::Iterator it(curTimes);
	while (it.next())
	{
		Object* o = it.getKey()->object;
		int id = o->globalID->intValue();
		curTimes.set(it.getKey(), it.getValue() + (newTime - timeAtLastUpdate) * (float)GetLinkedValueT(speed, 0));
	}

	timeAtLastUpdate = newTime;

}