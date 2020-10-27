# HALCON/Qt pattern matching and measure example
#
# (c) 2004-2020 MVTec Software GmbH

# matching.pro
#

CONFIG		+= qt debug
QT              += core gui  widgets

# MacOS specific settings. Note that while dyld will search under
# /Library/Frameworks by default, the preprocessor/compiler/linker will not
# and need to be told explicitly.
macx {
  QMAKE_CXXFLAGS += -F/Library/Frameworks
  QMAKE_LFLAGS   += -F/Library/Frameworks
  LIBS           += -framework HALCONCpp
}
else {
  #defines
  win32:DEFINES += WIN32

  #includes
  INCLUDEPATH   += "$$(HALCONROOT)/include"
  INCLUDEPATH   += "$$(HALCONROOT)/include/halconcpp"

  #libs
  QMAKE_LIBDIR  += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
  unix:LIBS     += -lhalconcpp -lhalcon -lXext -lX11 -ldl -lpthread
  win32:LIBS    += "$$(HALCONROOT)/lib/$$(HALCONARCH)/halconcpp.lib" \
                   "$$(HALCONROOT)/lib/$$(HALCONARCH)/halcon.lib"
}

#sources
HEADERS	    += qhalconwindow.h
HEADERS	    += matching.h
SOURCES	    += qhalconwindow.cpp
SOURCES	    += matching.cpp
SOURCES	    += main.cpp
