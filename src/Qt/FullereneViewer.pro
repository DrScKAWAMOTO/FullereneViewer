#
# Project: FullereneViewer
# Version: 1.0
# Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
# Create: 2012/03/14 15:36:05 JST
#

QT       += core gui opengl
contains(QT_VERSION, ^4.*) {
    QT       += webkit
}
contains(QT_VERSION, ^5.*) {
    QT       += webkitwidgets
}
TEMPLATE = app
TARGET = FullereneViewer
DEPENDPATH += . ..
INCLUDEPATH += . ..
macx {
    DEPENDPATH += /usr/X11/include
    INCLUDEPATH += /usr/X11/include
    ICON = icons/FullereneViewer.icns
}
!macx:unix {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lGLU
}
win32 {
    RC_FILE = FullereneViewer.rc
}

# Input
HEADERS += ../Automorphism.h ../Automorphisms.h ../AvoidBugs.h \
           ../Bond.h ../BoundaryCarbons.h \
           ../Carbon.h ../CarbonAllotrope.h ../Center.h ../Characteristic.h \
           ../Clustering.h ../Configuration.h ../Config.h ../ConnectedBoundary.h \
           ../Debug.h ../DebugMemory.h ../DistanceVector.h \
           ../ErrorCode.h \
           ../Fullerene.h \
           ../Generator.h \
           ../Interactive.h ../InteractiveLine.h ../InteractiveOperation.h \
           ../InteractivePlane.h ../InteractivePoint.h ../InteractiveRegularPolygon.h \
           ../Interactives.h \
           ../List.h \
           ../Matrix3.h ../MyString.h \
           ../Normal.h \
           ../Object.h ../OpenGLUtil.h \
           ../Pattern.h ../Posture.h \
           ../Quaternion.h \
           ../Radius.h ../Random.h ../Representation.h ../RepresentationInfo.h \
           ../Representations.h ../Ring.h \
           ../Set.h ../Statistics.h ../SymmetryAxis.h ../SymmetryAxisNormal.h \
           ../ThreeViewNormal.h \
           ../Utils.h \
           ../Vector3.h ../Version.h \
           AboutWindow.h ConfigurationDialog.h \
           GeneratorFormulaDialog.h Guruguru.h \
           HelpBrowser.h \
           MainWindow.h \
           SelectFullereneDialog.h \
           TreeItem.h TreeModel.h
FORMS += MainWindow.ui ConfigurationDialog.ui AboutWindow.ui GeneratorFormulaDialog.ui \
    SelectFullereneDialog.ui
SOURCES += ../Automorphism.cc ../Automorphisms.cc \
           ../Bond.cc ../BoundaryCarbons.cc \
           ../Carbon.cc ../CarbonAllotrope.cc ../Characteristic.cc ../Clustering.cc \
           ../Configuration.cc ../ConnectedBoundary.cc \
           ../DebugMemory.cc ../DistanceVector.cc \
           ../ErrorCode.cc \
           ../Fullerene.cc \
           ../Generator.cc \
           ../Interactive.cc ../InteractiveLine.cc ../InteractiveOperation.cc \
           ../InteractivePlane.cc ../InteractivePoint.cc \
           ../InteractiveRegularPolygon.cc ../Interactives.cc \
           ../Matrix3.cc ../MyString.cc \
           ../Object.cc ../OpenGLUtil.cc \
           ../Pattern.cc \
           ../Quaternion.cc \
           ../Random.cc ../Representation.cc ../RepresentationInfo.cc \
           ../Representations.cc ../Ring.cc \
           ../Statistics.cc ../SymmetryAxis.cc ../SymmetryAxisNormal.cc \
           ../ThreeViewNormal.cc \
           ../Utils.cc \
           ../Vector3.cc ../Version.cc \
           AboutWindow.cc \
           BondItem.cc \
           CarbonItem.cc ConfigurationDialog.cc \
           GeneratorFormulaDialog.cc Guruguru.cc \
           HelpBrowser.cc \
           main.cc MainWindow.cc MergedItem.cc \
           RingItem.cc \
           SelectFullereneDialog.cc \
           TreeItem.cc TreeModel.cc TubeItem.cc

OTHER_FILES +=

RESOURCES += doc.qrc media.qrc
