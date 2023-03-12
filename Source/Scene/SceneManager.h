/*
  ==============================================================================

    SceneManager.h
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;

class SceneManager :
    public BaseManager<Scene>,
    public Inspectable::InspectableListener,
    public SceneListener,
    public Thread
{
public:
    juce_DeclareSingleton(SceneManager, true);

    SceneManager();
    ~SceneManager();

    Factory<Scene> factory;


    float loadTime;

    Scene* previousScene;
    Scene* currentScene;

    Trigger* loadNextSceneTrigger;
    Trigger* loadPreviousSceneTrigger;
    BoolParameter * autoPreview;

    BoolParameter* lockUI;

    void addItemInternal(Scene* s, var data) override;
    void removeItemInternal(Scene* s) override;

    void loadScene(Scene* s, float loadTime = -1);

    Scene* getNextScene();
    Scene* getPreviousScene();

    void run() override;
    //void lerpSceneParams(float weight);

    void askForLoadScene(Scene* s, float time) override;


    Array<ChainVizTarget *> getChainVizTargetsForObject(Object * o);
    void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values);

    void onContainerTriggerTriggered(Trigger* t) override;
    void onContainerParameterChanged(Parameter* p) override;

    void inspectableDestroyed(Inspectable* i) override;

    QueuedNotifier<SceneManagerEvent> sceneManagerNotifier;

    void addAsyncSceneManagerListener(AsyncSceneListener* newListener) { sceneManagerNotifier.addListener(newListener); }
    void addAsyncCoalescedSceneManagerListener(AsyncSceneListener* newListener) { sceneManagerNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncSceneManagerListener(AsyncSceneListener* listener) { sceneManagerNotifier.removeListener(listener); }
};