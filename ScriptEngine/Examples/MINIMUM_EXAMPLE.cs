namespace Mist.Scripting;

/*
 * MINIMUM REQUIRED TO WRITE A C# SCRIPT
 * ======================================
 * 
 * That's it. A user needs just this:
 */

public class MyScript : ManagedScript {
	public override void OnUpdate (float deltaTime) {
		// Your game logic here
	}
}

/*
 * THAT'S LITERALLY IT.
 * 
 * The OnCreate() and OnDestroy() overrides are optional (virtual methods with empty implementations in the base class).
 * 
 * FULL EXAMPLE WITH ALL METHODS:
 */

public class FullExample : ManagedScript {
	private float timer = 0f;

	public override void OnCreate () {
		// Called once when the script instance is created
		System.Console.WriteLine("Script initialized!");
	}

	public override void OnUpdate (float deltaTime) {
		// Called every frame
		timer += deltaTime;

		if (timer > 1f) {
			System.Console.WriteLine("One second has passed!");
			timer = 0f;
		}
	}

	public override void OnDestroy () {
		// Called when the entity is destroyed
		System.Console.WriteLine("Script cleanup!");
	}
}

/*
 * HOW TO COMPILE:
 * 
 *   1. Add your .cs file to the ScriptEngine folder
 *   2. Run: dotnet build -c Release
 *   3. Done! ScriptEngine.dll is generated with your script
 * 
 * HOW TO USE IN ENGINE (once Phase 2 is complete):
 * 
 *   Entity entity = scene.CreateEntity("MyActor");
 *   auto& script = entity.AddComponent<ManagedScriptComponent>();
 *   script.ScriptClassName = "MyScript";  // Fully qualified name if in namespace
 * 
 *   // Engine automatically calls:
 *   // - OnCreate() once
 *   // - OnUpdate(deltaTime) every frame
 *   // - OnDestroy() on cleanup
 * 
 * WHAT YOU CAN'T DO YET (coming in future phases):
 * 
 *   - Access entity transform, rendering, etc. (Phase 3+)
 *   - Listen to input events (Phase 3+)
 *   - Access physics/rigidbodies (Phase 3+)
 *   - Hot-reload scripts without recompiling (Phase 4+)
 *   - Use debugger (Phase 4+)
 * 
 * WHAT YOU CAN DO:
 * 
 *   ✅ Write game logic
 *   ✅ Use all C# 12 features (.NET 8)
 *   ✅ Call other C# classes and libraries
 *   ✅ Use Console.WriteLine for debugging (for now)
 *   ✅ Any managed code that doesn't need engine APIs
 */
