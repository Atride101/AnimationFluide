#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QImage>
#include <QString>

/* Cubemap/Environment/Skybox. In other words : 6 textures : one for
 * each face of a cube.
 */

class CubeMap
{
public:
    CubeMap( const QString& path );

    void initialize();
    void bind();
    void release();

private:
    QImage _images[6];
    unsigned int _textureId;
};

#endif // CUBEMAP_H
