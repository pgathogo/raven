# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\OATS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OATS_autogen.dir\\ParseCache.txt"
  "OATS_autogen"
  )
endif()
