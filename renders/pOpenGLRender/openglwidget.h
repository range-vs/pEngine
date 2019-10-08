#pragma once

#include "../../pEntryPoint/pch.h"

// qt
#include <QGLWidget>
#include <QtOpenGL>

// pEngine
#include "lib_define.h"
#include "../pgeneralrender.h"
#include "../../pEngine/pengine.h"
#include "../../pMath/mathematics.h"

using namespace gmath;

class DYNLIB_PRENDER_OPENGL OpenGLRender: public QOpenGLWidget, public IRender
{
    Q_OBJECT

    QOpenGLFunctions* func;
    QOpenGLVertexArrayObject* vao1;
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint programID;
//    QMatrix4x4 MVP;
//    QMatrix4x4 Model;
//    QMatrix4x4 Projection;
//    QMatrix4x4 View;
    Matrix4 MVP;
    Matrix4 Model;
    Matrix4 Projection;
    Matrix4 View;
    GLuint MatrixID;
    float angle;
    int minorVersion;
    int majorVersion;
    int gl_Width;
    int gl_Height;

    GLuint vertexPosition_modelspaceID;
    GLuint vertexColorID;

    std::shared_ptr<QTimer> updater;
    std::shared_ptr<FPSCounter> fps;
    std::shared_ptr<QTimer> rotateCube;

protected:
    void initializeGL() override;
    void resizeGL(int vw, int h) override;
    void paintGL() override;


    void getVersion();
    void getContext();
    void createBuffers();
    void initScene();
    void initCamera(int vw, int h);
    void initRenderLoop();
    GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

private:
    void closeEvent(QCloseEvent *event) override;

public:
    OpenGLRender(QWidget* parent = nullptr);

public slots:
    void updaterRender();
    void rotate();

signals:
    void closedChildrenWindows();

    // IRender interface
public:
    void initializeRender(int vw, int h) override;
    void resizeRender(int vw, int h) override;
    void updateRender() override;
    void drawRender() override;
    void showRender() override;
    void updateFps() override;
    ~OpenGLRender() override;

};
