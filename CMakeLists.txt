cmake_minimum_required(VERSION 3.10)

# Controlla che VITASDK sia definito
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path (e.g. export VITASDK=/path/to/vitasdk)")
  endif()
endif()

# Progetto
project(VitaVolume)

# Include VITASDK helpers
include("${VITASDK}/share/vita.cmake" REQUIRED)

# Informazioni app
set(VITA_APP_NAME "VitaVolume")
set(VITA_TITLEID  "VITAVOLUM")
set(VITA_VERSION  "01.00")

# Opzioni di compilazione
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# File sorgenti
add_executable(${PROJECT_NAME}
  src/main.c
  src/ctrl.c
)

# Librerie da linkare
target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub
  SceIofilemgr_stub
  SceAppMgr_stub
  SceCtrl_stub
  SceDisplay_stub
  ScePower_stub
  SceRegistryMgr_stub
  SceLibc_stub
)

# Crea eboot.bin (applicazione vera)
vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)

# Crea VPK con icone e livearea
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
