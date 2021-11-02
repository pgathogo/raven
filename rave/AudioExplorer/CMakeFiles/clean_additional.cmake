# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "AudioExplorer_autogen"
  "CMakeFiles\\AudioExplorer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AudioExplorer_autogen.dir\\ParseCache.txt"
  )
endif()
