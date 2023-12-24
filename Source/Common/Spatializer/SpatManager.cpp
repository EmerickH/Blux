/*
  ==============================================================================

    SpatManager.cpp
    Created: 24 Dec 2023 10:04:41am
    Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

SpatManager::SpatManager() :
    BaseManager("Spatializer")
{
    selectItemWhenCreated = false;
}

SpatManager::~SpatManager()
{
}
