PKG_CHECK_MODULES(KEY_MANAGER_DEP
    dlog
    openssl
    libsmack
    libsystemd-daemon
    REQUIRED
    )

SET(KEY_MANAGER_SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET(KEY_MANAGER_PATH ${PROJECT_SOURCE_DIR}/src/manager)

SET(KEY_MANAGER_SOURCES
    ${KEY_MANAGER_PATH}/main/key-manager-util.cpp
    ${KEY_MANAGER_PATH}/main/generic-socket-manager.cpp
    ${KEY_MANAGER_PATH}/main/socket-manager.cpp
    ${KEY_MANAGER_PATH}/main/key-manager-main.cpp
    ${KEY_MANAGER_PATH}/service/echo.cpp
    ${KEY_MANAGER_PATH}/service/ckm-service.cpp
    ${KEY_MANAGER_PATH}/service/ckm-logic.cpp
    ${KEY_MANAGER_PATH}/service/key-provider.cpp
    )

SET_SOURCE_FILES_PROPERTIES(
    ${KEY_MANAGER_SOURCES}
    PROPERTIES
        COMPILE_FLAGS "-D_GNU_SOURCE -fvisibility=hidden")

INCLUDE_DIRECTORIES(SYSTEM
    ${KEY_MANAGER_DEP_INCLUDE_DIRS}
    )

INCLUDE_DIRECTORIES(
    ${KEY_MANAGER_SRC_PATH}/include
    ${KEY_MANAGER_PATH}/main
    ${KEY_MANAGER_PATH}/common
    ${KEY_MANAGER_PATH}/service
    ${KEY_MANAGER_PATH}/dpl/core/include
    ${KEY_MANAGER_PATH}/dpl/log/include
    )

ADD_EXECUTABLE(${TARGET_KEY_MANAGER} ${KEY_MANAGER_SOURCES})

TARGET_LINK_LIBRARIES(${TARGET_KEY_MANAGER}
    ${KEY_MANAGER_DEP_LIBRARIES}
    ${TARGET_KEY_MANAGER_COMMON}
	${TARGET_KEY_MANAGER_KEY_PROVIDER}
    )



################################################################################

SET(KEY_MANAGER_CLIENT_VERSION_MAJOR 1)
SET(KEY_MANAGER_CLIENT_VERSION ${KEY_MANAGER_CLIENT_VERSION_MAJOR}.0.1)

SET(KEY_MANAGER_CLIENT_SRC_PATH ${KEY_MANAGER_PATH}/client)

INCLUDE_DIRECTORIES(
    ${KEY_MANAGER_PATH}/client
    ${KEY_MANAGER_PATH}/common
    ${KEY_MANAGER_PATH}/dpl/core/include
    ${KEY_MANAGER_PATH}/dpl/log/include
    )

SET(KEY_MANAGER_CLIENT_SOURCES
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-control.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-common.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-echo.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-key.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-key-impl.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-manager.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-manager-impl.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-certificate.cpp
    ${KEY_MANAGER_CLIENT_SRC_PATH}/client-certificate-impl.cpp
    )

ADD_LIBRARY(${TARGET_KEY_MANAGER_CLIENT} SHARED ${KEY_MANAGER_CLIENT_SOURCES})

SET_TARGET_PROPERTIES(
    ${TARGET_KEY_MANAGER_CLIENT}
    PROPERTIES
        COMPILE_FLAGS "-D_GNU_SOURCE -fPIC -fvisibility=hidden" 
        SOVERSION ${KEY_MANAGER_CLIENT_VERSION_MAJOR}
        VERSION ${KEY_MANAGER_CLIENT_VERSION}
    )

TARGET_LINK_LIBRARIES(${TARGET_KEY_MANAGER_CLIENT}
    ${KEY_MANAGER_DEP_LIBRARIES}
    ${TARGET_KEY_MANAGER_COMMON}
    )

################################################################################

INSTALL(TARGETS ${TARGET_KEY_MANAGER_CLIENT} DESTINATION ${LIB_INSTALL_DIR})

INSTALL(TARGETS ${TARGET_KEY_MANAGER} DESTINATION bin)

INSTALL(FILES
    ${KEY_MANAGER_SRC_PATH}/include/ckm/key-manager.h
    ${KEY_MANAGER_SRC_PATH}/include/ckm/ckm-error.h
    ${KEY_MANAGER_SRC_PATH}/include/ckm/ckm-type.h
    DESTINATION /usr/include/ckm/ckm
    )

################################################################################

ADD_SUBDIRECTORY(manager)