CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (kdots)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

FIND_PACKAGE (Qt4 REQUIRED)

SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTCORE TRUE)

INCLUDE (${QT_USE_FILE})


INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET (PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)


SET (KDOTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET (SRCS
  graphpoint.cpp
  graph.cpp
  dottable.cpp
  stepqueue.cpp
  tablewidget.cpp
  main.cpp
  newgamewidget.cpp
  newgamedialog.cpp
  mainwindow.cpp
  polygonfinder.cpp
  polygon.cpp
  plugincontainer.cpp
  pluginmanagerwidget.cpp
  gameconfig.cpp
)

SET (HEADERS
  include/graphpoint.hpp
  include/graph.hpp
  include/dottable.hpp
  include/stepqueue.hpp
  include/tablewidget.hpp
  include/edgelist.hpp
  include/point.hpp
  include/newgamedialog.hpp
  include/newgamewidget.hpp
  include/mainwindow.hpp
  include/constants.hpp
  include/polygonfinder.hpp
  include/polygon.hpp
  include/plugincontainer.hpp
  include/pluginmanagerwidget.hpp
  include/gameconfig.hpp
)

SET (INTERFACE_HEAD
  interface/iplugin.hpp
  interface/irival.hpp
  interface/iconfigurationwidget.hpp
)

SET (FORMS
  ui/mainwindow.ui
  ui/newgamewidget.ui
  ui/newgamedialog.ui
)

QT4_WRAP_CPP (HEADERS_MOC ${HEADERS})
QT4_WRAP_CPP (INTERFACE_MOC ${INTERFACE_HEAD})
QT4_WRAP_UI (FORMS_HEADERS ${FORMS})

ADD_EXECUTABLE (kdots
  ${SRCS}
  ${HEADERS_MOC}
  ${INTERFACE_MOC}
  ${FORMS_HEADERS}
)

TARGET_LINK_LIBRARIES (kdots
  ${QT_LIBRARIES}
)

ADD_SUBDIRECTORY (plugins)