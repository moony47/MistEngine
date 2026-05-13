/// <summary>
/// Base class for all C# scripts in Mistwraith
/// Inherit from this class to create custom game behavior
/// </summary>
public abstract class ManagedScript {

	/// <summary>
	/// Called once when the script instance is created
	/// Used for initialization
	/// </summary>
	public virtual void OnCreate () {
	}

	/// <summary>
	/// Called every frame during scene update
	/// </summary>
	/// <param name="deltaTime">Time elapsed since last frame in seconds</param>
	public virtual void OnUpdate (float deltaTime) {
	}

	/// <summary>
	/// Called when the script instance is being destroyed
	/// Used for cleanup
	/// </summary>
	public virtual void OnDestroy () {
	}
}
