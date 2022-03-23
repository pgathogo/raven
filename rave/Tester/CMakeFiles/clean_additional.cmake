# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\Tester_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Tester_autogen.dir\\ParseCache.txt"
  "Tester_autogen"
  )
endif()
