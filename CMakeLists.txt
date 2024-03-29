cmake_minimum_required(VERSION 3.10)

project(AOC21 VERSION 1.0)

# compiler arguments
if (MSVC)
   add_compile_options()
else()
   add_compile_options(-Wall -Werror -Wextra -pedantic)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# enable interprocedural optimization if availible
if(ENABLE_IPO)
   include(CheckIPOSupported)
   check_ipo_supported( RESULT result OUTPUT output)
   if(result)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
   else()
      message(SEND_ERROR "IPO is not supported: ${output}")
   endif()
endif()

# enable color in diagnortics
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
   add_compile_options(-fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   # require at least gcc 11.1
   if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.1)
      message(FATAL_ERROR "GCC version must be at least 11.1!  " ${CMAKE_CXX_COMPILER_VERSION})
   endif()
   add_compile_options(-fdiagnostics-color=always)
else()
   message(STATUS "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
endif()

# Test Results for each day
list(APPEND part_1 500 5928 116920 74743 10584 4475 JMQZELVYXTIGPHFNSOADKWBRUC 46829 404502 "" "21,72" 3051 "74,87" 6985103122 224370 640 31038 675100 2016 3415 11050031 10395 652 10890 314)  
list(APPEND part_2 709 bqlporuexkwzyabnmgjqctvfs 382 132484 6968 35237 1133 37450 3243916887 10905 "242,13,9" 1300000000669 "29,74" 20182290 45539 472 25250 191820 22674960 8583 11341721 1010 164960498 7730 "") 

enable_testing()

# add utils subdirectory
add_subdirectory(Utils)

# add executable and test each day
foreach(day RANGE 1 25)
   
   # leading zero for single digit days
   if (${day} LESS 10)
      set(day "0${day}")
   endif()

   # set week
   if(day LESS 8)
      set(week 1)
   elseif(day LESS 15)
      set(week 2)
   elseif(day LESS 22)
      set(week 3)
   else()
      set(week 4)
   endif()

   # add each executable
   add_executable(day_${day} ${PROJECT_SOURCE_DIR}/week_${week}/day_${day}/day_${day}.cpp)
   set_target_properties(day_${day} PROPERTIES OUTPUT_NAME "day_${day}")
   target_link_libraries(day_${day} PUBLIC utils)

   # copy input over
   configure_file(${PROJECT_SOURCE_DIR}/week_${week}/day_${day}/input_${day} ${PROJECT_BINARY_DIR}/input_${day} COPYONLY)

   # set output for testing
   math(EXPR index ${day}-1)
   list(GET part_1 ${index} output_1)
   list(GET part_2 ${index} output_2)

   # test results
   add_test(NAME day_${day}_test COMMAND day_${day})
   set_tests_properties(day_${day}_test PROPERTIES PASS_REGULAR_EXPRESSION "${output_1}\n.*${output_2}" TIMEOUT 15)
   
endforeach()
