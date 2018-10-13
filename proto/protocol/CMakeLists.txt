################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

SET(CMAKE_USE_RELATIVE_PATHS on)

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET( RELEASE  $ENV{RELEASE})
SET( UNICODE  $ENV{UNICODE})

PROJECT ( stormcommon )
SET(PRONAME "stormproto")


################ OSBIT CHECK #####################
IF(CMAKE_SIZEOF_VOID_P MATCHES 4)
    SET ( OSBIT    32)
    MESSAGE(STATUS "OSBIT 32")
ELSE()
    SET ( OSBIT 64)
    MESSAGE(STATUS "OSBIT 64")
ENDIF(CMAKE_SIZEOF_VOID_P MATCHES 4)


################ DEBUG/RELEASE #####################
IF(RELEASE)
    MESSAGE(STATUS "RELEASE ON")
    MESSAGE(STATUS "PROJECT DIR:" ${CMAKE_CURRENT_SOURCE_DIR})

    IF(UNICODE)
        MESSAGE(STATUS "UNICODE ON")
        SET( CMAKE_BUILD_TYPE Release_Unicode )
    ELSE()
        SET( CMAKE_BUILD_TYPE Release )
    ENDIF(UNICODE)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -g -std=c++11 -ltcmalloc -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")

    ################ MACRO #####################
    ADD_DEFINITIONS(-fPIC -DNDEBUG)

    ################ OUT PATH #####################
    SET (RUNPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lib3rd/storm/bin/linux64/release")

ELSE()
    MESSAGE(STATUS "DEBUG ON")
    MESSAGE(STATUS "PROJECT DIR:" ${CMAKE_CURRENT_SOURCE_DIR})

    IF(UNICODE)
        MESSAGE(STATUS "UNICODE ON")
        set( CMAKE_BUILD_TYPE Debug_Unicode )
    ELSE()
        set( CMAKE_BUILD_TYPE Debug )
    ENDIF(UNICODE)

    ################ MACRO #####################
    ADD_DEFINITIONS(-fPIC -DDEBUG -D_DEBUG -Wno-unused-variable -D_REENTRANT -D_LINUX -Wno-deprecated)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11 -ltcmalloc -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")

    ################ OUT PATH #####################
    SET (RUNPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lib3rd/storm/bin/linux64/debug")

ENDIF(RELEASE)


################ INCLUDE #####################
INCLUDE_DIRECTORIES( 
        #
        ./
        ../common
	../common/async
	../common/db
	../common/mytimer
	../common/timer
	../common/redis
	../common/exception
        ../protocol
        ../../proto/include
        ../../lib3rd/rapidjson
        ../../lib3rd/curl-7.50.3/include
        ../../lib3rd/protobuff/include
        ../../lib3rd/storm/include
        ../../lib3rd/storm/include/base
        ../../lib3rd/storm/thirdparty/openssl/linux/include
        ../../lib3rd/storm/include/components/server
        ../../lib3rd/storm/thirdparty/tinyxml/include
        #
        ../../lib3rd/hiredis/linux
        ../../lib3rd/boost_1_64_0
        ../../lib3rd/cryptopp6

        )

################ LIBRARIES #####################
SET(LIBRARIES
        #
        -lstormpipe
        -lprotobuf
        -lstormbase
        -lstormdb
        -lstormnet
        -lrt
        -lcrypto
        -ltinyxml
        -lstdc++
        -lmysqlclient
        -lhiredis
        -lcryptopp
        -ltcmalloc
    )



################ SOURCE_FILES #####################
AUX_SOURCE_DIRECTORY("." SRC_FILES)     
AUX_SOURCE_DIRECTORY("async" SRC_FILES)
AUX_SOURCE_DIRECTORY("db" SRC_FILES)
AUX_SOURCE_DIRECTORY("mytimer" SRC_FILES)
AUX_SOURCE_DIRECTORY("redis" SRC_FILES)
AUX_SOURCE_DIRECTORY("timer" SRC_FILES)



################ BUILD #####################
set(OutDir "")

ADD_LIBRARY( ${PRONAME} STATIC ${HEAD_FILES} ${SRC_FILES} )
SET(LIBRARY_OUTPUT_PATH ${RUNPATH}) 

#SET(LIBRARY_OUTPUT_PATH ${OUTPUT}) 


################ INSTALL #####################
#INSTALL( TARGETS ${PRONAME} RUNTIME DESTINATION ${RUNPATH} )


