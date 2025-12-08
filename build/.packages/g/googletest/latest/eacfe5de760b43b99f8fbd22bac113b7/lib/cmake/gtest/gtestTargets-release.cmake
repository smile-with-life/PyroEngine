#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gtest::gtest" for configuration "Release"
set_property(TARGET gtest::gtest APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gtest::gtest PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_NASM;C"
  # IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gtest.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/gtest.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gtest::gtest )
list(APPEND _IMPORT_CHECK_FILES_FOR_gtest::gtest "${_IMPORT_PREFIX}/lib/gtest.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
