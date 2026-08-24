#include "mistpch.h"
#include "ManagedScript.h"

#include "Mist/Scene/Entity.h"
#include "Mist/Events/EventSystem.h"

namespace Mist {

	on_create_fn ManagedScript::s_OnCreateFunc = nullptr;
	on_update_fn ManagedScript::s_OnUpdateFunc = nullptr;
	on_destroy_fn ManagedScript::s_OnDestroyFunc = nullptr;

	on_button_fn ManagedScript::s_OnKeyPressedFunc = nullptr;
	on_button_fn ManagedScript::s_OnKeyReleasedFunc = nullptr;

	on_button_fn ManagedScript::s_OnMousePressedFunc = nullptr;
	on_button_fn ManagedScript::s_OnMouseReleasedFunc = nullptr;
	on_move_fn ManagedScript::s_OnMouseMovedFunc = nullptr;
	on_move_fn ManagedScript::s_OnMouseScrolledFunc = nullptr;

	ManagedScript::ManagedScript(entt::entity entity, Scene* scene, managed_script instance) :
		m_Entity(new Entity {scene, entity}),
		m_ManagedScriptInstance(instance) {
		OnCreate();
	}

	ManagedScript::~ManagedScript() {
		OnDestroy();
		delete m_ManagedScriptInstance;
	}

	void ManagedScript::OnCreate() const {
		MIST_ASSERT(s_OnCreateFunc, "Dotnet managed script OnCreate function point is not initialised")

			s_OnCreateFunc(m_Entity, m_ManagedScriptInstance);
	}
	void ManagedScript::OnUpdate(float deltaTime) const {
		MIST_ASSERT(s_OnUpdateFunc, "Dotnet managed script OnUpdate function point is not initialised")

			s_OnUpdateFunc(m_Entity, m_ManagedScriptInstance, deltaTime);
	}
	void ManagedScript::OnDestroy() const {
		MIST_ASSERT(s_OnDestroyFunc, "Dotnet managed script OnDestroy function point is not initialised")

			s_OnDestroyFunc(m_Entity, m_ManagedScriptInstance);
	}

	void ManagedScript::OnEvent(Event& e) const {
		switch(e.GetEventType()) {
			case EventType::KeyPressed:
			{
				MIST_ASSERT(s_OnKeyPressedFunc, "Dotnet managed script OnKeyPressedFunc function point is not initialised")
					KeyPressedEvent* event = (KeyPressedEvent*) &e;
				s_OnKeyPressedFunc(m_Entity, m_ManagedScriptInstance, (uint16_t) event->GetKeyCode());
				break;
			}
			case EventType::KeyReleased:
			{
				MIST_ASSERT(s_OnKeyReleasedFunc, "Dotnet managed script OnKeyReleasedFunc function point is not initialised")
					KeyReleasedEvent* event = (KeyReleasedEvent*) &e;
				s_OnKeyReleasedFunc(m_Entity, m_ManagedScriptInstance, (uint16_t) event->GetKeyCode());
				break;
			}
			case EventType::MouseButtonPressed:
			{
				MIST_ASSERT(s_OnMousePressedFunc, "Dotnet managed script OnMousePressedFunc function point is not initialised")
					MouseButtonPressedEvent* event = (MouseButtonPressedEvent*) &e;
				s_OnMousePressedFunc(m_Entity, m_ManagedScriptInstance, (uint16_t) event->GetMouseButton());
				break;
			}
			case EventType::MouseButtonReleased:
			{
				MIST_ASSERT(s_OnMouseReleasedFunc, "Dotnet managed script OnMouseReleasedFunc function point is not initialised")
					MouseButtonReleasedEvent* event = (MouseButtonReleasedEvent*) &e;
				s_OnMouseReleasedFunc(m_Entity, m_ManagedScriptInstance, (uint16_t) event->GetMouseButton());
				break;
			}
			case EventType::MouseMoved:
			{
				MIST_ASSERT(s_OnMouseMovedFunc, "Dotnet managed script OnMouseMovedEventFunc function point is not initialised")
					MouseMovedEvent* event = (MouseMovedEvent*) &e;
				s_OnMouseMovedFunc(m_Entity, m_ManagedScriptInstance, event->GetX(), event->GetY());
				break;
			}
			case EventType::MouseScrolled:
			{
				MIST_ASSERT(s_OnMouseScrolledFunc, "Dotnet managed script OnMouseScrolledEventFunc function point is not initialised")
					MouseScrolledEvent* event = (MouseScrolledEvent*) &e;
				s_OnMouseScrolledFunc(m_Entity, m_ManagedScriptInstance, event->GetXOffset(), event->GetYOffset());
				break;
			}
		}
	}

} // namespace Mist