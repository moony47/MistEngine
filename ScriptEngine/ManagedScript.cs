using System;
using System.Runtime.InteropServices;

namespace Mist.Scripting;

/// <summary>
/// Base class for all C# scripts in Mistwraith
/// Inherit from this class to create custom game behavior
/// </summary>
public abstract class ManagedScript {

	[UnmanagedCallersOnly]
	public static unsafe IntPtr CreateScript (char* typeNamePtr) {
		// Convert native wchar_t* to C# string
		string typeName = Marshal.PtrToStringUni((IntPtr) typeNamePtr);

		// Find the type in the current assembly
		Type t = Type.GetType(typeName);

		if (t != null && !t.IsAbstract && typeof(ManagedScript).IsAssignableFrom(t)) {
			object instance = Activator.CreateInstance(t);
			return GCHandle.ToIntPtr(GCHandle.Alloc(instance));
		}

		return IntPtr.Zero;
	}

	[UnmanagedCallersOnly]
	public static void OnCreateNative (IntPtr handle) {
		GCHandle gcHandle = GCHandle.FromIntPtr(handle);
		ManagedScript instance = (ManagedScript) gcHandle.Target;

		instance.OnCreate();
	}

	[UnmanagedCallersOnly]
	public static void OnUpdateNative (IntPtr handle, float deltaTime) {
		GCHandle gcHandle = GCHandle.FromIntPtr(handle);
		ManagedScript instance = (ManagedScript) gcHandle.Target;

		instance.OnUpdate(deltaTime);
	}

	[UnmanagedCallersOnly]
	public static void OnDestroyNative (IntPtr handle) {
		GCHandle gcHandle = GCHandle.FromIntPtr(handle);
		ManagedScript instance = (ManagedScript) gcHandle.Target;

		instance.OnDestroy();
	}

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
