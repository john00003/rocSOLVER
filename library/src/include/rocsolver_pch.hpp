/* **************************************************************************
 * Copyright (C) 2019-2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * *************************************************************************/

#pragma once

/*
 * ===========================================================================
 *    rocSOLVER Precompiled Header
 *    
 *    This header contains the most commonly used headers across the rocSOLVER
 *    project to improve compilation performance through precompilation.
 *    It includes:
 *    - Standard C++ library headers
 *    - ROCm/HIP runtime headers
 *    - rocBLAS headers  
 *    - External library headers (fmt)
 *    - Common project utility headers
 * ===========================================================================
 */

// ========================================
// Standard C++ Library Headers
// ========================================
#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// ========================================
// ROCm/HIP Headers
// ========================================
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>

// ========================================
// rocBLAS Headers
// ========================================
#include <rocblas/rocblas.h>

// ========================================
// External Library Headers
// ========================================
#include <fmt/core.h>
#include <fmt/ostream.h>

// ========================================
// Common Project Headers
// ========================================
// #include "rocblas_utility.hpp"
// #include "rocsolver_datatype2string.hpp"
// #include "common_host_helpers.hpp"
