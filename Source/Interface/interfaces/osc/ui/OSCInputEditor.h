/*
  ==============================================================================

    OSCInputEditor.h
    Created: 13 Oct 2020 9:44:13am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class OSCInputEditor :
    public EnablingControllableContainerEditor
{
public:
    OSCInputEditor(EnablingControllableContainer* cc, bool isRoot);
    ~OSCInputEditor();


    void resizedInternalHeader(Rectangle<int>& r) override;
    Label ipLabel;


    static InspectableEditor* create(bool isRoot, Array<ControllableContainer*> cc) { return new OSCInputEditor((EnablingControllableContainer*)cc[0], isRoot); }
};