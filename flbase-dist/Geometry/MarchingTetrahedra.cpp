#include "MarchingTetrahedra.h"
#include <QtOpenGL>

MarchingTetrahedra::MarchingTetrahedra( const BoundingBox& boundingBox, unsigned int nbCubeX, unsigned int nbCubeY, unsigned int nbCubeZ )
    : _boundingBox( boundingBox )
{
    QVector3D boxExtent = boundingBox.maximum() - boundingBox.minimum();

    _nbCubes[0] = nbCubeX;
    _nbCubes[1] = nbCubeY;
    _nbCubes[2] = nbCubeZ;
    _cubeSize[0] = boxExtent.x() / _nbCubes[0];
    _cubeSize[1] = boxExtent.y() / _nbCubes[1];
    _cubeSize[2] = boxExtent.z() / _nbCubes[2];

    // Allocate vertex value and position vector
    unsigned int nbCubes = ( nbCubeX + 1 ) * ( nbCubeY + 1 ) * ( nbCubeZ + 1 );
    _vertexValues.resize( nbCubes );
    _vertexNormals.resize( nbCubes );
    _vertexPositions.resize( nbCubes );

    computeVertexPositions();
}

void MarchingTetrahedra::render( const QMatrix4x4& transformation, GLShader& shader, ImplicitSurface& implicitSurface )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // Ceci est la fonction d'entrée pour débuter le
    // calcul de la surface implicite. Il s'agit de
    // calculer les valeurs et les normales aux sommets
    // (computeVertexInfo) et de faire le rendu de chacun
    // des cubes de la grille (renderCube).
    //
    // Le nombre de cubes en:
    // x: _nbCubes[0]
    // y: _nbCubes[1]
    // z: _nbCubes[2]
    ////////////////////////////////////////////////////

    computeVertexInfo(implicitSurface);

    // Gather triangles
    _nbGLVertices = 0;

    // Rendu de chacun des cubes (i.e. remplissage de la liste des triangles)
    for ( unsigned int z=0 ; z<_nbCubes[2] ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1] ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0] ; ++x )
                renderCube(x, y, z);

    // Send it to OpenGL
    renderTriangles( transformation, shader );
}

void MarchingTetrahedra::computeVertexPositions()
{
    unsigned int currentVertex = 0;

    // Precompute the position of each vertex of the grid
    for ( unsigned int z=0 ; z<_nbCubes[2]+1 ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]+1 ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]+1 ; ++x, ++currentVertex )
                _vertexPositions[currentVertex] = vertexPosition( x, y, z );
}

void MarchingTetrahedra::computeVertexInfo( ImplicitSurface& implicitSurface )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // Pour chaque sommet de la grille, remplir les
    // variables membres '_vertexValues' et '_vertexNormals'
    // à l'aide de la position du vertex '_vertexPositions'
    // et de la classe 'implicitSurface'.
    //
    // Les tableaux sont indexés par un seul nombre. Utilisez
    // 'vertexIndex' ou incrémentez une variable manuellement.
    // Le nombre de sommets en:
    // x: '_nbCubes[0]+1'
    // y: '_nbCubes[1]+1'
    // z: '_nbCubes[2]+1'
    // Si vous utilisez une variable que vous incrémentez
    // manuellement, portez bien attention à l'ordre d'imbrication
    // des boucles pour qu'elles correspondent bien à la
    // la fonction 'vertexIndex'.
    ////////////////////////////////////////////////////

    for ( unsigned int z=0 ; z<_nbCubes[2]+1 ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]+1 ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]+1 ; ++x ) {
                unsigned int index = vertexIndex(x, y, z);
                implicitSurface.surfaceInfo(_vertexPositions[index], _vertexValues[index], _vertexNormals[index]);
            }
}

void MarchingTetrahedra::renderCube( unsigned int x, unsigned int y, unsigned int z )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // Divisez votre cube en six tétraèdres en utilisant
    // les sommets du cube, et faire appel à 'renderTetrahedron'
    // pour le rendu de chacun d'eux. Il faut calculer l'index
    // de chaque sommet, et non leurs valeurs (x,y,z) entières.
    ////////////////////////////////////////////////////

    // 8 coins du cube
    unsigned int rbl = vertexIndex(x, y, z);
    unsigned int rtl = vertexIndex(x,y+1,z);
    unsigned int rtr = vertexIndex(x,y+1,z+1);
    unsigned int rbr = vertexIndex(x,y,z+1);
    unsigned int ftl = vertexIndex(x+1,y+1,z);
    unsigned int fbl = vertexIndex(x+1,y,z);
    unsigned int ftr = vertexIndex(x+1,y+1,z+1);
    unsigned int fbr = vertexIndex(x+1,y,z+1);

    //6 tetrahedrons
    renderTetrahedron(rtl,rtr,ftr,fbr);
    renderTetrahedron(rtr,rbr,fbr,rtl);
    renderTetrahedron(rbr,fbr,rbl,rtl);
    renderTetrahedron(fbr,fbl,ftl,rtl);
    renderTetrahedron(fbr,fbl,rbl,rtl);
    renderTetrahedron(ftl,ftr,rtl,fbr);
}

void MarchingTetrahedra::renderTetrahedron( unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4 )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // En utilisant les valeurs aux sommets, voyez dans
    // quel cas de rendu vous vous trouvez. Faites appel
    // à 'renderTriangle' ou 'renderQuad' dépendant du cas
    // et réordonnancez les sommets pour que le (ou les)
    // sommet de signe différent soit au début. Les noms
    // de paramètres 'in' et 'out' ne sont que des indicateurs
    // pour différencier les signes, et non un indicateur
    // absolu de ce qui est à l'intérieur du liquide ou
    // à l'extérieur.
    ////////////////////////////////////////////////////

    float value1 = _vertexValues[p1];
    float value2 = _vertexValues[p2];
    float value3 = _vertexValues[p3];
    float value4 = _vertexValues[p4];

    //Attribution des signes
    int sign1 = ((value1 > 0) ? 1 : -1);
    int sign2 = ((value2 > 0) ? 1 : -1);
    int sign3 = ((value3 > 0) ? 1 : -1);
    int sign4 = ((value4 > 0) ? 1 : -1);

    //8 cas a evaluer avec la parite
    //7 puisqu'on ne fait rien pour le cas trivial
    //Evaluation de tous les cas possibles a traiter pour renderTriangle
    if ((sign1 != sign2) && (sign1 != sign3) && (sign1 != sign4)) {
        renderTriangle(p1, p2, p3, p4);
        return;
    }
    if ((sign2 != sign1) && (sign2 != sign3) && (sign2 != sign4)) {
        renderTriangle(p2, p1, p3, p4);
        return;
    }
    if ((sign3 != sign1) && (sign3 != sign2) && (sign3 != sign4)) {
        renderTriangle(p3, p1, p2, p4);
        return;
    }
    if ((sign4 != sign1) && (sign4 != sign2) && (sign4 != sign3)) {
        renderTriangle(p4, p1, p2, p3);
        return;
    }

    // Evaluation de tous les cas possibles a traiter pour renderQuad
    if ((sign1 == sign2) && (sign1 != sign3) && (sign1 != sign4)) {
        renderQuad(p1, p2, p3, p4);
        return;
    }
    if ((sign1 == sign3) && (sign1 != sign2) && (sign1 != sign4)) {
        renderQuad(p1, p3, p2, p4);
        return;
    }
    if ((sign1 == sign4) && (sign1 != sign2) && (sign1 != sign3)) {
        renderQuad(p1, p4, p2, p3);
        return;
    }
}

void MarchingTetrahedra::renderTriangle( unsigned int in1, unsigned int out2, unsigned int out3, unsigned int out4 )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // Calculez l'interpolation des valeurs et des normales
    // pour les arêtes dont les sommets sont de signes
    // différents. N'oubliez pas de normaliser vos normales.
    //
    // Le triangle n'est pas rendu tout de suite. Il vous
    // faudra l'ajouter à la liste des triangles affichés
    // en utilisant la méthode 'addTriangle'
    ////////////////////////////////////////////////////

    //Extraction des vecteurs position
    QVector3D vec1 = _vertexPositions[in1];
    QVector3D vec2 = _vertexPositions[out2];
    QVector3D vec3 = _vertexPositions[out3];
    QVector3D vec4 = _vertexPositions[out4];

    //Extraction des normales
    QVector3D vecn1 =  _vertexNormals[in1];
    QVector3D vecn2 =  _vertexNormals[out2];
    QVector3D vecn3 =  _vertexNormals[out3];
    QVector3D vecn4 =  _vertexNormals[out4];

    //Interpolation lineaire pour obtenir le triangle
    QVector3D p0 = vec1 + (vec2 - vec1).normalized() * (vec2 - vec1).length() * (-_vertexValues[in1]/(_vertexValues[out2]-_vertexValues[in1]));
    QVector3D p1 = vec1 + (vec3 - vec1).normalized() * (vec3 - vec1).length() * (-_vertexValues[in1]/(_vertexValues[out3]-_vertexValues[in1]));
    QVector3D p2 = vec1 + (vec4 - vec1).normalized() * (vec4 - vec1).length() * (-_vertexValues[in1]/(_vertexValues[out4]-_vertexValues[in1]));

    //Interpolation des normales
    QVector3D n0 = vecn1 + (vecn2 - vecn1).normalized() * (vecn2 - vecn1).length() * (-_vertexValues[in1]/(_vertexValues[out2]-_vertexValues[in1]));
    QVector3D n1 = vecn1 + (vecn3 - vecn1).normalized() * (vecn3 - vecn1).length() * (-_vertexValues[in1]/(_vertexValues[out3]-_vertexValues[in1]));
    QVector3D n2 = vecn1 + (vecn4 - vecn1).normalized() * (vecn4 - vecn1).length() * (-_vertexValues[in1]/(_vertexValues[out4]-_vertexValues[in1]));

    //Normalisation des normales
    n0.normalize();
    n1.normalize();
    n2.normalize();

    //Ajout du triangle
    addTriangle(p0, p1, p2, n0, n1, n2);
}

void MarchingTetrahedra::renderQuad( unsigned int in1, unsigned int in2, unsigned int out3, unsigned int out4 )
{
    ////////////////////////////////////////////////////
    // IFT3355 - À compléter
    //
    // Calculer l'interpolation des valeurs et des normales
    // pour les arêtes dont les sommets sont de signes
    // différents. Vous aurez quatre sommets. Séparez le
    // quadrilatère en deux triangles et les ajouter à
    // la liste avec 'addTriangle'.
    ////////////////////////////////////////////////////

    //Extraction des vecteurs position
    QVector3D vec1 = _vertexPositions[in1];
    QVector3D vec2 = _vertexPositions[in2];
    QVector3D vec3 = _vertexPositions[out3];
    QVector3D vec4 = _vertexPositions[out4];

    //Extraction des normales
    QVector3D vecn1 = _vertexNormals[in1];
    QVector3D vecn2 = _vertexNormals[in2];
    QVector3D vecn3 = _vertexNormals[out3];
    QVector3D vecn4 = _vertexNormals[out4];

    //Interpolation lineaire pour obtenir le rectangle
    QVector3D p0 = vec1 + (vec3 - vec1).normalized() * (vec3 - vec1).length() * (-_vertexValues[in1]/(_vertexValues[out3]-_vertexValues[in1]));
    QVector3D p1 = vec1 + (vec4 - vec1).normalized() * (vec4 - vec1).length() * (-_vertexValues[in1]/(_vertexValues[out4]-_vertexValues[in1]));
    QVector3D p2 = vec2 + (vec3 - vec2).normalized() * (vec3 - vec2).length() * (-_vertexValues[in2]/(_vertexValues[out3]-_vertexValues[in2]));
    QVector3D p3 = vec2 + (vec4 - vec2).normalized() * (vec4 - vec2).length() * (-_vertexValues[in2]/(_vertexValues[out4]-_vertexValues[in2]));

    //Interpolation des normales
    QVector3D n0 = vecn1 + (vecn3 - vecn1).normalized() * (vecn3 - vecn1).length() * (-_vertexValues[in1]/(_vertexValues[out3]-_vertexValues[in1]));
    QVector3D n1 = vecn1 + (vecn4 - vecn1).normalized() * (vecn4 - vecn1).length() * (-_vertexValues[in1]/(_vertexValues[out4]-_vertexValues[in1]));
    QVector3D n2 = vecn2 + (vecn3 - vecn2).normalized() * (vecn3 - vecn2).length() * (-_vertexValues[in2]/(_vertexValues[out3]-_vertexValues[in2]));
    QVector3D n3 = vecn2 + (vecn4 - vecn2).normalized() * (vecn4 - vecn2).length() * (-_vertexValues[in2]/(_vertexValues[out4]-_vertexValues[in2]));

    //Normalisation des normales
    n0.normalize();
    n1.normalize();
    n2.normalize();
    n3.normalize();

    //Ajout des triangles
    addTriangle(p0, p1, p2, n0, n1, n2);
    addTriangle(p1, p2, p3, n1, n2, n3);

}

QVector3D MarchingTetrahedra::vertexPosition( unsigned int x, unsigned int y, unsigned int z ) const
{
    return _boundingBox.minimum() + QVector3D( x * _cubeSize[0], y * _cubeSize[1], z * _cubeSize[2] );
}

unsigned int MarchingTetrahedra::vertexIndex( unsigned int x, unsigned int y, unsigned int z ) const
{
    return z * ( _nbCubes[0] + 1 ) * ( _nbCubes[1] + 1 ) + y * ( _nbCubes[0] + 1 ) + x;
}

void MarchingTetrahedra::addTriangle( const QVector3D& p0, const QVector3D& p1, const QVector3D& p2,
                                      const QVector3D& n0, const QVector3D& n1, const QVector3D& n2 )
{
    if ( _glVertices.size() <= _nbGLVertices )
    {
        _glVertices.resize( _glVertices.size() + 192 );
        _glNormals.resize( _glNormals.size() + 192 );
    }

    _glVertices[_nbGLVertices+0] = p0;
    _glVertices[_nbGLVertices+1] = p1;
    _glVertices[_nbGLVertices+2] = p2;
    _glNormals[_nbGLVertices+0] = n0;
    _glNormals[_nbGLVertices+1] = n1;
    _glNormals[_nbGLVertices+2] = n2;
    _nbGLVertices += 3;
}

void MarchingTetrahedra::renderTriangles( const QMatrix4x4& transformation, GLShader& shader )
{
    shader.setGlobalTransformation( transformation );

    shader.enableVertexAttributeArray();
    shader.enableNormalAttributeArray();
    shader.setVertexAttributeArray( _glVertices.data() );
    shader.setNormalAttributeArray( _glNormals.data() );

    glDrawArrays( GL_TRIANGLES, 0, _nbGLVertices );

    shader.disableVertexAttributeArray();
    shader.disableNormalAttributeArray();
}




