using System;

namespace Mist.Scripting;

/// <summary>
/// Example C# script showing the minimal code needed to create game behavior
/// </summary>
public class SimpleMovement : ManagedScript {
	float moveSpeed = 5.0f;

	public override void OnCreate () {
		// Initialization code here
		Console.WriteLine($"C# - OnCreate");
	}

	public override void OnUpdate (float deltaTime) {
		// Game logic here
		// This is called every frame

		GetTransformPosition(out float x, out float y, out float z);

		Random rn = new Random();

		x += rn.Next(-2, 3) / 500f;
		y += rn.Next(-2, 3) / 500f;
		z += rn.Next(-2, 3) / 500f;

		SetTransformPosition(x, y, z);

		Console.WriteLine($"C# - OnUpdate({deltaTime}ms)\nPosition:{x},{y},{z}");
	}

	public override void OnDestroy () {
		// Cleanup code here
		Console.WriteLine($"C# - OnDestroy");
	}
}
