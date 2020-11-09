/*
  ==============================================================================

	NoiseEffect.cpp
	Created: 30 Sep 2020 2:57:23pm
	Author:  bkupe

  ==============================================================================
*/

#include "NoiseEffect.h"
#include "Object/Object.h"

NoiseEffect::NoiseEffect(var params) :
	TimedEffect(getTypeString(), params)
{
	type = addEnumParameter("Noise Type", "Type of noise to use");
	type->addOption("Sine", SINE)->addOption("Perlin", PERLIN);

	frequency = addFloatParameter("Frequency", "Frequency of the noise. This will act weirdly when animating it !", 1);
	amplitude = addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
	valueOffset = addFloatParameter("Value Offset", "This defines the center value of the noise. For example, 0 will make the sine oscillate from -amplitude/2 to +amplitude/2", .5f);

	offsetByID->defaultUI = FloatParameter::TIME;
	offsetByValue->defaultUI = FloatParameter::TIME;

	filterManager.componentSelector.allowedComponents.removeAllInstancesOf(ObjectComponent::COLOR);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
}

NoiseEffect::~NoiseEffect()
{
}

var NoiseEffect::getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, var value, int id, float time)
{
	NoiseType t = type->getValueDataAsEnum<NoiseType>();
	float noiseVal = 0;
	switch (t)
	{
	case PERLIN:
		noiseVal = perlin.noise(time) * 2;
		break;

	case SINE:
		noiseVal = sinf(time * float_Pi) * .5f;
		break;
	}

	return valueOffset->floatValue() + noiseVal * amplitude->floatValue();
}

float NoiseEffect::getCurrentTime(float timeOverride)
{
	return TimedEffect::getCurrentTime(timeOverride) * frequency->floatValue();
}
