#pragma once

#include <entt.hpp>
#include "Mist/Events/Event.h"

namespace Mist {

	class Entity;
	class Scene;

	using managed_script = void*;

	using on_create_fn = void (*)(Entity*, managed_script);
	using on_update_fn = void (*)(Entity*, managed_script, float);
	using on_destroy_fn = void (*)(Entity*, managed_script);
	using on_button_fn = void (*)(Entity*, managed_script, uint16_t);
	using on_move_fn = void (*)(Entity*, managed_script, float, float);
	
	class ManagedScript {
	public:
		static on_create_fn s_OnCreateFunc;
		static on_update_fn s_OnUpdateFunc;
		static on_destroy_fn s_OnDestroyFunc;
		static on_button_fn s_OnKeyPressedFunc;
		static on_button_fn s_OnKeyReleasedFunc;
		static on_button_fn s_OnMousePressedFunc;
		static on_button_fn s_OnMouseReleasedFunc;
		static on_move_fn s_OnMouseMovedFunc;
		static on_move_fn s_OnMouseScrolledFunc;

	public:
		ManagedScript(entt::entity entity, Scene* scene, managed_script instance);
		~ManagedScript();

		void OnCreate() const;
		void OnUpdate(float deltaTime) const;
		void OnDestroy() const;
		void OnEvent(Event& e) const;

	private:
		managed_script m_ManagedScriptInstance;
		Entity* m_Entity;
	};

} // namespace Mist
