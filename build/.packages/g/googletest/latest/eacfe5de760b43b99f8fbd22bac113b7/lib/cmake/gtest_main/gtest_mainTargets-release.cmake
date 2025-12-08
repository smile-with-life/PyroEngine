#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gtest_main::gtest_main" for configuration "Release"
set_property(TARGET gtest_main::gtest_main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gtest_main::gtest_main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_NASM;C"
  # IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gtest_main.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/gtest_main.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gtest_main::gtest_main )
list(APPEND _IMPORT_CHECK_FILES_FOR_gtest_main::gtest_main "${_IMPORT_PREFIX}/lib/gtest_main.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
