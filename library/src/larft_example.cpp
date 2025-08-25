#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include <hip/hip_runtime.h>

#include "auxiliary/rocauxiliary_larft.hpp"
#include "rocsolver_datatype2string.hpp"

ROCSOLVER_BEGIN_NAMESPACE

void read_string(const std::string& str, std::vector<float>& matrix, char delim = ' '){
    std::string segment;
    std::stringstream ss(str);

    while (std::getline(ss, segment, delim)) {
        try {
            matrix.push_back(std::stof(segment));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << " for segment: " << segment << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << " for segment: " << segment << std::endl;
        }
    }
}

void read_matrix(const char* filename, std::vector<float>& V, std::vector<float>& tau, std::vector<float>& F, char delim = ' '){
    std::vector<float> matrix;
    std::ifstream file(filename);
    std::string segmentV;
    std::string segmentTau;
    std::string segmentF;

    std::getline(file, segmentV);
    std::getline(file, segmentTau);
    std::getline(file, segmentF);

    file.close();

    read_string(segmentV, V, delim);
    read_string(segmentTau, tau, delim);
    read_string(segmentF, F, delim);
}


int main(int argc, char* argv[]){
    //
    /*
        This program should be invoked with 2 or 3 command line arguments:
            1) the path to a text file containing the input matrix
            2) the path to a text file containing the kernel arguments
            3) (optional) a string "yes" (case insensitive).
                This will enable verification mode, where the matrix output by the kernel is compared to the recorded output matrix
    */
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: ./larft-example <matrix input file> <kernel args input file> [yes]" << std::endl;
        return 1;
    }

    if (argc == 4){
        std::string verify_arg = argv[3];
        if (verify_arg != "yes" && verify_arg != "YES" && verify_arg != "Yes"){
            std::cerr << "Third argument must be 'yes' to enable verification mode" << std::endl;
            return 1;
        }
    }

    bool verify = (argc == 4);

    char storev_char;
    int n_int;
    int k_int;
    int shiftV_int;
    int ldv_int;
    int strideV_int;
    int strideT_int;
    int ldf_int;
    int strideF_int;

    int grid_dim_x;
    int grid_dim_y;
    int grid_dim_z;
    int block_dim_x;
    int block_dim_y;
    int block_dim_z;
    int lmemsize;

    rocblas_storev storev;
    rocblas_int n;
    rocblas_int k;
    rocblas_int shiftV;
    rocblas_int ldv;
    rocblas_stride strideV;
    rocblas_stride strideT;
    rocblas_int ldf;
    rocblas_stride strideF;

    std::vector<float> hV;
    std::vector<float> hTau;
    std::vector<float> hT;

    read_matrix(argv[1], hV, hTau, hT);

    float* dV;
    float* dTau;
    float* dF;

    hipMalloc(&dV, hV.size() * sizeof(float));
    hipMalloc(&dTau, hTau.size() * sizeof(float));
    hipMalloc(&dF, hT.size() * sizeof(float));

    hipMemcpy(dV, hV.data(), hV.size() * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(dTau, hTau.data(), hTau.size() * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(dF, hT.data(), hT.size() * sizeof(float), hipMemcpyHostToDevice);

    FILE* arguments_file = fopen(argv[2], "r");
    fscanf(arguments_file, "rocsolver_larft_template (grid dim x: %d, grid dim y: %d, grid dim z: %d, block dim x: %d, block dim y: %d, block dim z: %d, lmemsize: %d, storev: %c, n: %d, k: %d, shiftV: %d, ldv: %d, strideV: %d, strideT: %d, ldf: %d, strideF: %d)", &grid_dim_x, &grid_dim_y, &grid_dim_z, &block_dim_x, &block_dim_y, &block_dim_z, &lmemsize, &storev_char, &n_int, &k_int, &shiftV_int, &ldv_int, &strideV_int, &strideT_int, &ldf_int, &strideF_int);

    storev = rocsolver::char2rocblas_storev(storev_char);

    // optimized kernel call
    hipLaunchKernelGGL(larft_kernel_forward, dim3(grid_dim_x, grid_dim_y, grid_dim_z), dim3(block_dim_x, block_dim_y, block_dim_z), lmemsize, 0, storev, n_int, k_int, dV, shiftV_int, ldv_int, strideV_int, dTau, strideT_int, dF, ldf_int, strideF_int);

    if (verify){
        // copy back optimized output data
        std::vector<float> optimized_output(hT.size());
        hipMemcpy(optimized_output.data(), dF, optimized_output.size() * sizeof(float), hipMemcpyDeviceToHost);

        // copy original input to already allocated device memory
        hipMemcpy(dV, hV.data(), hV.size() * sizeof(float), hipMemcpyHostToDevice);
        hipMemcpy(dTau, hTau.data(), hTau.size() * sizeof(float), hipMemcpyHostToDevice);
        hipMemcpy(dF, hT.data(), hT.size() * sizeof(float), hipMemcpyHostToDevice);

        // call original kernel
        hipLaunchKernelGGL(larft_kernel_forward, dim3(grid_dim_x, grid_dim_y, grid_dim_z), dim3(block_dim_x, block_dim_y, block_dim_z), lmemsize, 0, storev, n_int, k_int, dV, shiftV_int, ldv_int, strideV_int, dTau, strideT_int, dF, ldf_int, strideF_int);

        // copy back original output
        hipMemcpy(hT.data(), dF, optimized_output.size() * sizeof(float), hipMemcpyDeviceToHost);

        // verify using l2 norm
        float norm = 0.0f;

        float tolerance = 1e-5f;

        for (int i=0; i < hT.size(); ++i){
            norm += (hT[i] - optimized_output[i]) * (hT[i] - optimized_output[i]);
        }

        assert(norm < tolerance);
    }

    return 0;
}

ROCSOLVER_END_NAMESPACE

int main(int argc, char* argv[]){
    return rocsolver::main(argc, argv);
}
