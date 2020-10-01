/*
  ==============================================================================

    FilterManager.h
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Filter.h"

class FilterManager :
    public BaseManager<Filter>
{
public:
    FilterManager(bool hasComponentFilter = true, bool hasObjectFilter = true);
    ~FilterManager();

    Factory<Filter> factory;

    int getFilteredIDForObject(Object* o);
};