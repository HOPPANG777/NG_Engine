#include "Managers/SceneManager/SceneManager.h"

#include <utility>

using namespace NG_Engine;

SceneManager::~SceneManager()
{
    Shutdown();
}

void SceneManager::SetActiveScene(std::unique_ptr<Scene> scene)
{
    if (!scene)
    {
        ClearActiveScene();
        return;
    }

    g_pendingScene = std::move(scene);
    bPendingClear = false;
}

void SceneManager::ClearActiveScene()
{
    bPendingClear = true;
    g_pendingScene.reset();
}

Scene* SceneManager::GetActiveScene()
{
    return g_activeScene.get();
}

const Scene* SceneManager::GetActiveScene() const
{
    return g_activeScene.get();
}

bool SceneManager::HasActiveScene() const
{
    return g_activeScene != nullptr;
}

void SceneManager::PrepareActiveScene()
{
    if (bPendingClear)
    {
        DestroyActiveScene();
        bPendingClear = false;
    }

    if (g_pendingScene)
    {
        DestroyActiveScene();
        g_activeScene = std::move(g_pendingScene);
    }

    if (!g_activeScene)
    {
        return;
    }

    if (!bAwakeCalled)
    {
        g_activeScene->Awake();
        bAwakeCalled = true;
    }

    if (!bStartCalled)
    {
        g_activeScene->Start();
        bStartCalled = true;
    }
}

void SceneManager::FixedUpdate()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->FixedUpdate();
    }
}

void SceneManager::Update()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->Update();
    }
}

void SceneManager::LateUpdate()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->LateUpdate();
    }
}

void SceneManager::Render()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->Render();
    }
}

void SceneManager::PostRender()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->PostRender();
    }
}

void SceneManager::GUI()
{
    if (g_activeScene && bStartCalled)
    {
        g_activeScene->GUI();
    }
}

void SceneManager::Shutdown()
{
    DestroyActiveScene();
    g_pendingScene.reset();
    bPendingClear = false;
}

void SceneManager::DestroyActiveScene()
{
    if (g_activeScene)
    {
        g_activeScene->Destroy();
        g_activeScene.reset();
    }

    bAwakeCalled = false;
    bStartCalled = false;
}