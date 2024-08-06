#include "./schema_system.hpp"

#include "sdk/interfaces/i_schema_system.hpp"

#include "fnv1a.hpp"
#include <optional>

std::optional<int32_t> CSchemaManager::GetSchemaOffsetInternal(const char* moduleName, const char* bindingName, const char* fieldName) {
    CSchemaSystemTypeScope* typeScope = CSchemaSystem::Get()->FindTypeScopeForModule(moduleName);
    if (!typeScope) {
        return {};
    }

    CSchemaClassInfo* classInfo = typeScope->FindDeclaredClass(bindingName);
    if (!classInfo) {
        return {};
    }

    uint32_t fieldHash = FNV1A::fnv1a<uint32_t>::hash(fieldName);
    for (int i = 0; classInfo->m_Fields && i < classInfo->m_nFieldsCount; ++i) {
        auto& field = classInfo->m_Fields[i];
        if (FNV1A::fnv1a<uint32_t>::hash(field.m_Name) == fieldHash) {
            return field.m_nSingleInheritanceOffset;
        }
    }

    return {};
}