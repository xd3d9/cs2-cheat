#include "cvar.hpp"
#define NOMINMAX
#include "./fnv1a.hpp"
#include "./virtual.hpp"
#include <windows.h>
#define SDK_LOG_PTR(x) reinterpret_cast<void*>(x)

std::uint8_t* raehaerhaefb(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}

CCVar* get_interfaceasdasdasd(const wchar_t* module_name, const char* interface_name)
{
	if (!module_name || !interface_name)
	{
		return nullptr;
	}

	HMODULE module = GetModuleHandle(module_name);

	if (!module)
	{
		return nullptr;
	}

	std::uint8_t* create_interface = reinterpret_cast<std::uint8_t*>(GetProcAddress(module, "CreateInterface"));

	if (!create_interface)
	{
		return nullptr;
	}

	using interface_callback_fn = void* (__cdecl*)();

	typedef struct _interface_reg_t
	{
		interface_callback_fn callback;
		const char* name;
		_interface_reg_t* flink;
	} interface_reg_t;

	interface_reg_t* interface_list = *reinterpret_cast<interface_reg_t**>(raehaerhaefb(create_interface, 3, 7));

	if (!interface_list)
	{
		return nullptr;
	}

	for (interface_reg_t* it = interface_list; it; it = it->flink)
	{
		if (!strcmp(it->name, interface_name))
		{
			return reinterpret_cast<CCVar*>(it->callback());
		}
	}

	return nullptr;
}

CCVar* CCVar::Get() {
	static const auto inst = get_interfaceasdasdasd(L"tier0.dll", "VEngineCvar007");
    return inst;
}

CCVar::CVarIterator_t CCVar::GetFirstCvarIterator() {
    CVarIterator_t iterator = -1;

#ifdef _WIN32
    vt::CallMethod<void>(this, 12, &iterator);
#elif __linux__
    iterator = vt::CallMethod<CVarIterator_t>(this, 12);
#endif

    return iterator;
}

CCVar::CVarIterator_t CCVar::GetNextCvarIterator(CVarIterator_t prev) {
    CVarIterator_t iterator = prev;

#ifdef _WIN32
    vt::CallMethod<void>(this, 13, &iterator, prev);
#elif __linux__
    iterator = vt::CallMethod<CVarIterator_t>(this, 13, prev);
#endif

    return iterator;
}

ConVar* CCVar::GetCvarByIndex(CVarIterator_t index) { return vt::CallMethod<ConVar*>(this, 37, index); }

ConVar* CCVar::GetCvarByName(const char* name) {
    const uint32_t hash = FNV1A::fnv1a<uint32_t>::hash(name);

    CVarIterator_t it = GetFirstCvarIterator();
    do {
        ConVar* cvar = GetCvarByIndex(it);

        if (cvar && FNV1A::fnv1a<uint32_t>::hash(cvar->m_Name) == hash) {
            return cvar;
        }

        it = GetNextCvarIterator(it);
    } while (it != static_cast<CVarIterator_t>(-1));

    return nullptr;
}