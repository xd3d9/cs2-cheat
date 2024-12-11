#include "load_materials.hpp"

#include <malloc.h>


void chams_material::LoadChamsMaterial() {
	CMeshData* pData = reinterpret_cast<CMeshData*>(_malloca(0x1050));

	CMaterial2** material_prototype;

	interfaces::material_system->FindOrCreateFromResource(&material_prototype, "materials/dev/primary_white.vmat");
	interfaces::material_system->SetCreateDataByMaterial(pData, &material_prototype);
	/**/
	pData->SetShaderType("csgo_unlitgeneric.vfx");
	pData->SetMaterialFunction("F_DISABLE_Z_BUFFERING", 1);
	pData->SetMaterialFunction("F_BLEND_MODE", 1);
	pData->SetMaterialFunction("F_TRANSLUCENT", 1);

	//CMaterial2** custom_material;

	interfaces::material_system->CreateMaterial(&chams_material::visible_material, "visible", pData);
	pData->SetMaterialFunction("F_TRANSLUCENT", 0);
	//interfaces::material_system->CreateMaterial(&chams::invisible_material, "invisible", pData);
	//chams::invisible_material = material_prototype;
}

void chams_material::LoadChamsMaterialInvisible() {
	CMeshData* pData = reinterpret_cast<CMeshData*>(_malloca(0x1050));

	CMaterial2** material_prototype;

	interfaces::material_system->FindOrCreateFromResource(&material_prototype, "materials/dev/primary_white.vmat");
	interfaces::material_system->SetCreateDataByMaterial(pData, &material_prototype);
	/**/
	pData->SetShaderType("csgo_unlitgeneric.vfx");
	pData->SetMaterialFunction("F_BLEND_MODE", 1);
	pData->SetMaterialFunction("F_TRANSLUCENT", 1);

	//CMaterial2** custom_material;

	interfaces::material_system->CreateMaterial(&chams_material::invisible_material, "invisible", pData);
	//interfaces::material_system->CreateMaterial(&chams::invisible_material, "invisible", pData);
	//chams::invisible_material = material_prototype;
}


__inline void* MemorySet(void* pDestination, const std::uint8_t uByte, std::size_t nCount)
{
#ifdef CS_COMPILER_MSC
	// @test: clang always tries to detect 'memset' like instructions and replace them with CRT's function call
	if (const std::size_t nCountAlign = (nCount & 3U); nCountAlign == 0U)
	{
		auto pDestinationLong = static_cast<unsigned long*>(pDestination);
		__stosd(pDestinationLong, static_cast<unsigned long>(uByte) * 0x01010101, nCount >> 2U);
	}
	else if (nCountAlign == 2U)
	{
		auto pDestinationWord = static_cast<unsigned short*>(pDestination);
		__stosw(pDestinationWord, static_cast<unsigned short>(uByte | (uByte << 8U)), nCount >> 1U);
	}
	else
	{
		auto pDestinationByte = static_cast<unsigned char*>(pDestination);
		__stosb(pDestinationByte, uByte, nCount);
	}
#else
	auto pDestinationByte = static_cast<std::uint8_t*>(pDestination);

	while (nCount--)
		*pDestinationByte++ = uByte;
#endif

	return pDestination;
}

static CKeyValues3* CreateMaterialResource()
{
	using fnSetTypeKV3 = CKeyValues3 * (__fastcall*)(CKeyValues3*, unsigned int, unsigned int);
	static const fnSetTypeKV3 oSetTypeKV3 = reinterpret_cast<fnSetTypeKV3>(scanner::pattern_scan(L"client.dll", "E8 ? ? ? ? 4C 89 7B 30"));//

#ifdef CS_PARANOID
	CS_ASSERT(oSetTypeKV3 != nullptr);
#endif

	CKeyValues3* pKeyValue = new CKeyValues3[0x10];
	return oSetTypeKV3(pKeyValue, 1U, 6U);
}

void materials::CreateMaterial(const char* szName)
{
	const char szVmatBuffer[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    Shader = "csgo_unlitgeneric.vfx"
    g_flBumpStrength = 1
	F_BLEND_MODE = 1
	F_TRANSLUCENT = 1
    g_vColorTint = [1.000000, 1.000000, 1.000000, 0.000000]
    g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
    g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
    g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
    g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
    g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";
	/*
	auto buffer = new unsigned char[0x100 + sizeof(CKeyValues3)];
	MemorySet(buffer, 0, sizeof(buffer));
	//void* buffer = interfaces::memalloc->IMemAlloc::Alloc(0x100 + sizeof(CKeyValues3));
	//auto pBuffer = static_cast<unsigned char*>(_malloca(0x100 + sizeof(CKeyValues3)));
	//MemorySet(buffer, 0, sizeof(buffer));
	CKeyValues3* kv3 = (CKeyValues3*)(buffer + 0x100);
	*/
	CKeyValues3* pKeyValues3 = CreateMaterialResource();

	KV3ID_t kv3ID;
	kv3ID.szName = szName;
	kv3ID.unk0 = 0x469806E97412167C;
	kv3ID.unk1 = 0xE73790B53EE6F2AF;

	if (!interfaces::fnLoadKV3(pKeyValues3, nullptr, szVmatBuffer, &kv3ID, nullptr))
		throw;

	//CStrongHandle<CMaterial2> custom_material;
	interfaces::fnCreateMaterial(nullptr, &materials::visible_material, szName, pKeyValues3, 0, 1);
	//std::cout << kv3 << " " << custom_material << std::endl;
	//return custom_material;
}

void materials::CreateMaterialInvis(const char* szName)
{
	const char szVmatBuffer[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    Shader = "csgo_unlitgeneric.vfx"
    F_DISABLE_Z_BUFFERING = 1
	F_BLEND_MODE = 1
	F_TRANSLUCENT = 1
    g_flBumpStrength = 1
    g_vColorTint = [1.000000, 1.000000, 1.000000, 0.000000]
    g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
    g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
    g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
    g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
    g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";
	/*
	auto buffer = new unsigned char[0x100 + sizeof(CKeyValues3)];
	MemorySet(buffer, 0, sizeof(buffer));
	CKeyValues3* kv3 = (CKeyValues3*)(buffer + 0x100);
	*/
	CKeyValues3* pKeyValues3 = CreateMaterialResource();

	KV3ID_t kv3ID;
	kv3ID.szName = szName;
	kv3ID.unk0 = 0x469806E97412167C;
	kv3ID.unk1 = 0xE73790B53EE6F2AF;

	if (!interfaces::fnLoadKV3(pKeyValues3, nullptr, szVmatBuffer, &kv3ID, nullptr))
		throw;

	CStrongHandle<CMaterial2> custom_material;
	interfaces::fnCreateMaterial(nullptr, &materials::invisible_material, szName, pKeyValues3, 0, 1);

	//return custom_material;
}

void materials::LoadAllMaterials() {
	/*
	chams::LoadChamsMaterial("visible", chams::visible_material);
	chams::LoadChamsMaterial("invisible", chams::invisible_material);
	*/
	//CreateMaterial("visible");
	materials::CreateMaterialInvis("csgo_unlitgeneric");
	materials::CreateMaterial("csgo_unlitgeneric");
	//chams_material::LoadChamsMaterial();
	//chams_material::LoadChamsMaterialInvisible();
}