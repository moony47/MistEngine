using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Mist.Scripting;

/// <summary>
/// Base class for all C# scripts in Mistwraith
/// Inherit from this class to create custom game behavior
/// </summary>
public abstract class ManagedScript {

	private unsafe static class Callbacks {
		internal static delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void> GetTransformComponent;
		internal static delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void> GetIDComponent;
		internal static delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void> GetCameraComponent;

		internal static delegate* unmanaged[Stdcall]<IntPtr, out float, out float, out float, void> GetTransformPosition;
		internal static delegate* unmanaged[Stdcall]<IntPtr, float, float, float, void> SetTransformPosition;
	}

	[UnmanagedCallersOnly]
	public static unsafe int RegisterCallback (char* callbackName, IntPtr callbackPtr) {
		try {
			string callbackLabel = Marshal.PtrToStringUni((IntPtr) callbackName);
			Console.WriteLine($"RegisterCallback: {callbackLabel}");

			switch (callbackLabel) {
				case "GetTransformComponent":
					Callbacks.GetTransformComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>) callbackPtr;
					break;
				case "GetIDComponent":
					Callbacks.GetIDComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>) callbackPtr;
					break;
				case "GetCameraComponent":
					Callbacks.GetCameraComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>) callbackPtr;
					break;
				case "SetTransformPosition":
					Callbacks.SetTransformPosition = (delegate* unmanaged[Stdcall]<IntPtr, float, float, float, void>) callbackPtr;
					break;
				case "GetTransformPosition":
					Callbacks.GetTransformPosition = (delegate* unmanaged[Stdcall]<IntPtr, out float, out float, out float, void>) callbackPtr;
					break;
				default:
					throw new ArgumentException($"Unexpected callback label: {callbackLabel}");
			}

			return 1;
		} catch (Exception ex) {
			Console.WriteLine($"Error in OnCreate: {ex}");
			return 0;
		}
	}

	[UnmanagedCallersOnly]
	public static unsafe IntPtr CreateManagedScript (char* typeNamePtr) {
		try {
			string typeName = Marshal.PtrToStringUni((IntPtr) typeNamePtr);
			Console.WriteLine($"CreateManagedScript: {typeName}");

			Type t = Type.GetType(typeName);

			if (t != null && !t.IsAbstract && typeof(ManagedScript).IsAssignableFrom(t)) {
				ManagedScript instance = (ManagedScript) Activator.CreateInstance(t);
				return GCHandle.ToIntPtr(GCHandle.Alloc(instance));
			}
		} catch (Exception ex) {
			Console.WriteLine($"Error in OnCreate: {ex}");
		}

		return IntPtr.Zero;
	}

	[UnmanagedCallersOnly]
	public static void OnCreateNative (IntPtr entityPtr, IntPtr scriptPtr) {
		try {
			Console.WriteLine($"OnCreateNative");

			GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
			ManagedScript instance = (ManagedScript) gcHandle.Target;
			instance.entityPtr = entityPtr;

			instance.OnCreate();
		} catch (Exception ex) {
			Console.WriteLine($"Error in OnCreate: {ex}");
		}
	}

	[UnmanagedCallersOnly]
	public static void OnUpdateNative (IntPtr entityPtr, IntPtr scriptPtr, float deltaTime) {
		try {
			Console.WriteLine($"OnUpdateNative");

			GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
			ManagedScript instance = (ManagedScript) gcHandle.Target;
			instance.entityPtr = entityPtr;

			instance.OnUpdate(deltaTime);
		} catch (Exception ex) {
			Console.WriteLine($"Error in OnUpdate: {ex}");
		}
	}

	[UnmanagedCallersOnly]
	public static void OnDestroyNative (IntPtr entityPtr, IntPtr scriptPtr) {
		try {
			Console.WriteLine($"OnDestroyNative");

			GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
			ManagedScript instance = (ManagedScript) gcHandle.Target;
			instance.entityPtr = entityPtr;

			instance.OnDestroy();
		} catch (Exception ex) {
			Console.WriteLine($"Error in OnDestroy: {ex}");
		}
	}


	IntPtr entityPtr;

	protected unsafe IntPtr TransformPtr {
		get {
			Callbacks.GetTransformComponent(entityPtr, out IntPtr transformPtr);
			return transformPtr;
		}
	}

	protected void SetTransformPosition (float x, float y, float z) {
		unsafe {
			Callbacks.SetTransformPosition(TransformPtr, x, y, z);
		}
	}
	protected void GetTransformPosition (out float x, out float y, out float z) {
		unsafe {
			Callbacks.GetTransformPosition(TransformPtr, out x, out y, out z);
		}
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
