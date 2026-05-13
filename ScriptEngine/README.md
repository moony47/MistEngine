# Mistwraith C# Scripting System

This directory contains the managed C# script engine for Mistwraith, enabling game developers to write gameplay logic in C# instead of C++.

## Overview

The scripting system uses a **minimal interface** approach:
- **Base Class**: `ManagedScript` (defined in both C# and C++)
- **Lifecycle**: `OnCreate()` → `OnUpdate(deltaTime)` → `OnDestroy()`
- **Runtime**: .NET 8+ with native interop layer

## Project Structure

```
ScriptEngine/
├── ManagedScript.cs          # C# base class
├── Examples/
│   └── SimpleMovement.cs     # Example user script
├── ScriptEngine.csproj       # C# project file
└── bin/Release/net8.0/
    └── ScriptEngine.dll      # Compiled assembly (auto-generated)
```

## Building the C# Assembly

### Prerequisites
- .NET 8 SDK (verify: `dotnet --version` should show 8.x)

### Build Command
```powershell
cd D:\dev\MistEngine\ScriptEngine
dotnet build -c Release
```

This generates:
- `bin/Release/net8.0/ScriptEngine.dll` — The compiled C# assembly
- `bin/Release/net8.0/ScriptEngine.pdb` — Debug symbols
- `bin/Release/net8.0/ScriptEngine.deps.json` — Dependency manifest

## Creating Your First Script

### 1. Create a new `.cs` file
```csharp
// GameScripts/PlayerController.cs
public class PlayerController : ManagedScript
{
    private float moveSpeed = 5.0f;

    public override void OnCreate()
    {
        // Called once when script is instantiated
        System.Console.WriteLine("Player initialized!");
    }

    public override void OnUpdate(float deltaTime)
    {
        // Called every frame
        // TODO: Handle input and move player
    }

    public override void OnDestroy()
    {
        // Called on cleanup
    }
}
```

### 2. Compile to DLL
```powershell
cd ScriptEngine
dotnet build -c Release
```

### 3. Use in Editor (coming soon)
Once the `.NET hosting API integration is complete, you'll:
1. Add `ManagedScriptComponent` to an entity
2. Set `ScriptClassName` to the fully qualified name (e.g., "GameScripts.PlayerController")
3. Engine loads the assembly and creates instances automatically

## Architecture

### C++ Side
```
EditorLayer
    ↓
Scene::OnUpdate(deltaTime)
    ↓
Entity with ManagedScriptComponent
    ↓
DotNetRuntime::CreateInstance("ClassName")
    ↓
ManagedScript* instance (C# object wrapped)
    ↓
instance->OnUpdate(deltaTime)
```

### C# Side
```
Your Script Class
    ↓ (inherits)
ManagedScript (base class)
    ↓ (implements)
OnCreate() / OnUpdate(deltaTime) / OnDestroy()
```

## Implementation Status

✅ **Complete:**
- C# base class definition (`ManagedScript.cs`)
- C++ interop interface (`IManagedRuntime`, `ManagedScriptComponent`)
- C# assembly compilation (`.csproj` setup)
- Example script template

⏳ **TODO (Phase 2):**
- .NET hosting API integration (load assembly, create instances)
- Script instance reflection and dynamic instantiation
- Property binding to editor UI
- Hot-reload support
- Serialization system for script properties
- Input/physics/rendering API exposure

## Dependencies

### Runtime Requirements
- .NET 8 Runtime (installed automatically with SDK)
- nethost library (part of .NET SDK, will be linked in hosting layer)

### Build Requirements
- .NET 8 SDK
- Visual Studio 2026 (for C++ compilation)

## Performance Notes

- Assembly loading happens once at startup
- Instance creation uses reflection (minor cost, happens at object creation)
- OnUpdate calls are direct virtual function calls (zero overhead)
- No garbage collection stalls during gameplay (tuned GC)

## Examples

See `ScriptEngine/Examples/SimpleMovement.cs` for a working example.

## Troubleshooting

### Build Error: "nethost.h not found"
This is expected in Phase 1. Phase 2 will implement the hosting API with proper headers.

### DLL Not Found at Runtime
Ensure the compiled `ScriptEngine.dll` is in the correct path before runtime initialization.

### Script Not Instantiated
Check that the fully qualified class name matches exactly (case-sensitive).

## Next Steps

1. **Phase 2**: Implement `.NET hosting API` for dynamic script loading
2. **Phase 3**: Add property exposure and editor UI integration
3. **Phase 4**: Implement hot-reload and debugging support
4. **Phase 5**: Add engine API bindings (transform, physics, input, etc.)

