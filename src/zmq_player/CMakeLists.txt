

#The minimum CMake version which is needed
cmake_minimum_required(VERSION 2.8)

#This includes the environment
set( SELFCONTAINED_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/)
set( SELFCONTAINED_THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_CMAKE} )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_THIRDPARTY} )
include( setEnv )

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../
                    ${Boost_INCLUDE_DIRS}
                    ${Zeromq_INCLUDE_DIRS}
                    ${Sqlite3_INCLUDE_DIRS}
                    ${Sqlite3cc_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME}   ${Boost_LIBRARIES}
                                        ${Zeromq_LIBRARIES}
                                        ${Sqlite3_LIBRARIES}
                                        ${Sqlite3cc_LIBRARIES})

#-- Set the Debug and Release names for the libraries
SET_TARGET_PROPERTIES( ${PROJECT_NAME}
    PROPERTIES
    DEBUG_OUTPUT_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}
    RELEASE_OUTPUT_NAME ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}/bin)

#this installs the start.bat
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/startPlayer.bat
        DESTINATION ${PROJECT_NAME}/bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/startPlayer.sh
        DESTINATION ${PROJECT_NAME}/bin)
        
