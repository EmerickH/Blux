/*
  ==============================================================================

    BluxEngine.h
    Created: 26 Sep 2020 10:01:11am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class BluxEngine : public Engine,
    public SimpleWebSocketServer::Listener
{
public:
    BluxEngine();
    ~BluxEngine();

    std::unique_ptr<SimpleWebSocketServer> vizServer;
    void initVizServer();

    void connectionOpened(const String& id);
    void messageReceived(const String& id, const String& message);

    void sendAllData(const String& id = "");
    void sendControllableData(Controllable* c);
    void sendServerMessage(const String message = "", const String & id = "");

    var getVizData();

    void onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;
    void clearInternal() override;

    virtual String getMinimumRequiredFileVersion() override;

    var getJSONData() override;
    void loadJSONDataInternalEngine(var data, ProgressTask * task) override;
};

class BluxSettings : 
    public ControllableContainer
{
public:
    juce_DeclareSingleton(BluxSettings, true);

    BluxSettings();
    ~BluxSettings();

    FloatParameter * defaultSceneLoadTime;
};
