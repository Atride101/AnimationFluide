#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>

/* A diffuse/environment/refraction material
 */

class Material
{
public:
    Material(); // Cubemap constructor
    Material( const QColor& diffuse ); // Diffuse constructor
    Material( float refractiveIndex ); // Refraction constructor

    bool isUsingCubemap() const;
    const QColor& diffuse() const;
    float refractiveIndex() const;

private:
    bool _isUsingCubemap;
    QColor _diffuse;
    float _refractiveIndex;
};

#endif // MATERIAL_H
