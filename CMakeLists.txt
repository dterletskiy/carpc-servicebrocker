cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

fenix_project( TARGET servicebrocker PROJECT SERVICEBROCKER )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_GEN_DIR} )
include_directories( ${TRACING_SOURCE_DIR}/api )
include_directories( ${BASE_SOURCE_DIR}/api )
include_directories( ${OSW_SOURCE_DIR}/api )
include_directories( ${TOOLS_SOURCE_DIR}/api )
include_directories( ${RUNTIME_SOURCE_DIR}/api )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( ${TRACING_BINARY_DIR} )
link_directories( ${BASE_BINARY_DIR} )
link_directories( ${OSW_BINARY_DIR} )
link_directories( ${TOOLS_BINARY_DIR} )
link_directories( ${RUNTIME_BINARY_DIR} )



###########################################################################################
#                                                                                         #
#                                   Compile definitions                                   #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
find_files_by_ext( RECURSE FILES PROJECT_SOURCE_FILES
      LOCATION ${PROJECT_SOURCE_DIR}
      EXTENTIONS ${EXTENTIONS_CPP_SRC}
   )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_variables( )



add_executable(
      ${PROJECT_TARGET_NAME}
      ${PROJECT_SOURCE_FILES}
   )

target_link_libraries( ${PROJECT_TARGET_NAME} ${RUNTIME_TARGET_NAME}-shared )
if( MEMORY_HOOK )
   target_link_libraries( ${PROJECT_TARGET_NAME} ${HOOKS_TARGET_NAME} )
endif( )
if( INSTRUMENTAL )
   target_link_libraries( ${PROJECT_TARGET_NAME} ${INSTRUMENTAL_TARGET_NAME} )
endif( )

add_custom_target( "${PROJECT_TARGET_NAME}-documentation" ALL
      COMMENT "cmake ${PROJECT_TARGET_NAME}-documentation"
      DEPENDS ${PROJECT_GEN_PLANTUML_FILES}
   )

add_dependencies( ${PROJECT_TARGET_NAME} "${PROJECT_TARGET_NAME}-documentation" )

install( TARGETS ${PROJECT_TARGET_NAME} DESTINATION bin )
install(
      FILES ${PROJECT_SOURCE_DIR}/configuration.cfg
      DESTINATION etc
      RENAME ${PROJECT_TARGET_NAME}.cfg
   )
