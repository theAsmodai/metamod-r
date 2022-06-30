#----------------------------------------
# 1. Preparing build:
#    rm -rf build
#    mkdir build && cd build
#
# 2. Select compiler and build it
# - Compile with Clang:
#    CC="clang" CXX="clang++" cmake ..
#    make
#
# - Compile with Intel C++ Compiler:
#    CC="icc" CXX="icpc" cmake ..
#    make
#
# - Compile with GCC Compiler:
#    cmake ..
#    make
#
# Use -DCMAKE_BUILD_TYPE=COMPAT_GLIBC
# to backward compatibility with oldest version of glibc 2.11
#
#----------------------------------------

cmake_minimum_required(VERSION 3.1)
project(metamod CXX)

option(DEBUG "Build with debug information." OFF)
option(USE_STATIC_LIBSTDC "Enables static linking libstdc++." OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Avoid -rdynamic -fPIC options
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(COMPILE_FLAGS "-m32 -U_FORTIFY_SOURCE")
set(LINK_FLAGS "-m32 -s")

set(COMPILE_FLAGS "${COMPILE_FLAGS} -Wall -fno-exceptions -fno-builtin -Wno-unknown-pragmas")

# Remove noxref code and data
set(COMPILE_FLAGS "${COMPILE_FLAGS} -ffunction-sections -fdata-sections")

if (DEBUG)
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -g3 -O3 -ggdb")
else()
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -g0 -O3 -fno-stack-protector")
endif()

# Check Intel C++ compiler
if ("$ENV{CXX}" MATCHES "icpc")
	set(COMPILE_FLAGS "${COMPILE_FLAGS} \
		-fasm-blocks -Qoption,cpp,--treat_func_as_string_literal_cpp\
		-inline-forceinline -no-ansi-alias")

	set(LINK_FLAGS "${LINK_FLAGS} -static-intel -no-intel-extensions")

	if (NOT DEBUG)
		set(COMPILE_FLAGS "${COMPILE_FLAGS} -ipo")
		set(LINK_FLAGS "${LINK_FLAGS} -ipo -diag-disable=11003")
	endif()
else()
	# Produce code optimized for the most common IA32/AMD64/EM64T processors.
	# As new processors are deployed in the marketplace, the behavior of this option will change.
	set(COMPILE_FLAGS "${COMPILE_FLAGS} \
		-mtune=generic -msse3\
		-fno-sized-deallocation -Wno-invalid-offsetof -Wno-ignored-attributes\
		-Wno-write-strings -Wno-strict-aliasing -fno-plt")

	# Check Clang compiler
	if (NOT "$ENV{CXX}" MATCHES "clang")
		set(COMPILE_FLAGS "${COMPILE_FLAGS} -Wno-maybe-uninitialized -Wno-unused-but-set-variable")

		# GCC >= 8.3
		if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0)
			set(COMPILE_FLAGS "${COMPILE_FLAGS} -Wno-stringop-truncation -Wno-format-truncation -Wno-class-memaccess")
		endif()
	endif()
endif()

# GCC >= 8.3
if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0)
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -fcf-protection=none")
endif()

if (NOT DEBUG)
	set(LINK_FLAGS "${LINK_FLAGS} \
		-Wl,-gc-sections -Wl,--version-script=\"${PROJECT_SOURCE_DIR}/../version_script.lds\"")
endif()

if (CMAKE_BUILD_TYPE MATCHES COMPAT_GLIBC)
	set(LINK_FLAGS	"${LINK_FLAGS} -Wl,--wrap=stat64")
endif()

set(PROJECT_SRC_DIR
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/include/engine"
	"${PROJECT_SOURCE_DIR}/include/common"
	"${PROJECT_SOURCE_DIR}/include/dlls"
	"${PROJECT_SOURCE_DIR}/include/game_shared"
	"${PROJECT_SOURCE_DIR}/include/pm_shared"
	"${PROJECT_SOURCE_DIR}/include/public"
)

set(PROJECT_PUBLIC_DIR
	"${PROJECT_SOURCE_DIR}/include/public"
	"${PROJECT_SOURCE_DIR}/include/public/metamod"
)

add_library(metamod SHARED ${appversion.sh})

if (NOT TARGET appversion)
	add_custom_target(appversion DEPENDS COMMAND "${PROJECT_SOURCE_DIR}/version/appversion.sh" "${PROJECT_SOURCE_DIR}/..")
endif()

add_dependencies(metamod appversion)

target_include_directories(metamod PRIVATE
	${PROJECT_SRC_DIR}
	${PROJECT_PUBLIC_DIR}
)

target_compile_definitions(metamod PRIVATE
	_LINUX
	LINUX
	NDEBUG
	METAMOD_CORE
	_GLIBCXX_USE_CXX11_ABI=0
	_stricmp=strcasecmp
	_strnicmp=strncasecmp
	_strdup=strdup
	_unlink=unlink
	_write=write
	_close=close
	_getcwd=getcwd
	_vsnprintf=vsnprintf
	_vsnwprintf=vswprintf
	_snprintf=snprintf
)

target_sources(metamod PRIVATE
	"src/api_info.cpp"
	"src/callback_jit.cpp"
	"src/commands_meta.cpp"
	"src/conf_meta.cpp"
	"src/dllapi.cpp"
	"src/engine_api.cpp"
	"src/game_support.cpp"
	"src/h_export.cpp"
	"src/linkent.cpp"
	"src/linkgame.cpp"
	"src/log_meta.cpp"
	"src/mdebug.cpp"
	"src/mem_utils.cpp"
	"src/meta_rehlds_api.cpp"
	"src/metamod.cpp"
	"src/mextdll.cpp"
	"src/mlist.cpp"
	"src/mplayer.cpp"
	"src/mplugin.cpp"
	"src/mreg.cpp"
	"src/mutil.cpp"
	"src/precompiled.cpp"
	"src/public_amalgamation.cpp"
	"src/reg_support.cpp"
	"src/sdk_util.cpp"
	"src/studioapi.cpp"
	"src/sys_module.cpp"
	"src/utils.cpp"
	"src/glibc_compat.cpp"
)

target_link_libraries(metamod PRIVATE
	dl
	$<$<CONFIG:COMPAT_GLIBC>:libc-2.11.1.so>
)

if (USE_STATIC_LIBSTDC)
	target_compile_definitions(metamod PRIVATE BUILD_STATIC_LIBSTDC)
	set(LINK_FLAGS "${LINK_FLAGS} -static-libgcc -static-libstdc++")
endif()

if (CMAKE_BUILD_TYPE MATCHES COMPAT_GLIBC)
	set(LINK_FLAGS "${LINK_FLAGS} \
		-Wl,-rpath,'$ORIGIN/.' \
		-L${PROJECT_SOURCE_DIR}/lib/linux32")
endif()

set_target_properties(metamod PROPERTIES
	OUTPUT_NAME metamod_i386
	PREFIX ""
	COMPILE_FLAGS ${COMPILE_FLAGS}
	LINK_FLAGS ${LINK_FLAGS}
	POSITION_INDEPENDENT_CODE ON
)
