# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\clusterman_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\clusterman_autogen.dir\\ParseCache.txt"
  "clusterman_autogen"
  )
endif()
