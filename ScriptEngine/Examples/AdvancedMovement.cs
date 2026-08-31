using System;

namespace Mist.Scripting;

/// <summary>
/// Example script showing the minimal code needed to create game behavior
/// </summary>
public class AdvancedMovement : ManagedScript {
	float moveSpeed = 10.0f;

	public override void OnCreate () {
		// Initialization code
		Console.WriteLine($"[ManagedRuntime] OnCreate - {GetName()}[{GetID():X}]");
	}

	public override void OnUpdate (float deltaTime) {
		// Game logic - This is called every frame

		if (IsKeyPressed(Key.W))
			PositionY += moveSpeed * deltaTime;
		if (IsKeyPressed(Key.A))
			PositionX -= moveSpeed * deltaTime;
		if (IsKeyPressed(Key.S))
			PositionY -= moveSpeed * deltaTime;
		if (IsKeyPressed(Key.D))
			PositionX += moveSpeed * deltaTime;
	}

	public override void OnKeyPressed (Key key) {
	}
	public override void OnKeyReleased (Key key) {
	}
	public override void OnMousePressed (MouseButton button) {
	}
	public override void OnMouseReleased (MouseButton button) {
	}
	public override void OnMouseMoved (float x, float y) {
	}
	public override void OnMouseScrolled (float xOffset, float yOffset) {
	}

	public override void OnDestroy () {
		// Cleanup code
		Console.WriteLine($"[ManagedRuntime] OnDestroy - {GetName()}[{GetID():X}]");
	}
}
