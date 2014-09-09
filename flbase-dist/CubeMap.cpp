#include "CubeMap.h"
#include <QtOpenGL>

#if !defined(GL_TEXTURE_CUBE_MAP_ARB)
#  define GL_TEXTURE_CUBE_MAP_ARB             0x8513
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB  0x8515
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB  0x8516
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB  0x8517
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB  0x8518
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB  0x8519
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB  0x851A
#endif

#if !defined(GL_BGRA)
#define GL_BGRA 0x80E1
#endif

namespace
{
    GLenum cubeMapTarget[6] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
    };

    QString fileExtensions[6] =
    {
        QString( "XN.png" ),
        QString( "XP.png" ),
        QString( "YN.png" ),
        QString( "YP.png" ),
        QString( "ZN.png" ),
        QString( "ZP.png" )
    };
}

CubeMap::CubeMap( const QString& path )
    : _textureId( 0 )
{
    for ( unsigned int i=0 ; i<6 ; ++i )
    {
        QString filename = path + fileExtensions[i];
        _images[i] = QImage( filename ).convertToFormat( QImage::Format_ARGB32 );
    }
}

void CubeMap::initialize()
{
    glGenTextures( 1, &_textureId );
    glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, _textureId );

    for ( unsigned int i=0 ; i<6 ; ++i )
        glTexImage2D( cubeMapTarget[i], 0, GL_RGB, _images[i].width(), _images[i].height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, _images[i].bits() );

    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
}

void CubeMap::bind()
{
    glEnable( GL_TEXTURE_CUBE_MAP_ARB );
    glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, _textureId );
}

void CubeMap::release()
{
    glDisable( GL_TEXTURE_CUBE_MAP_ARB );
}
