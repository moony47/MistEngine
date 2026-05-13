using System;

/// <summary>
/// Example C# script showing the minimal code needed to create game behavior
/// </summary>
public class SimpleMovement : ManagedScript {
	private float moveSpeed = 5.0f;

	public override void OnCreate () {
		// Initialization code here
	}

	public override void OnUpdate (float deltaTime) {
		// Game logic here
		// This is called every frame
		Console.WriteLine($"C# - OnUpdate - {deltaTime}ms");
	}

	public override void OnDestroy () {
		// Cleanup code here
	}
}
