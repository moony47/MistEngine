using System;

namespace Mist.Scripting;

/// <summary>
/// Example script showing the minimal code needed to create game behavior
/// </summary>
public class SimpleMovement : ManagedScript {
    float moveSpeed = 1.0f;

    public override void OnCreate() {
        // Initialization code
        Console.WriteLine($"[ManagedRuntime] OnCreate - {GetName()}[{GetID():X}]");
    }

    public override void OnUpdate(float deltaTime) {
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

    public override void OnKeyPressed(Key key) {
        Console.WriteLine($"[ManagedRuntime] OnKeyPressed - {key}");
    }
    public override void OnKeyReleased(Key key) {
        Console.WriteLine($"[ManagedRuntime] OnKeyReleased - {key}");
    }
    public override void OnMousePressed(MouseButton button) {
        Console.WriteLine($"[ManagedRuntime] OnMousePressed - {button}");
    }
    public override void OnMouseReleased(MouseButton button) {
        Console.WriteLine($"[ManagedRuntime] OnMouseReleased - {button}");
    }
    public override void OnMouseMoved(float x, float y) {
        Console.WriteLine($"[ManagedRuntime] OnMouseMoved - ({x}, {y})");
    }
    public override void OnMouseScrolled(float xOffset, float yOffset) {
        Console.WriteLine($"[ManagedRuntime] OnMouseScrolled - ({xOffset}, {yOffset})");
    }

    public override void OnDestroy() {
        // Cleanup code
        Console.WriteLine($"[ManagedRuntime] OnDestroy");
    }
}
