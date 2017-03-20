include(base.pri)

QT += core widgets

INCLUDEPATH += $$PWD/interface
INCLUDEPATH += $$PWD/implib

HEADERS += \
	$$PWD/interface/IPlugin.h \
	$$PWD/interface/IInit.h \
	$$PWD/interface/IMainWnd.h \
	$$PWD/interface/INode.h \
	$$PWD/interface/ISetting.h \
	$$PWD/interface/ICommand.h \
	$$PWD/interface/INodeBox.h \
	$$PWD/interface/INodeFactory.h \
	$$PWD/interface/ICarrier.h \
	$$PWD/interface/ICarrierFactory.h \
	$$PWD/interface/IRegion.h \
	$$PWD/interface/IProgram.h \
	$$PWD/interface/IScreen.h \
	$$PWD/interface/IAnimate.h \
	$$PWD/interface/INodePart.h \
	$$PWD/interface/IMsg.h \
	$$PWD/interface/IMsgFactory.h \
	$$PWD/interface/IProjJson.h \
	$$PWD/interface/IProtocol.h \
	$$PWD/interface/IProtocolFactory.h \
	$$PWD/interface/IBorder.h \
	$$PWD/interface/IBorderFactory.h \
	$$PWD/interface/IBackground.h \
	$$PWD/interface/IBackgroundFactory.h \
	$$PWD/interface/IFade.h \
	$$PWD/interface/IFadeFactory.h \
	$$PWD/interface/ICore.h \
	$$PWD/interface/ICoreJson.h \
	$$PWD/interface/IProject.h \
	$$PWD/interface/IGlowFactory.h \
	$$PWD/interface/IGlow.h \
	$$PWD/implib/NodeAttr.h \
	$$PWD/implib/RegionAttr.h \
	$$PWD/implib/RegionView.h \
	$$PWD/implib/Region.h \
	$$PWD/implib/NodeBox.h \
	$$PWD/implib/Node.h \
	$$PWD/implib/Message.h \
	$$PWD/implib/View.h

SOURCES +=

