# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\sedric_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\sedric_autogen.dir\\ParseCache.txt"
  "sedric_autogen"
  )
endif()
