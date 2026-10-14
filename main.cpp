// Compact, in-process binding to the two recovered PrelaunchAppManager routines.
// Addresses are RVAs from this dump (preferred image base 0x140000000).
// Use only with the matching 64-bit explorer.exe build and valid in-process objects.
#include <Windows.h>
#include <cstddef>
#include <cstdint>

static_assert(sizeof(void*) == 8, "This binding is for 64-bit explorer.exe only");

namespace explorer {

class PrelaunchAppApi final {
public:
    explicit PrelaunchAppApi(HMODULE module) noexcept
        : imageBase_(reinterpret_cast<std::byte*>(module)) {}

    // Recovered at VA 0x140005EA0. `manager` is the method's this pointer;
    // `candidate` is an opaque COM-like app object from the original process.
    HRESULT AddApp(void* manager, void* candidate) const noexcept {
        if (!imageBase_ || !manager || !candidate) return E_POINTER;
        using Function = HRESULT(__fastcall*)(void*, void*);
        return Resolve<Function>(0x5EA0)(manager, candidate);
    }

    // Recovered at VA 0x140005FEC. Sets `alreadyTracked` to 0 or 1.
    HRESULT IsAppAlreadyTracked(
        void* manager, void* candidate, BOOLEAN* alreadyTracked) const noexcept {
        if (!imageBase_ || !manager || !candidate || !alreadyTracked) return E_POINTER;
        using Function = HRESULT(__fastcall*)(void*, void*, BOOLEAN*);
        return Resolve<Function>(0x5FEC)(manager, candidate, alreadyTracked);
    }

private:
    template<class Function>
    Function Resolve(std::uintptr_t rva) const noexcept {
        return reinterpret_cast<Function>(imageBase_ + rva);
    }

    std::byte* imageBase_;
};

} // namespace explorer

/* Example (call only from the matching explorer.exe process):

HMODULE image = GetModuleHandleW(nullptr);
explorer::PrelaunchAppApi api(image);
BOOLEAN tracked = FALSE;
HRESULT hr = api.IsAppAlreadyTracked(managerObject, appObject, &tracked);

*/
