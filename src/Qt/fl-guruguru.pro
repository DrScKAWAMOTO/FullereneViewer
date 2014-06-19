#
# Project: FullereneViewer
# Version: 1.0
# Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
# Create: 2012/03/14 15:36:05 JST
#

QT       += core gui opengl
TEMPLATE = app
TARGET = fl-guruguru
DEPENDPATH += . ..
INCLUDEPATH += . ..
macx {
    DEPENDPATH += /usr/X11/include
    INCLUDEPATH += /usr/X11/include
    LIBS += -framework glut
    ICON = icons/fl-guruguru.icns
}
!macx:unix {
LIBS += -lglut -lGLU
}
win32 {
    DEPENDPATH += C:\Users\kawamoto\Documents\_projects\Qt5\freeglut-2.8.1\include
    INCLUDEPATH += C:\Users\kawamoto\Documents\_projects\Qt5\freeglut-2.8.1\include
    LIBS += -LC:\Users\kawamoto\Documents\_projects\Qt5\freeglut-2.8.1\lib\x86\Debug -lfreeglut
    RC_FILE = fl-guruguru.rc
}

# Input
HEADERS += ../Automorphism.h \
           ../Automorphisms.h \
           ../AvoidBugs.h \
           ../Bond.h \
           ../Carbon.h \
           ../CarbonAllotrope.h \
           ../Center.h \
           ../Config.h \
           ../Debug.h \
           ../DebugMemory.h \
           ../DistanceMatrix.h \
           ../DistanceSummaryLine.h \
           ../DistanceVector.h \
           ../ErrorCode.h \
           ../Fullerene.h \
           ../FullereneCharacteristic.h \
           ../FullereneMenu.h \
           ../Fullerenes.h \
           ../Generator.h \
           ../GeneratorLine.h \
           ../Interactive.h \
           ../InteractiveLine.h \
           ../InteractiveOperation.h \
           ../InteractivePlane.h \
           ../InteractivePoint.h \
           ../InteractiveRegularPolygon.h \
           ../Interactives.h \
           ../List.h \
           ../Matrix3.h \
           ../MenuEntry.h \
           ../Normal.h \
           ../Object.h \
           ../OpenGLUtil.h \
           ../Pattern.h \
           ../Posture.h \
           ../Quaternion.h \
           ../Radius.h \
           ../Random.h \
           ../Representation.h \
           ../RepresentationInfo.h \
           ../Representations.h \
           ../Ring.h \
           ../SortedList.h \
           ../Statistics.h \
           ../SymmetryAxis.h \
           ../ThreeViewNormal.h \
           ../Utils.h \
           ../Vector3.h \
           QtFullereneMenu.h \
           mainwindow.h \
           fl-guruguru.h
FORMS += mainwindow.ui
SOURCES += ../Automorphism.cc \
           ../Automorphisms.cc \
           ../Bond.cc \
           ../Carbon.cc \
           ../CarbonAllotrope.cc \
           ../DebugMemory.cc \
           ../DistanceMatrix.cc \
           ../DistanceSummaryLine.cc \
           ../DistanceVector.cc \
           ../ErrorCode.cc \
           ../Fullerene.cc \
           ../FullereneCharacteristic.cc \
           ../Fullerenes.cc \
           ../Generator.cc \
           ../GeneratorLine.cc \
           ../Interactive.cc \
           ../InteractiveLine.cc \
           ../InteractiveOperation.cc \
           ../InteractivePlane.cc \
           ../InteractivePoint.cc \
           ../InteractiveRegularPolygon.cc \
           ../Interactives.cc \
           ../Matrix3.cc \
           ../Object.cc \
           ../OpenGLUtil.cc \
           ../Pattern.cc \
           ../Quaternion.cc \
           ../Random.cc \
           ../Representation.cc \
           ../RepresentationInfo.cc \
           ../Representations.cc \
           ../Ring.cc \
           ../Statistics.cc \
           ../SymmetryAxis.cc \
           ../ThreeViewNormal.cc \
           ../Utils.cc \
           ../Vector3.cc \
           QtFullereneMenu.cc \
           main.cc \
           mainwindow.cc \
           fl-guruguru.cc
