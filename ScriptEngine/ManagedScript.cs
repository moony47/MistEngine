using System;
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

        internal static delegate* unmanaged[Stdcall]<IntPtr, out ulong, void> GetID;
        internal static delegate* unmanaged[Stdcall]<IntPtr, out char*, void> GetName;

        internal static delegate* unmanaged[Stdcall]<IntPtr, out float, out float, out float, void> GetTransformPosition;
        internal static delegate* unmanaged[Stdcall]<IntPtr, float, float, float, void> SetTransformPosition;

        internal static delegate* unmanaged[Stdcall]<ushort, out bool, void> IsKeyPressed;
        internal static delegate* unmanaged[Stdcall]<ushort, out bool, void> IsMouseButtonPressed;
        internal static delegate* unmanaged[Stdcall]<out float, out float, void> GetMousePosition;

    }

    [UnmanagedCallersOnly]
    public static unsafe int RegisterCallback(char* callbackName, IntPtr callbackPtr) {
        try {
            string callbackLabel = Marshal.PtrToStringUni((IntPtr)callbackName);
            Console.WriteLine($"[ManagedRuntime] RegisterCallback: {callbackLabel}");

            switch (callbackLabel) {
                case "GetTransformComponent":
                    Callbacks.GetTransformComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>)callbackPtr;
                    break;
                case "GetIDComponent":
                    Callbacks.GetIDComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>)callbackPtr;
                    break;
                case "GetCameraComponent":
                    Callbacks.GetCameraComponent = (delegate* unmanaged[Stdcall]<IntPtr, out IntPtr, void>)callbackPtr;
                    break;
                case "GetID":
                    Callbacks.GetID = (delegate* unmanaged[Stdcall]<IntPtr, out ulong, void>)callbackPtr;
                    break;
                case "GetName":
                    Callbacks.GetName = (delegate* unmanaged[Stdcall]<IntPtr, out char*, void>)callbackPtr;
                    break;
                case "SetTransformPosition":
                    Callbacks.SetTransformPosition = (delegate* unmanaged[Stdcall]<IntPtr, float, float, float, void>)callbackPtr;
                    break;
                case "GetTransformPosition":
                    Callbacks.GetTransformPosition = (delegate* unmanaged[Stdcall]<IntPtr, out float, out float, out float, void>)callbackPtr;
                    break;
                case "IsKeyPressed":
                    Callbacks.IsKeyPressed = (delegate* unmanaged[Stdcall]<ushort, out bool, void>)callbackPtr;
                    break;
                case "IsMouseButtonPressed":
                    Callbacks.IsMouseButtonPressed = (delegate* unmanaged[Stdcall]<ushort, out bool, void>)callbackPtr;
                    break;
                case "GetMousePosition":
                    Callbacks.GetMousePosition = (delegate* unmanaged[Stdcall]<out float, out float, void>)callbackPtr;
                    break;
                default:
                    throw new ArgumentException($"[ManagedRuntime] Unexpected callback label: {callbackLabel}");
            }

            return 1;
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in RegisterCallback: {ex}");
            return 0;
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe IntPtr CreateManagedScript(char* typeNamePtr) {
        try {
            string typeName = Marshal.PtrToStringUni((IntPtr)typeNamePtr);
            Console.WriteLine($"[ManagedRuntime] CreateManagedScript: {typeName}");

            Type t = Type.GetType(typeName);

            if (t != null && !t.IsAbstract && typeof(ManagedScript).IsAssignableFrom(t)) {
                ManagedScript instance = (ManagedScript)Activator.CreateInstance(t);
                return GCHandle.ToIntPtr(GCHandle.Alloc(instance));
            }
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in CreateManagedScript: {ex}");
        }

        return IntPtr.Zero;
    }

    [UnmanagedCallersOnly]
    public static void OnCreateNative(IntPtr entityPtr, IntPtr scriptPtr) {
        try {
            //Console.WriteLine($"[ManagedRuntime] OnCreateNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnCreate();
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnCreateNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnUpdateNative(IntPtr entityPtr, IntPtr scriptPtr, float deltaTime) {
        try {
            //Console.WriteLine($"[ManagedRuntime] OnUpdateNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnUpdate(deltaTime);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnUpdateNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnDestroyNative(IntPtr entityPtr, IntPtr scriptPtr) {
        try {
            //Console.WriteLine($"[ManagedRuntime] OnDestroyNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnDestroy();
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnDestroyNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnKeyPressedNative(IntPtr entityPtr, IntPtr scriptPtr, ushort key) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnKeyPressedNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnKeyPressed((Key)key);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnKeyPressedNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnKeyReleasedNative(IntPtr entityPtr, IntPtr scriptPtr, ushort key) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnKeyReleasedNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnKeyReleased((Key)key);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnKeyReleasedNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnMousePressedNative(IntPtr entityPtr, IntPtr scriptPtr, ushort button) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnMousePressedNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnMousePressed((MouseButton)button);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnMousePressedNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnMouseReleasedNative(IntPtr entityPtr, IntPtr scriptPtr, ushort button) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnMouseReleasedNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnMouseReleased((MouseButton)button);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnMouseReleasedNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnMouseMovedNative(IntPtr entityPtr, IntPtr scriptPtr, float x, float y) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnMouseMovedNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnMouseMoved(x, y);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnMouseMovedNative: {ex}");
        }
    }

    [UnmanagedCallersOnly]
    public static void OnMouseScrolledNative(IntPtr entityPtr, IntPtr scriptPtr, float xOffset, float yOffset) {
        try {
            Console.WriteLine($"[ManagedRuntime] OnMouseScrolledNative");

            GCHandle gcHandle = GCHandle.FromIntPtr(scriptPtr);
            ManagedScript instance = (ManagedScript)gcHandle.Target;
            instance.entityPtr = entityPtr;

            instance.OnMouseScrolled(xOffset, yOffset);
        } catch (Exception ex) {
            Console.WriteLine($"[ManagedRuntime] Error in OnMouseScrolledNative: {ex}");
        }
    }

    IntPtr entityPtr;

    protected unsafe IntPtr TransformComponentPtr {
        get {
            Callbacks.GetTransformComponent(entityPtr, out IntPtr transformPtr);
            return transformPtr;
        }
    }
    protected unsafe IntPtr IDComponentPtr {
        get {
            Callbacks.GetIDComponent(entityPtr, out IntPtr idPtr);
            return idPtr;
        }
    }
    protected unsafe IntPtr CameraComponentPtr {
        get {
            Callbacks.GetCameraComponent(entityPtr, out IntPtr cameraPtr);
            return cameraPtr;
        }
    }

    protected ulong GetID() {
        unsafe {
            Callbacks.GetID(IDComponentPtr, out ulong id);
            return id;
        }
    }
    protected string GetName() {
        unsafe {
            Callbacks.GetName(IDComponentPtr, out char* chars);
            return Marshal.PtrToStringUni((IntPtr)chars);
        }
    }

    protected void SetPosition(float x, float y, float z) {
        unsafe {
            Callbacks.SetTransformPosition(TransformComponentPtr, x, y, z);
        }
    }

    protected void GetPosition(out float x, out float y, out float z) {
        unsafe {
            Callbacks.GetTransformPosition(TransformComponentPtr, out x, out y, out z);
        }
    }

    protected float PositionX {
        get {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out float x, out _, out _);
                return x;
            }
        }
        set {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out _, out float y, out float z);
                Callbacks.SetTransformPosition(TransformComponentPtr, value, y, z);
            }
        }
    }
    protected float PositionY {
        get {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out _, out float y, out _);
                return y;
            }
        }
        set {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out float x, out _, out float z);
                Callbacks.SetTransformPosition(TransformComponentPtr, x, value, z);
            }
        }
    }
    protected float PositionZ {
        get {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out _, out _, out float z);
                return z;
            }
        }
        set {
            unsafe {
                Callbacks.GetTransformPosition(TransformComponentPtr, out float x, out float y, out _);
                Callbacks.SetTransformPosition(TransformComponentPtr, x, y, value);
            }
        }
    }



    static protected bool IsKeyPressed(Key key) {
        unsafe {
            Callbacks.IsKeyPressed((ushort) key, out bool pressed);
            return pressed;
        }
    }

    static protected bool IsMouseButtonPressed(MouseButton button) {
        unsafe {
            Callbacks.IsMouseButtonPressed((ushort)button, out bool pressed);
            return pressed;
        }
    }

    static protected void GetMousePosition(out float x, out float y) {
        unsafe {
            Callbacks.GetMousePosition(out x, out y);
        }
    }


    /// <summary> 
    /// Called once when the script instance is created
    /// Used for initialization
    /// </summary>
    public virtual void OnCreate() { }

    /// <summary>
    /// Called every frame during scene update
    /// </summary>
    /// <param name="deltaTime">Time elapsed since last frame in seconds</param>
    public virtual void OnUpdate(float deltaTime) { }

    /// <summary>
    /// Called on the first frame that the key is pressed
    /// </summary>
    /// <param name="key">KeyCode of the pressed key</param>
    public virtual void OnKeyPressed(Key key) { }

    /// <summary>
    /// Called on the first frame that the key is released
    /// </summary>
    /// <param name="key">KeyCode of the released key</param>
    public virtual void OnKeyReleased(Key key) { }

    /// <summary>
    /// Called on the first frame that the mouse button is pressed
    /// </summary>
    /// <param name="mouseButton">KeyCode of the pressed mouse button</param>
    public virtual void OnMousePressed(MouseButton mouseButton) { }

    /// <summary>
    /// Called on the first frame that the mouse button is released
    /// </summary>
    /// <param name="mouseButton">KeyCode of the released mouse button</param>
    public virtual void OnMouseReleased(MouseButton mouseButton) { }

    /// <summary>
    /// Called each frame the mouse position changes
    /// </summary>
    /// <param name="x">X coordinate of the mouse's new position</param>
    /// <param name="y">Y coordinate of the mouse's new position</param>
    public virtual void OnMouseMoved(float x, float y) { }

    /// <summary>
    /// Called each frame that the mouse is scrolled
    /// </summary>
    /// <param name="xOffset">X coordinate of the scroll delta vector</param>
    /// <param name="yOffset">Y coordinate of the scroll delta vector</param>
    public virtual void OnMouseScrolled(float xOffset, float yOffset) { }

    /// <summary>
    /// Called when the script instance is being destroyed
    /// Used for cleanup
    /// </summary>
    public virtual void OnDestroy() { }
}
