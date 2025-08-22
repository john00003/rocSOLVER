# ##########################################################################
# Copyright (C) 2016-2025 Advanced Micro Devices, Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# ##########################################################################

set(rocsolver_bench_common_source
  ${PROJECT_SOURCE_DIR}/common/src/common_host_helpers.cpp
)

set(rocsolver_auxiliary_source
  # vector & matrix manipulations
  ${PROJECT_SOURCE_DIR}/library/src/auxiliary/rocauxiliary_aliases.cpp
  ${PROJECT_SOURCE_DIR}/library/src/auxiliary/rocauxiliary_lacgv.cpp
  ${PROJECT_SOURCE_DIR}/library/src/auxiliary/rocauxiliary_laswp.cpp
  # householder reflections
  ${PROJECT_SOURCE_DIR}/library/src/auxiliary/rocauxiliary_larft.cpp
)

add_executable(larft-example
  larft_example.cpp
  ${rocsolver_auxiliary_source}
  ${common_source_paths}
  ${rocsolver_bench_common_source}
)
if(USE_HIPCXX)
  set_source_files_properties(
    larft-example.cpp
    ${rocsolver_auxiliary_source}
    ${common_source_paths}
    ${rocsolver_bench_common_source}
    PROPERTIES
      LANGUAGE HIP
  )
endif()

add_armor_flags(larft-example "${ARMOR_LEVEL}")

# maybe need to add hip::host
target_link_libraries(larft-example PUBLIC
  roc::rocblas
  rocsolver
  PRIVATE
  rocsolver-common
  clients-common
)


if(NOT USE_HIPCXX)
  target_link_libraries(larft-example PRIVATE hip::device)
endif()

target_compile_definitions(larft-example PRIVATE
  ROCM_USE_FLOAT16
  ROCSOLVER_CLIENTS_BENCH
)

target_include_directories(larft-example
  PRIVATE
    ${PROJECT_SOURCE_DIR}/library/include  # Add this first
    ${PROJECT_SOURCE_DIR}/library/src
    ${PROJECT_SOURCE_DIR}/library/src/auxiliary
    ${PROJECT_SOURCE_DIR}/library/src/include
    ${PROJECT_SOURCE_DIR}/common/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# don't know what this is
rocm_install(TARGETS larft-example COMPONENT llamas)
