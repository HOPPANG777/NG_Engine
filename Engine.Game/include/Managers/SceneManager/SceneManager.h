#pragma once
#pragma once

#include <memory>

#include "Interfaces/IScene.h"

namespace NG_Engine
{
    class SceneManager
    {
    public:
        SceneManager() = default;
        ~SceneManager();

        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

        void SetActiveScene(std::unique_ptr<Scene> scene);
        void ClearActiveScene();

        Scene* GetActiveScene();
        const Scene* GetActiveScene() const;
        bool HasActiveScene() const;

        void PrepareActiveScene();
        void FixedUpdate();
        void Update();
        void LateUpdate();
        void Render();
        void PostRender();
        void GUI();

        void Shutdown();

    private:
        void DestroyActiveScene();

    private:
        std::unique_ptr<Scene> g_activeScene;
        std::unique_ptr<Scene> g_pendingScene;
        bool bPendingClear = false;
        bool bAwakeCalled = false;
        bool bStartCalled = false;
    };
}