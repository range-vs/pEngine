TEMPLATE = subdirs

CONFIG += ordered

win32: SUBDIRS += pDirect3DRender pOpenGLRender
unix: SUBDIRS += pOpenGLRender
