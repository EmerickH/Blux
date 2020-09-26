/*
  ==============================================================================

    BluxEngine.cpp
    Created: 26 Sep 2020 10:01:11am
    Author:  bkupe

  ==============================================================================
*/

#include "BluxEngine.h"

BluxEngine::BluxEngine() :
    Engine("Blux",".blux")
{
    mainEngine = this;
}

BluxEngine::~BluxEngine()
{
}
