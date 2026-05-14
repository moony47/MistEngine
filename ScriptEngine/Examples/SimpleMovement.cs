using System;

namespace Mist.Scripting;

/// <summary>
/// Example C# script showing the minimal code needed to create game behavior
/// </summary>
public class SimpleMovement : ManagedScript {
	private float moveSpeed = 5.0f;

	public override void OnCreate () {
		// Initialization code here
		Console.WriteLine($"C# - OnCreate");
	}

	public override void OnUpdate (float deltaTime) {
		// Game logic here
		// This is called every frame
		Console.WriteLine($"C# - OnUpdate({deltaTime}ms)");
	}

	public override void OnDestroy () {
		// Cleanup code here
		Console.WriteLine($"C# - OnDestroy");
	}
}
