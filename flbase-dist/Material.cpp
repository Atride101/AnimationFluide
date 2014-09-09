#include "Material.h"

Material::Material()
    : _isUsingCubemap( true )
    , _refractiveIndex( 1 )
{
}

Material::Material( const QColor& diffuse )
    : _isUsingCubemap( false )
    , _diffuse( diffuse )
    , _refractiveIndex( 1 )
{
}

Material::Material( float refractiveIndex )
    : _isUsingCubemap( false )
    , _refractiveIndex( refractiveIndex )
{
}

bool Material::isUsingCubemap() const
{
    return _isUsingCubemap;
}

const QColor& Material::diffuse() const
{
    return _diffuse;
}

float Material::refractiveIndex() const
{
    return _refractiveIndex;
}
