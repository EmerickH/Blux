/*
  ==============================================================================

    ObjectGridUI.cpp
    Created: 26 Sep 2020 7:51:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectGridUI.h"
#include "UI/AssetManager.h"
#include "../ObjectManager.h"
#include "../Component/components/intensity/IntensityComponent.h"
#include "../Component/components/color/ColorComponent.h"

ObjectGridUI::ObjectGridUI(Object* object) :
	BaseItemMinimalUI(object),
	shouldRepaint(false),
	flashMode(false),
	transparentBG(false),
	previewIntensity(0)
{
	updateThumbnail();
	bringToFrontOnSelect = false;
	autoHideWhenDragging = false;

	if (IntensityComponent* ic = item->getComponent<IntensityComponent>())
	{
		computedIntensityUI.reset(((FloatParameter*)ic->computedParameters[0])->createSlider());
		computedIntensityUI->useCustomBGColor = true;
		computedIntensityUI->customBGColor = BG_COLOR.darker(.6f);
		computedIntensityUI->showLabel = false;
		computedIntensityUI->showValue = false;
		addAndMakeVisible(computedIntensityUI.get());

		intensityUI.reset(((FloatParameter*)ic->values[0])->createSlider());
		intensityUI->useCustomBGColor = true;
		intensityUI->customBGColor = BG_COLOR.darker(.6f);
		intensityUI->showLabel = false;
		intensityUI->showValue = false;
		addAndMakeVisible(intensityUI.get());
	}

	if (ColorComponent* cc = item->getComponent<ColorComponent>())
	{
		computedColorUI.reset((ColorParameterUI *)((ColorParameter*)cc->computedParameters[0]->createDefaultUI()));
		addAndMakeVisible(computedColorUI.get());
	}

	globalIDUI.reset(item->globalID->createLabelUI());
	addAndMakeVisible(globalIDUI.get());

	setRepaintsOnMouseActivity(true);

	setSize(128, 128);

}

ObjectGridUI::~ObjectGridUI()
{
	//if (item != nullptr) item->removeAsyncModelListener(this);
}

void ObjectGridUI::paint(Graphics& g)
{
	g.setColour(bgColor);
	if(!transparentBG) g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);

	Rectangle<int> r = getLocalBounds();

	
	r.removeFromBottom(20);

	if (!previewData.isVoid())
	{
		Rectangle<float> pr = r.withHeight(10).withBottomY(r.getBottom()).reduced(2).toFloat();
		g.setColour(Colours::purple.darker(1));
		g.fillRoundedRectangle(pr, 4);
		g.setColour(Colours::purple.brighter(.3f));
		g.fillRoundedRectangle(pr.withWidth(previewIntensity * pr.getWidth()), 4);
		g.setColour(Colours::purple.brighter());
		g.drawRoundedRectangle(pr, 4, .5f);
	}

	g.setColour(Colours::white.withAlpha(isMouseOver() ? .2f : 1.f));
	if (objectImage.getWidth() > 0) g.drawImage(objectImage, r.reduced(6).toFloat(), RectanglePlacement::centred);

	if (objectImage.getWidth() == 0 || isMouseOver())
	{
		g.setColour(Colours::white);
		g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
	}
}

void ObjectGridUI::resized()
{
	Rectangle<int> r = getLocalBounds();

	globalIDUI->setVisible(r.getWidth() >= 90);
	if(globalIDUI->isVisible()) globalIDUI->setBounds(r.withSize(40, 16).reduced(2));

	if (computedColorUI != nullptr)
	{
		Rectangle<int> cr = getLocalBounds().removeFromRight(30).removeFromTop(30).reduced(4);
		computedColorUI->setBounds(cr);
	}
	
	if (computedIntensityUI != nullptr)
	{
		computedIntensityUI->setBounds(r.removeFromBottom(10).reduced(2));
		intensityUI->setBounds(r.removeFromBottom(10).reduced(2));
	}
}


void ObjectGridUI::updateThumbnail()
{
	if (item->customThumbnailPath.existsAsFile()) objectImage = ImageCache::getFromFile(item->customThumbnailPath);
	if (objectImage.getWidth() == 0) objectImage = BluxAssetManager::getImage("icon128");

	shouldRepaint = true;
}

void ObjectGridUI::setPreviewData(var data)
{
	if (item->excludeFromScenes->boolValue()) return;
	if (previewData.isVoid() && data.isVoid()) return; //Avoid repainting all the time

	previewData = data.clone();
	var iData = previewData.getProperty("components", var()).getProperty("intensity", var()).getProperty("value", var());
	if (!iData.isVoid()) previewIntensity = (float)iData;

	repaint();
}

void ObjectGridUI::mouseDown(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isAltDown())
	{
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
			if (e.mods.isRightButtonDown()) if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
		}
	}
}

void ObjectGridUI::mouseDrag(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDrag(e);

	if (e.mods.isAltDown())
	{
		const float pixelRange = 200;
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef - e.getDistanceFromDragStartY() / pixelRange);
		}
	}
}

void ObjectGridUI::mouseUp(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseUp(e);

	if (e.mods.isAltDown())
	{
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		
		if (e.mods.isRightButtonDown())
		{
			for (auto& o : objects)
			{
				if (o->slideManipParameter == nullptr) continue;
				o->slideManipParameter->setValue(o->slideManipValueRef);
			}
		}
		else
		{
			Array<UndoableAction*> actions;
			for (auto& o : objects) if (o->slideManipParameter != nullptr) actions.add(o->slideManipParameter->setUndoableValue(o->slideManipValueRef, o->slideManipParameter->floatValue(), true));
			UndoMaster::getInstance()->performActions("Change " + String(actions.size()) + " values ", actions);
		}
	}
}

bool ObjectGridUI::keyStateChanged(bool isDown)
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::createFromDescription("f").getKeyCode()))
	{
		flashMode = true;

		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
		}

		return true;
	}
	else if (flashMode)
	{
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef);
		}
		
		flashMode = false;
		return true;
	}

	return false;
}

void ObjectGridUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	if (IntensityComponent* ic = c->getParentAs<IntensityComponent>())
	{
		shouldRepaint = true;

	}
}

void ObjectGridUI::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}