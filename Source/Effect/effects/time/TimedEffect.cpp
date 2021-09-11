/*
  ==============================================================================

    TimedEffect.cpp
    Created: 9 Oct 2020 9:16:14pm
    Author:  bkupe

  ==============================================================================
*/

TimedEffect::TimedEffect(const String &name, var params) :
    Effect(name, params),
    timeAtLastUpdate(Time::getMillisecondCounterHiRes() / 1000.0),
    curTime(0)
{
    speed = addFloatParameter("Speed", "The speed at which play this", 1);
	timeOffset = addFloatParameter("Time Offset", "This allows for offsetting the time, for manual position animation for example.", 0);
	timeOffset->defaultUI = FloatParameter::TIME;
	resetTime = addTrigger("Reset Time", "When triggered, this will reset this effect's internal time to 0.");
	offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
	offsetByValue = addFloatParameter("Time Offset By Value", "Time Offset by parameter inside a component", 0);
}

TimedEffect::~TimedEffect()
{
}

void TimedEffect::onContainerTriggerTriggered(Trigger* t)
{
	Effect::onContainerTriggerTriggered(t);
	if (t == resetTime) curTime = 0;
}

void TimedEffect::updateEnabled()
{
	if (isFullyEnabled())
	{
		timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
		curTime = 0;
		startTimer(20);
	}
	else
	{
		stopTimer();
	}
}

var TimedEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].isArray()) continue;
		float targetTime =  getCurrentTime(time) - offsetByID->floatValue() * id - offsetByValue->floatValue() * i + timeOffset->floatValue();
		values[i] = getProcessedComponentValueTimeInternal(o, c, values[i], id, targetTime);
	}

	return values;
}

float TimedEffect::getCurrentTime(float timeOverride)
{
	return timeOverride >= 0 ? timeOverride : curTime;
}

void TimedEffect::hiResTimerCallback()
{
    if (!isFullyEnabled()) return;
    addTime();
}

void TimedEffect::addTime()
{
    double newTime = Time::getMillisecondCounterHiRes() / 1000.0;
    curTime += (newTime - timeAtLastUpdate) * speed->floatValue();
    timeAtLastUpdate = newTime;

}