#include "Blueprint.h"

Blueprint::Blueprint(const std::string_view& name) :
    m_name(name)
{ 
    m_nameHash = std::hash<std::string_view>()(m_name);
}

const std::string_view& Blueprint::get_name() const
{
    return m_name;
}

AABoundingBox<> Blueprint::get_bounds() const
{
    return m_boundingBox;
}

Mesh<>& Blueprint::mesh()
{
    return m_mesh;
}