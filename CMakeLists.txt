cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(VitaVolume)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaVolume")
set(VITA_TITLEID  "VITAVOLUM")

set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -s CATEGORY=gdb")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS}")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
  src/main.c
  src/ctrl.c
)

target_link_libraries(${PROJECT_NAME}
	SceLibKernel_stub
	SceIofilemgr_stub
	SceAppMgr_stub
	SceCtrl_stub
	SceDisplay_stub
	SceVshBridge_stub
	ScePower_stub
	SceRegistryMgr_stub
	SceLibc_stub
)

vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  #FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
