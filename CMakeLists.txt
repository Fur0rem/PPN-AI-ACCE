cmake_minimum_required(VERSION 3.10)
project(PPN-AI-ACCE)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
# set(SANITIZER_LIB -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined)
set(SANITIZER_LIB -Og -g3)
set(DEBUG_FLAGS -Og -g -Wall -Wextra ${SANITIZER_LIB})
set(RELEASE_FLAGS -Og -g3 -ffast-math -funsafe-math-optimizations)

set(TEST_FLAGS --coverage ${SANITIZER_LIB})
set(NANOBENCH_LINK_FLAGS -I${CMAKE_SOURCE_DIR}/nanobench/src/include -fno-eliminate-unused-debug-types)
set(CODELET_FLAGS -lm ${NANOBENCH_LINK_FLAGS} -O3)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/nanobench/src/include)

# Add the Eigen library
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Use Vulkan for GPU implementation
find_package(Vulkan REQUIRED)

# Add the nanobench library
add_library(nanobench STATIC ${CMAKE_SOURCE_DIR}/nanobench/src/test/app/nanobench.cpp)
target_include_directories(nanobench PUBLIC ${CMAKE_SOURCE_DIR}/nanobench/src/include)

set(SRCS
    ${CMAKE_SOURCE_DIR}/src/parsing/iparser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/asm_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_with_split_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/hexadecimal_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/hexadecimal_with_split_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/xor_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/non_encoder.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/cycles_encoders.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/size_encoder.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/activation_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/gradients.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/optimiser.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/neural_network.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/sparse_multiplication.cpp
    ${CMAKE_SOURCE_DIR}/src/dataset/dataset.cpp
    ${CMAKE_SOURCE_DIR}/src/culkan/culkan.cpp
)
# Add the main executable
add_executable(main 
    ${SRCS}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
target_include_directories(main PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(main nanobench OpenMP::OpenMP_CXX ${Vulkan_LIBRARIES})
target_compile_options(main PRIVATE ${RELEASE_FLAGS} -fopenmp)

# Add the topology_finder executable
add_executable(topology_finder
    ${SRCS}
    ${CMAKE_SOURCE_DIR}/src/topology_finder.cpp
)
target_link_libraries(topology_finder nanobench)
target_compile_options(topology_finder PRIVATE ${RELEASE_FLAGS})

# Add the extract data executable
add_executable(extract_data
    ${CMAKE_SOURCE_DIR}/src/dataset/extract_data.cpp
)
target_link_libraries(extract_data)
target_compile_options(extract_data PRIVATE ${RELEASE_FLAGS})

# Add the test executables
enable_testing()
find_package(GTest REQUIRED)

set(TESTS
    asm_parser
    binary_with_split_parser
    binary_parser
    hexadecimal_parser
    hexadecimal_with_split_parser
    xor_parser
    recognises_xor
    cycles_encoders
    sparse_multiplication
    gpu
)

foreach(TEST ${TESTS})
    add_executable(test_${TEST} ${CMAKE_SOURCE_DIR}/tests/${TEST}.cpp)
    target_sources(test_${TEST} PRIVATE ${SRCS})
    target_sources(test_${TEST} PRIVATE ${CMAKE_SOURCE_DIR}/src/culkan/culkan.cpp)
    target_link_libraries(test_${TEST} PUBLIC GTest::GTest GTest::Main ${Vulkan_LIBRARIES})
    target_include_directories(test_${TEST} PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_compile_options(test_${TEST} PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
    target_link_options(test_${TEST} PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
    add_test(NAME test_${TEST} COMMAND test_${TEST}
)
endforeach()

# Add the codelets executables for the dataset
# PPN-AI-ACCE/build]$ find ../codelets -type f -name "*.cpp" | sed 's/\.cpp$//' | sed 's|../codelets/||' | sort | grep -v dummy
set(CODELETS
    array_add
    array_copy
    array_min
    codelet_c_k_means_line109
    codelet_crystal_cholesky
    codelet_generate_seq_laplacian_line73
    codelet_kernels_serial_amr_c_main_line500_0
    codelet_kernels_serial_amr_c_main_line502_0
    codelet_kernels_serial_amr_c_main_line506_0
    codelet_kernels_serial_amr_c_main_line508_0
    codelet_kernels_serial_sparse_c_main_line236_0
    codelet_kernels_serial_sparse_c_main_line256_0
    codelet_kernels_serial_transpose_c_main_line176_0
    codelet_kernels_stencil_c_main_line259_0
    codelet_liblinear
    codelet_libraries_default_daala_dct_c_compute_mse_line8488_0
    codelet_libraries_default_daala_dct_c_compute_mse_line8488_8
    codelet_libraries_default_daala_dct_c_compute_mse_line8497_0
    codelet_libraries_default_daala_dct_c_dynamic_range_line8685_0
    codelet_libraries_default_libsndfile_alac_c_alac_read_d_line575_0
    codelet_libraries_default_twolame_encode_c_combine_lr_line330_0
    codelet_libraries_default_twolame_encode_c_combine_lr_line330_1
    codelet_libraries_default_twolame_psycho_1_c_psycho_1_hann_fft_pickmax_line184_0
    codelet_libraries_default_twolame_psycho_3_c_psycho_3_fft_line89_0
    codelet_livermore_default_lloops_c_endloop_line1795_0
    codelet_livermore_default_lloops_c_endloop_line1795_3
    codelet_livermore_default_lloops_c_kernels_line1031_0
    codelet_livermore_default_lloops_c_kernels_line1113_0
    codelet_livermore_default_lloops_c_kernels_line1113_3
    codelet_livermore_default_lloops_c_kernels_line1340_0
    codelet_livermore_default_lloops_c_kernels_line1351_0
    codelet_livermore_default_lloops_c_kernels_line1351_64
    codelet_livermore_default_lloops_c_kernels_line1444_0
    codelet_livermore_default_lloops_c_kernels_line1444_144
    codelet_livermore_default_lloops_c_kernels_line1444_156
    codelet_livermore_default_lloops_c_kernels_line771_0
    codelet_livermore_default_lloops_c_kernels_line869_0
    codelet_livermore_default_lloops_c_kernels_line913_0
    codelet_livermore_default_lloops_c_kernels_line937_0
    codelet_livermore_default_lloops_c_kernels_line971_0
    codelet_livermore_default_lloops_c_kernels_line993_0
    codelet_netlib
    codelet_neural_runcont
    codelet_nn_backpropagate_second_dervatives_line262
    codelet_npb_BT_bt_c_add_line179_0
    codelet_npb_BT_bt_c_compute_rhs_line1771_0
    codelet_npb_BT_bt_c_compute_rhs_line1808_0
    codelet_npb_BT_bt_c_compute_rhs_line1869_0
    codelet_npb_BT_bt_c_compute_rhs_line1881_0
    codelet_npb_BT_bt_c_compute_rhs_line1892_0
    codelet_npb_BT_bt_c_compute_rhs_line1892_352
    codelet_npb_BT_bt_c_compute_rhs_line1892_361
    codelet_npb_BT_bt_c_compute_rhs_line1892_id4o6_jl1f2_tl3s128_tl4s8
    codelet_npb_BT_bt_c_compute_rhs_line1907_0
    codelet_npb_BT_bt_c_compute_rhs_line1919_0
    codelet_npb_BT_bt_c_compute_rhs_line1933_0
    codelet_npb_BT_bt_c_compute_rhs_line1989_0
    codelet_npb_BT_bt_c_compute_rhs_line1989_12
    codelet_npb_BT_bt_c_compute_rhs_line2001_0
    codelet_npb_BT_bt_c_compute_rhs_line2012_0
    codelet_npb_BT_bt_c_compute_rhs_line2012_18
    codelet_npb_BT_bt_c_compute_rhs_line2027_0
    codelet_npb_BT_bt_c_compute_rhs_line2039_0
    codelet_npb_BT_bt_c_compute_rhs_line2039_12
    codelet_npb_BT_bt_c_compute_rhs_line2053_0
    codelet_npb_BT_bt_c_compute_rhs_line2110_0
    codelet_npb_BT_bt_c_compute_rhs_line2122_0
    codelet_npb_BT_bt_c_compute_rhs_line2122_12
    codelet_npb_BT_bt_c_compute_rhs_line2133_0
    codelet_npb_BT_bt_c_compute_rhs_line2133_15
    codelet_npb_BT_bt_c_compute_rhs_line2148_0
    codelet_npb_BT_bt_c_compute_rhs_line2148_12
    codelet_npb_BT_bt_c_compute_rhs_line2160_0
    codelet_npb_BT_bt_c_compute_rhs_line2171_0
    codelet_npb_BT_bt_c_compute_rhs_line2171_1088
    codelet_npb_BT_bt_c_exact_rhs_line300_0
    codelet_npb_BT_bt_c_exact_rhs_line300_1
    codelet_npb_BT_bt_c_exact_rhs_line300_358
    codelet_npb_BT_bt_c_exact_rhs_line634_0
    codelet_npb_BT_bt_c_lhsy_line1191_0
    codelet_npb_BT_bt_c_lhsy_line1293_0
    codelet_npb_BT_bt_c_lhsz_line1485_0
    codelet_npb_BT_bt_c_rhs_norm_line261_0
    codelet_npb_BT_bt_c_rhs_norm_line261_3
    codelet_npb_BT_bt_c_x_solve_cell_line2730_0
    codelet_npb_BT_bt_c_y_solve_cell_line2884_0
    codelet_npb_CG_cg_c_conj_grad_line373_0
    codelet_npb_CG_cg_c_conj_grad_line387_0
    codelet_npb_CG_cg_c_conj_grad_line549_0
    codelet_npb_CG_cg_c_main_line215_0
    codelet_npb_CG_cg_c_main_line227_0
    codelet_npb_CG_cg_c_main_line271_0
    codelet_npb_CG_cg_c_main_line290_0
    codelet_npb_CG_cg_c_sparse_line757_0
    codelet_npb_FT_ft_c_compute_indexmap_line422_0
    codelet_npb_LU_lu_c_blts_line193_0
    codelet_npb_LU_lu_c_blts_line193_9
    codelet_npb_LU_lu_c_erhs_line678_0
    codelet_npb_LU_lu_c_erhs_line678_1
    codelet_npb_LU_lu_c_erhs_line678_72
    codelet_npb_LU_lu_c_erhs_line724_0
    codelet_npb_LU_lu_c_erhs_line861_0
    codelet_npb_LU_lu_c_l2norm_line1921_0
    codelet_npb_LU_lu_c_pintgr_line1988_0
    codelet_npb_LU_lu_c_pintgr_line2019_0
    codelet_npb_LU_lu_c_pintgr_line2037_0
    codelet_npb_LU_lu_c_pintgr_line2073_0
    codelet_npb_LU_lu_c_pintgr_line2092_0
    codelet_npb_LU_lu_c_pintgr_line2128_0
    codelet_npb_LU_lu_c_rhs_line2259_0
    codelet_npb_LU_lu_c_rhs_line2277_0
    codelet_npb_LU_lu_c_rhs_line2418_0
    codelet_npb_LU_lu_c_ssor_line2976_0
    codelet_npb_LU_lu_c_ssor_line3018_0
    codelet_npb_LU_lu_c_ssor_line3067_0
    codelet_npb_SP_sp_c_add_line174_0
    codelet_npb_SP_sp_c_compute_rhs_line1327_0
    codelet_npb_SP_sp_c_compute_rhs_line1387_0
    codelet_npb_SP_sp_c_compute_rhs_line1398_0
    codelet_npb_SP_sp_c_compute_rhs_line1409_0
    codelet_npb_SP_sp_c_compute_rhs_line1424_0
    codelet_npb_SP_sp_c_compute_rhs_line1436_0
    codelet_npb_SP_sp_c_compute_rhs_line1452_0
    codelet_npb_SP_sp_c_compute_rhs_line1508_0
    codelet_npb_SP_sp_c_compute_rhs_line1520_0
    codelet_npb_SP_sp_c_compute_rhs_line1531_0
    codelet_npb_SP_sp_c_compute_rhs_line1546_0
    codelet_npb_SP_sp_c_compute_rhs_line1558_0
    codelet_npb_SP_sp_c_compute_rhs_line1573_0
    codelet_npb_SP_sp_c_compute_rhs_line1630_0
    codelet_npb_SP_sp_c_compute_rhs_line1642_0
    codelet_npb_SP_sp_c_compute_rhs_line1653_0
    codelet_npb_SP_sp_c_compute_rhs_line1668_0
    codelet_npb_SP_sp_c_compute_rhs_line1680_0
    codelet_npb_SP_sp_c_compute_rhs_line1691_0
    codelet_npb_SP_sp_c_exact_rhs_line301_0
    codelet_npb_SP_sp_c_exact_rhs_line301_576
    codelet_npb_SP_sp_c_exact_rhs_line601_0
    codelet_npb_SP_sp_c_initialize_line635_0
    codelet_npb_SP_sp_c_lhsinit_line811_0
    codelet_npb_SP_sp_c_lhsinit_line811_612
    codelet_npb_SP_sp_c_lhsx_line885_0
    codelet_npb_SP_sp_c_lhsx_line898_0
    codelet_npb_SP_sp_c_lhsx_line912_0
    codelet_npb_SP_sp_c_lhsx_line930_0
    codelet_npb_SP_sp_c_lhsy_line1014_0
    codelet_npb_SP_sp_c_lhsy_line1028_0
    codelet_npb_SP_sp_c_lhsy_line1045_0
    codelet_npb_SP_sp_c_lhsy_line999_0
    codelet_npb_SP_sp_c_lhsz_line1114_0
    codelet_npb_SP_sp_c_lhsz_line1128_0
    codelet_npb_SP_sp_c_lhsz_line1142_0
    codelet_npb_SP_sp_c_lhsz_line1159_0
    codelet_npb_SP_sp_c_ninvr_line1196_0
    codelet_npb_SP_sp_c_pinvr_line1235_0
    codelet_npb_SP_sp_c_rhs_norm_line264_0
    codelet_npb_SP_sp_c_rhs_norm_line264_3
    codelet_npb_SP_sp_c_txinvr_line1919_0
    codelet_npb_SP_sp_c_tzetar_line1968_0
    codelet_npb_SP_sp_c_x_solve_line2449_0
    codelet_npb_SP_sp_c_x_solve_line2449_120
    codelet_npb_SP_sp_c_y_solve_line2675_0
    codelet_npb_SP_sp_c_y_solve_line2675_120
    codelet_npb_SP_sp_c_z_solve_line2901_0
    codelet_npb_SP_sp_c_z_solve_line2901_120
    codelet_npb_UA_SNU_adapt_c_remap_line868_0
    codelet_npb_UA_SNU_adapt_c_remap_line868_12
    codelet_npb_UA_SNU_adapt_c_remap_line894_0
    codelet_npb_UA_SNU_adapt_c_remap_line894_288
    codelet_npb_UA_SNU_adapt_c_remap_line894_306
    codelet_npb_UA_SNU_convect_c_convect_line223_0
    codelet_npb_UA_SNU_convect_c_convect_line223_30
    codelet_npb_UA_SNU_convect_c_convect_line223_36
    codelet_npb_UA_SNU_convect_c_convect_line243_0
    codelet_npb_UA_SNU_diffuse_c_diffusion_line142_0
    codelet_npb_UA_SNU_diffuse_c_diffusion_line142_9
    codelet_npb_UA_SNU_diffuse_c_diffusion_line153_0
    codelet_npb_UA_SNU_diffuse_c_diffusion_line63_0
    codelet_npb_UA_SNU_diffuse_c_diffusion_line63_12
    codelet_npb_UA_SNU_diffuse_c_diffusion_line75_0
    codelet_npb_UA_SNU_diffuse_c_laplacian_line190_0
    codelet_npb_UA_SNU_diffuse_c_laplacian_line201_0
    codelet_npb_UA_SNU_precond_c_setuppc_line63_0
    codelet_npb_UA_SNU_precond_c_setuppc_line63_2
    codelet_npb_UA_SNU_precond_c_setuppc_line87_0
    codelet_npb_UA_SNU_ua_c_main_line363_0
    codelet_npb_UA_SNU_utils_c_add2_line170_0
    codelet_npb_UA_SNU_utils_c_adds1m1_line132_0
    codelet_npb_UA_SNU_utils_c_adds2m1_line120_0
    codelet_npb_UA_SNU_utils_c_col2_line145_0
    codelet_npb_UA_SNU_utils_c_reciprocal_line43_0
    codelet_pifft
    codelet_polybench_datamining_correlation_correlation_c_init_array_line35_0
    codelet_polybench_datamining_correlation_correlation_c_init_array_line35_8
    codelet_polybench_datamining_correlation_correlation_c_init_array_line35_jl0f4_tl1s128_tl2s128
    codelet_polybench_datamining_covariance_covariance_c_init_array_line34_0
    codelet_polybench_datamining_covariance_covariance_c_kernel_covariance_line81_0
    codelet_polybench_linear_algebra_blas_gemm_gemm_c_init_array_line37_0
    codelet_polybench_linear_algebra_blas_gemm_gemm_c_init_array_line40_0
    codelet_polybench_linear_algebra_blas_gemm_gemm_c_init_array_line43_0
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line101_0
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line105_0
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line105_36
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line105_44
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line105_id2o1_jl0f4_tl1s128_tl2s128
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line109_0
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line112_0
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line112_8
    codelet_polybench_linear_algebra_blas_gemver_gemver_c_kernel_gemver_line112_jl0f4_tl1s128_tl2s128
    codelet_polybench_linear_algebra_blas_gesummv_gesummv_c_init_array_line37_0
    codelet_polybench_linear_algebra_blas_gesummv_gesummv_c_init_array_line37_1
    codelet_polybench_linear_algebra_blas_symm_symm_c_init_array_line37_0
    codelet_polybench_linear_algebra_blas_symm_symm_c_init_array_line37_8
    codelet_polybench_linear_algebra_blas_symm_symm_c_init_array_line37_jl0f4_tl1s128_tl2s128
    codelet_polybench_linear_algebra_blas_syr2k_syr2k_c_init_array_line37_0
    codelet_polybench_linear_algebra_blas_syr2k_syr2k_c_init_array_line42_0
    codelet_polybench_linear_algebra_blas_syr2k_syr2k_c_kernel_syr2k_line87_0
    codelet_polybench_linear_algebra_blas_syrk_syrk_c_init_array_line36_0
    codelet_polybench_linear_algebra_blas_syrk_syrk_c_init_array_line39_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_init_array_line38_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_init_array_line41_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_init_array_line44_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_init_array_line47_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_kernel_2mm_line89_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_kernel_2mm_line96_0
    codelet_polybench_linear_algebra_kernels_2mm_2mm_c_kernel_2mm_line96_8
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_init_array_line34_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_init_array_line37_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_init_array_line40_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_init_array_line43_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line101_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line101_8
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line85_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line85_8
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line93_0
    codelet_polybench_linear_algebra_kernels_3mm_3mm_c_kernel_3mm_line93_8
    codelet_polybench_linear_algebra_kernels_atax_atax_c_init_array_line34_0
    codelet_polybench_linear_algebra_kernels_atax_atax_c_init_array_line36_0
    codelet_polybench_linear_algebra_kernels_atax_atax_c_init_array_line36_4
    codelet_polybench_linear_algebra_kernels_atax_atax_c_kernel_atax_line74_0
    codelet_polybench_linear_algebra_kernels_bicg_bicg_c_init_array_line33_0
    codelet_polybench_linear_algebra_kernels_doitgen_doitgen_c_init_array_line32_0
    codelet_polybench_linear_algebra_kernels_doitgen_doitgen_c_init_array_line36_0
    codelet_polybench_linear_algebra_kernels_mvt_mvt_c_kernel_mvt_line88_0
    codelet_polybench_linear_algebra_kernels_mvt_mvt_c_kernel_mvt_line91_0
    codelet_polybench_linear_algebra_kernels_mvt_mvt_c_kernel_mvt_line91_36
    codelet_polybench_linear_algebra_kernels_mvt_mvt_c_kernel_mvt_line91_44
    codelet_polybench_linear_algebra_kernels_mvt_mvt_c_kernel_mvt_line91_id2o1_jl0f4_tl1s128_tl2s128
    codelet_polybench_linear_algebra_solvers_durbin_durbin_c_init_array_line31_0
    codelet_polybench_linear_algebra_solvers_gramschmidt_gramschmidt_c_init_array_line33_0
    codelet_polybench_linear_algebra_solvers_gramschmidt_gramschmidt_c_init_array_line33_1
    codelet_polybench_linear_algebra_solvers_gramschmidt_gramschmidt_c_init_array_line38_0
    codelet_polybench_linear_algebra_solvers_ludcmp_ludcmp_c_init_array_line35_0
    codelet_polybench_medley_deriche_deriche_c_init_array_line35_0
    codelet_polybench_medley_deriche_deriche_c_init_array_line35_4
    codelet_polybench_medley_deriche_deriche_c_init_array_line35_jl0f2_tl1s256_tl2s256
    codelet_polybench_medley_deriche_deriche_c_kernel_deriche_line118_0
    codelet_polybench_medley_deriche_deriche_c_kernel_deriche_line150_0
    codelet_polybench_medley_nussinov_nussinov_c_init_array_line42_0
    codelet_polybench_stencils_adi_adi_c_init_array_line31_0
    codelet_polybench_stencils_fdtd_2d_fdtd_2d_c_init_array_line38_0
    codelet_polybench_stencils_jacobi_1d_jacobi_1d_c_init_array_line32_0
    codelet_polybench_stencils_jacobi_2d_jacobi_2d_c_init_array_line32_0
    codelet_polybench_stencils_seidel_2d_seidel_2d_c_init_array_line31_0
    codelet_rbm_train
    codelet_spec2000
    codelet_srr_engine
    codelet_tsvc
    codelet_vector_c_hypre_seq_vector_axpy_line384
    crypto
    fibonacci
    mat_mul
    nbody
    poisson
    repeated_hash
    reverse_complement
    sorting
)


foreach(CODELET ${CODELETS})
    add_executable(codelet_${CODELET} ${CMAKE_SOURCE_DIR}/codelets/${CODELET}.cpp)
    target_link_libraries(codelet_${CODELET} nanobench)
    target_compile_options(codelet_${CODELET} PRIVATE ${RELEASE_FLAGS} -mavx2)
    target_link_options(codelet_${CODELET} PRIVATE ${CODELET_FLAGS})
endforeach()


# Add custom targets
add_custom_target(check 
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_asm_parser test_binary_parser test_xor_parser test_binary_with_split_parser test_recognises_xor test_hexadecimal_parser test_hexadecimal_with_split_parser test_cycles_encoders
)

# Add clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
)

# Generate a cmake_clean.cmake file to clean the build directory
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    "file(GLOB_RECURSE files \"${CMAKE_BINARY_DIR}/*\")\n"
    "foreach(file \${files})\n"
    "    if(NOT IS_DIRECTORY \${file})\n"
    "        file(REMOVE \${file})\n"
    "    endif()\n"
    "endforeach()\n"
)

enable_testing()

set(BENCHMARKS 
    training
    sparse_multiplication
    approximations
    gpu
)

foreach(BENCHMARK ${BENCHMARKS})
    add_executable(benchmark_${BENCHMARK} ${CMAKE_SOURCE_DIR}/benchmarks/${BENCHMARK}.cpp)
    target_sources(benchmark_${BENCHMARK} PRIVATE ${SRCS})
    target_link_libraries(benchmark_${BENCHMARK} nanobench OpenMP::OpenMP_CXX ${Vulkan_LIBRARIES})
    target_include_directories(benchmark_${BENCHMARK} PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_compile_options(benchmark_${BENCHMARK} PRIVATE ${RELEASE_FLAGS})
    target_link_options(benchmark_${BENCHMARK} PRIVATE ${CODELET_FLAGS})
endforeach()
