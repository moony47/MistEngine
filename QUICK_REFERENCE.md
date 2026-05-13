# C# Scripting Quick Reference Card

## One-Minute Summary

✅ **What**: C# scripting system for Mistwraith  
✅ **Status**: Phase 1 Complete (foundation ready)  
✅ **How It Works**: Users write C#, engine loads DLL, calls lifecycle methods  
✅ **Next**: Phase 2 - Hosting API (load/instantiate scripts dynamically)

---

## Creating Your First Script

### 1. Create File
```
ScriptEngine/MyScript.cs
```

### 2. Write Code
```csharp
public class MyScript : ManagedScript
{
    public override void OnUpdate(float deltaTime)
    {
        // Your game logic
    }
}
```

### 3. Compile
```bash
cd D:\dev\MistEngine\ScriptEngine
dotnet build -c Release
```

### 4. Output
```
ScriptEngine.dll created in bin/Release/net8.0/
```

---

## Lifecycle Methods

| Method | Called When | Use For |
|--------|-------------|---------|
| **OnCreate()** | Script instance created | Initialize, setup |
| **OnUpdate(float deltaTime)** | Every frame | Game logic, input, animation |
| **OnDestroy()** | Script destroyed | Cleanup, save state |

All optional (can override only what you need).

---

## File Structure

```
ScriptEngine/
├── ManagedScript.cs              ← Base class (inherit this)
├── ScriptEngine.csproj           ← .NET 8 project config
├── bin/Release/net8.0/
│   └── ScriptEngine.dll          ← Output you care about
└── Examples/
    └── SimpleMovement.cs         ← Template to copy
```

---

## C# Side (What You Write)

```csharp
using System;

public class GameLogic : ManagedScript
{
    private float timer = 0f;

    public override void OnCreate()
    {
        Console.WriteLine("Game started!");
    }

    public override void OnUpdate(float deltaTime)
    {
        timer += deltaTime;
    }

    public override void OnDestroy()
    {
        Console.WriteLine("Game ended!");
    }
}
```

---

## C++ Side (Engine Integration)

```cpp
// In Components.h
struct ManagedScriptComponent {
    ManagedScript* Instance = nullptr;
    std::string ScriptClassName;
    // ...
};

// In Entity
Entity entity = scene.CreateEntity("Player");
auto& script = entity.AddComponent<ManagedScriptComponent>();
script.ScriptClassName = "GameLogic";

// In Scene::OnUpdate
if (entity.HasComponent<ManagedScriptComponent>()) {
    auto& script = entity.GetComponent<ManagedScriptComponent>();
    if (script.Instance) {
        script.Instance->OnUpdate(deltaTime);
    }
}
```

---

## Build Commands

```bash
# Build C# assembly (Release)
cd D:\dev\MistEngine\ScriptEngine
dotnet build -c Release

# Build C# assembly (Debug)
dotnet build -c Debug

# Clean everything
dotnet clean

# Check .NET version
dotnet --version
```

---

## Verify Output

```bash
# Check DLL exists
Get-ChildItem D:\dev\MistEngine\ScriptEngine\bin\Release\net8.0\ScriptEngine.dll

# Check DLL size (should be ~4 KB)
(Get-ChildItem D:\dev\MistEngine\ScriptEngine\bin\Release\net8.0\ScriptEngine.dll).Length
```

---

## What You Get (Phase 1)

✅ Full C# 12 language support  
✅ Modern async/await patterns  
✅ LINQ, records, pattern matching  
✅ Static typing with IntelliSense  
✅ Exception handling  
✅ Debugging (manual Console.WriteLine for now)  

---

## What's Coming (Phase 2+)

⏳ Dynamic script loading (no recompile)  
⏳ Access Transform component  
⏳ Access SpriteComponent  
⏳ Input handling  
⏳ Physics queries  
⏳ Scene queries  
⏳ Hot-reload support  
⏳ Debugger integration  

---

## Common Questions

**Q: Do I need to know C++?**  
A: No, just write C#.

**Q: What if I make a mistake?**  
A: Compiler catches it, fix it, rebuild.

**Q: Where do I put my script?**  
A: ScriptEngine/ folder, any subfolder is fine.

**Q: How do I debug?**  
A: Use Console.WriteLine() for now. Debugger coming in Phase 2.

**Q: Can I use external libraries?**  
A: Yes, add NuGet packages to ScriptEngine.csproj

**Q: How do I access the entity?**  
A: Coming in Phase 2 - engine API exposure.

---

## Quick Checklist

- [ ] .NET 8 SDK installed (`dotnet --version`)
- [ ] Visual Studio 2026 Community Edition
- [ ] Can run `dotnet build` successfully
- [ ] ScriptEngine.dll generates in bin/Release/net8.0/
- [ ] C++ project builds without errors
- [ ] Ready to write first script

---

## Resources

**Documentation**
- `ScriptEngine/README.md` — Full guide
- `CSHARP_SCRIPTING_PROGRESS.md` — Implementation details
- `CSHARP_IMPLEMENTATION_SUMMARY.md` — Architecture overview

**Examples**
- `ScriptEngine/Examples/SimpleMovement.cs` — Template
- `ScriptEngine/MINIMUM_EXAMPLE.cs` — Minimal examples

**Reference**
- `ManagedScript.cs` — Base class source
- `ManagedScript.h` — C++ counterpart

---

## Support Quick Links

**If C# build fails:**
1. Check: `dotnet --version` (should be 8.x)
2. Run: `dotnet clean && dotnet build`
3. Check: No syntax errors in .cs files

**If C++ build fails:**
1. Make sure C# compiles first
2. Check: Include paths in Visual Studio
3. Try: Rebuild Solution

**If script doesn't load (Phase 2+):**
1. Check: Class name matches exactly (case-sensitive)
2. Check: Inherits from ManagedScript
3. Check: DLL is compiled

---

## Key Files Reference

| File | Purpose | You Need It? |
|------|---------|-------------|
| ManagedScript.cs | Base class | Read to understand |
| ScriptEngine.csproj | Build config | Rarely edit |
| SimpleMovement.cs | Example | Copy as template |
| ScriptEngine.dll | Output | Just runs |

---

## Phase Recap

**Phase 1: Foundation** ✅ DONE
- C# base class
- Build system
- C++ integration
- Architecture ready

**Phase 2: Hosting** ⏳ TODO
- Load DLL dynamically
- Instantiate scripts
- Call methods each frame

**Phase 3+: Features** 🔮 FUTURE
- Engine API access
- Hot reload
- Debugging

---

## Success!

You have a working C# scripting system ready for development.

```
✅ Write C# scripts
✅ Compile to DLL
✅ Engine is ready to load them
⏳ Phase 2 will complete the integration
```

**Get started**: Copy `SimpleMovement.cs`, rename it, modify it, build!

---

**Last Updated**: April 21, 2026  
**Version**: Phase 1 Complete  
**Status**: Ready for use ✅
