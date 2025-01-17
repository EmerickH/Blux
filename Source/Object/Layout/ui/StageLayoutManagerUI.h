/*
  ==============================================================================

    StageLayoutManagerUI.h
    Created: 6 Oct 2020 9:50:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StageLayoutManagerUI :
    public BaseManagerShapeShifterUI<StageLayoutManager, StageLayout, StageLayoutUI>
{
public:
    StageLayoutManagerUI(const String& name);
    ~StageLayoutManagerUI();


    static StageLayoutManagerUI* create(const String& name) { return new StageLayoutManagerUI(name); }
};