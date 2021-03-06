# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

 cmake_minimum_required(VERSION 2.8.0)
 project(SCHEMATIC)
 
 set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
 set (CMAKE_INSTALL_PREFIX       "../install" )

 find_package(LibXml2 REQUIRED)

 find_package(Qt5 COMPONENTS Widgets REQUIRED)
 find_package(Qt5 COMPONENTS Core    REQUIRED)
 find_package(Qt5 COMPONENTS Gui     REQUIRED)
# find_package(Qt5Core     REQUIRED)
# find_package(Qt5Gui     REQUIRED)
# find_package(Qt5Widgets     REQUIRED)

 set( mocIncludes SaveCellDialog.h
                  CellWidget.h
                  CellViewer.h
    )


 set(CMAKE_AUTOMOC ON)

 #include(${QT_USE_FILE})

    include_directories( ${SCHEMATIC_SOURCE_DIR}
                         ${LIBXML2_INCLUDE_DIR}
                       )

                     set( includes    Indentation.h
                                      XmlUtil.h 
                                      Point.h
                                      Box.h
                                      Term.h
                                      Net.h
                                      Instance.h
                                      Node.h
                                      Line.h
                                      Shape.h
                                      ArcShape.h
                                      BoxShape.h
                                      EllipseShape.h
                                      LineShape.h
                                      TermShape.h
                                      Symbol.h
                                      Cell.h
                                      CellsLib.h
                       )

                    set( mocIncludes  SaveCellDialog.h
                                      CellWidget.h
                                      CellViewer.h
                       )

                    set( cpps         Indentation.cpp
                                      XmlUtil.cpp 
                                      Point.cpp
                                      Box.cpp
                                      Term.cpp
                                      Net.cpp
                                      Instance.cpp
                                      Node.cpp
                                      Line.cpp
                                      Shape.cpp
                                      ArcShape.cpp
                                      BoxShape.cpp
                                      EllipseShape.cpp
                                      LineShape.cpp
                                      TermShape.cpp
                                      Symbol.cpp
                                      Cell.cpp
                                      CellsLib.cpp
                                      SaveCellDialog.cpp
                                      OpenCellDialog.cpp
                                      CellWidget.cpp
                                      CellViewer.cpp
                                      CellsModel.cpp
                                      InstancesWidget.cpp
                                      InstancesModel.cpp
                                      Main.cpp
                       )

           qt5_wrap_cpp( mocCpps      ${mocIncludes} )

         add_executable( tme810       ${cpps} ${mocCpps} )
  #target_link_libraries( tme810       ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} )
   target_link_libraries( tme810       ${LIBXML2_LIBRARIES} Qt5::Widgets )
                install( TARGETS      tme810                      DESTINATION bin )
                install( FILES        ${includes} ${mocIncludes}  DESTINATION include )
