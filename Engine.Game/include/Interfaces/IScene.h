#pragma once


namespace NG_Engine
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void Awake() = 0;
		virtual void Start() = 0;

		virtual void Destroy() = 0;

		virtual void FixedUpdate() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;

		virtual void Render() = 0;
		virtual void PostRender() = 0;

		virtual void GUI() = 0;
	};
}