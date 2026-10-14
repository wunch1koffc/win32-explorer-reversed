# Windows Explorer PrelaunchAppManager In-Process Binding

A compact, high-performance C++20 header-only binding to the undocumented `PrelaunchAppManager` routines inside the native 64-bit `explorer.exe`. 

This API provides access to the internal Windows shell mechanisms responsible for UWP/Modern app prelaunching (pre-loading applications into memory for faster startup).

## 🚀 Features
* **Zero Overhead:** Direct assembly-level mapping via RVAs (Relative Virtual Addresses).
* **Modern C++:** Wrapped into a clean, typesafe `explorer::PrelaunchAppApi` namespace.
* **In-Process execution:** Tailored for internal tools, DLL injections, and hook development.

## 🛠️ Recovered Routines
* `AddApp (RVA: 0x5EA0)` — In-process routine to manage prelaunched app context.
* `IsAppAlreadyTracked (RVA: 0x5FEC)` — Checks if the application lifecycle is already being managed by the Explorer prelauncher.

## 💻 Usage Example

Inject this logic into the `explorer.exe` address space (via DLL injection or native hooking framework):

```cpp
#include "prelaunch_api.hpp"

void InspectExplorer() {
    // Get the base address of the host explorer.exe process
    HMODULE hExplorer = GetModuleHandleW(nullptr);
    
    explorer::PrelaunchAppApi api(hExplorer);
    BOOLEAN isTracked = FALSE;
    
    // Query internal state
    HRESULT hr = api.IsAppAlreadyTracked(managerObject, appObject, &isTracked);
    if (SUCCEEDED(hr)) {
        if (isTracked) {
            // Your custom logic here
        }
    }
}
```

## ⚠️ Disclaimer
This project targets a specific build of 64-bit `explorer.exe`. Hardcoded RVAs (`0x5EA0`, `0x5FEC`) are version-dependent and might change across different Windows Updates. Use with caution.
