QT += core gui opengl

TARGET = Tp3
TEMPLATE = app
CONFIG += silent

DEFINES += _USE_MATH_DEFINES

CONFIG(debug,debug|release) {
} else {
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS += -O3 -fopenmp
    QMAKE_LFLAGS -= -O1
    QMAKE_LFLAGS += -O3 -fopenmp
}

CONFIG += c++11

QMAKE_CFLAGS += -std=c99

contains(QT_VERSION, ^4.*) {
QMAKE_CXXFLAGS += -std=gnu++0x
}

SOURCES += \
    Geometry/AbstractObject.cpp \
    Geometry/BoundingBox.cpp \
    Geometry/Camera.cpp \
    Geometry/Cube.cpp \
    Geometry/Cylinder.cpp \
    Geometry/Geometry.cpp \
    Geometry/Intersection.cpp \
    Geometry/MarchingTetrahedra.cpp \
    Geometry/Ray.cpp \
    Geometry/Sphere.cpp \
    Scenes/Scene.cpp \
    Scenes/SceneCube.cpp \
    Scenes/SceneCylinder.cpp \
    Scenes/SceneSphere.cpp \
    Scenes/SceneSphereHighRes.cpp \
    SPH/Grid.cpp \
    SPH/Particle.cpp \
    SPH/Particles.cpp \
    SPH/SPH.cpp \
    CubeMap.cpp \
    GLShader.cpp \
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    Material.cpp \
    TimeState.cpp

HEADERS  += \
    Geometry/AbstractObject.h \
    Geometry/BoundingBox.h \
    Geometry/Camera.h \
    Geometry/Cube.h \
    Geometry/Cylinder.h \
    Geometry/Geometry.h \
    Geometry/ImplicitSurface.h \
    Geometry/Intersection.h \
    Geometry/MarchingTetrahedra.h \
    Geometry/Ray.h \
    Geometry/Sphere.h \
    Scenes/Scene.h \
    Scenes/SceneCube.h \
    Scenes/SceneCylinder.h \
    Scenes/SceneSphere.h \
    Scenes/SceneSphereHighRes.h \
    SPH/Grid.h \
    SPH/Particle.h \
    SPH/Particles.h \
    SPH/SPH.h \
    CubeMap.h \
    GLShader.h \
    GLWidget.h \
    MainWindow.h \
    Material.h \
    TimeState.h

FORMS    += \
    MainWindow.ui

RESOURCES += \
    Ressources.qrc

OTHER_FILES += \
    Images/Checker/ZP.png \
    Images/Checker/ZN.png \
    Images/Checker/YP.png \
    Images/Checker/YN.png \
    Images/Checker/XP.png \
    Images/Checker/XN.png \
    Shaders/Refraction.vs \
    Shaders/Refraction.fs

DEPENDPATH += \
    Images/Checker \
    Shaders


























