# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.12)

# Set any variables required for importing libraries
if (DEFINED ENV{FREERTOS_PATH})
  SET(FREERTOS_PATH $ENV{FREERTOS_PATH})
else()
  SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)
endif()

message("FreeRTOS Kernel located in ${FREERTOS_PATH}")

# Import those libraries
include(pico_sdk_import.cmake)
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(embarcatech-freertos-tarefa-1 C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(embarcatech-freertos-tarefa-1
    main.c
)

# Habilita a saída de terminal (stdio) via USB e desabilita via UART
pico_enable_stdio_usb(embarcatech-freertos-tarefa-1 1)
pico_enable_stdio_uart(embarcatech-freertos-tarefa-1 0)

target_include_directories(embarcatech-freertos-tarefa-1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# ########## SEÇÃO ALTERADA ##########
# Adicionamos a biblioteca 'hardware_pwm' aqui
target_link_libraries(embarcatech-freertos-tarefa-1 
    pico_stdlib
    hardware_pwm 
    FreeRTOS-Kernel-Heap4
    # hardware_i2c foi removido pois não é usado
    )
# ####################################

pico_add_extra_outputs(embarcatech-freertos-tarefa-1)