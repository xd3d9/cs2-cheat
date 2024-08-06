#include "sdk/interfaces/interfaces.hpp"

struct ResourceBinding_t
{
	void* pData;
};

template <typename T>
class CStrongHandle
{
public:
	operator T* () const
	{
		if (pBinding == nullptr)
			return nullptr;

		return static_cast<T*>(pBinding->pData);
	}

	T* operator->() const
	{
		if (pBinding == nullptr)
			return nullptr;

		return static_cast<T*>(pBinding->pData);
	}

	const ResourceBinding_t* pBinding;
};

namespace materials
{
	void LoadAllMaterials();
	void CreateMaterial(const char* szName);
	void CreateMaterialInvis(const char* szName);
	inline CStrongHandle<CMaterial2> visible_material;
	inline CStrongHandle<CMaterial2> invisible_material;

}

namespace chams_material
{
	void LoadChamsMaterial(); //const char* material_name, CMaterial2** oMaterial
	void LoadChamsMaterialInvisible();

	inline CMaterial2** visible_material;
	inline CMaterial2** invisible_material;
}