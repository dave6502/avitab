target_sources(avitab_xplane PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/XPlaneEnvironment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/XPlaneGUIDriver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataRefImport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataRefExport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataCache.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MonitorBoundsDecider.cpp
)
