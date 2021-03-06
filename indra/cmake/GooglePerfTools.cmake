# -*- cmake -*-
include(Prebuilt)

if (STANDALONE)
  include(FindGooglePerfTools)
else (STANDALONE)
  if (LINUX OR WINDOWS)
    use_prebuilt_binary(google)
  endif (LINUX OR WINDOWS)
  if (WINDOWS)
    set(TCMALLOC_LIBRARIES libtcmalloc_minimal.lib)
    set(TCMALLOC_LINKER_FLAGS "/INCLUDE:\"__tcmalloc\"")
  endif (WINDOWS)
  if (LINUX)
    if(USE_GOOGLE_PERFTOOLS)
      set(TCMALLOC_LIBRARIES tcmalloc)
      set(STACKTRACE_LIBRARIES stacktrace)
      set(PROFILER_LIBRARIES profiler)
    else()
      set(TCMALLOC_LIBRARIES tcmalloc_minimal)
    endif()
    set(GOOGLE_PERFTOOLS_INCLUDE_DIR
        ${LIBS_PREBUILT_DIR}/${LL_ARCH_DIR}/include)
    set(GOOGLE_PERFTOOLS_FOUND "YES")
  endif (LINUX)
endif (STANDALONE)

#if (GOOGLE_PERFTOOLS_FOUND)
#  set(USE_GOOGLE_PERFTOOLS ON CACHE BOOL "Build with Google PerfTools support.")
#endif (GOOGLE_PERFTOOLS_FOUND)

# XXX Disable temporarily, until we have compilation issues on 64-bit
# Etch sorted.
#set(USE_GOOGLE_PERFTOOLS OFF)

if (USE_GOOGLE_PERFTOOLS)
  set(TCMALLOC_FLAG -DLL_USE_TCMALLOC=1)
  include_directories(${GOOGLE_PERFTOOLS_INCLUDE_DIR})
  set(GOOGLE_PERFTOOLS_LIBRARIES ${TCMALLOC_LIBRARIES} ${STACKTRACE_LIBRARIES} ${PROFILER_LIBRARIES})
else (USE_GOOGLE_PERFTOOLS)
  set(TCMALLOC_FLAG -ULL_USE_TCMALLOC)
endif (USE_GOOGLE_PERFTOOLS)

if (NOT(DISABLE_TCMALLOC OR USE_GOOGLE_PERFTOOLS))
  message(STATUS "Building with Google TCMalloc")
	set(TCMALLOC_FLAG -DLL_USE_TCMALLOC=1_)
	include_directories(${GOOGLE_PERFTOOLS_INCLUDE_DIR})
	set(GOOGLE_PERFTOOLS_LIBRARIES ${TCMALLOC_LIBRARIES})
  set(GOOGLE_PERFTOOLS_LINKER_FLAGS ${TCMALLOC_LINKER_FLAGS})
endif()
