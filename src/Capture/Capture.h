#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <chrono>
#include <cassert>
#include <thread>
#include <cuda_runtime_api.h>
#include <cuda_d3d11_interop.h>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <memory> 
#include "../Helpers/logger.h"
#include "../Security/xor.h"
#include "../Utils/Timer.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace capture {
    bool InitializeCapture();
    void _ReleaseCapture();
    bool captureScreenRegion(std::vector<BYTE>& capturedData);
    bool captureScreenRegionGPU(cv::cuda::GpuMat& gpuMat);
    
    void capture_cache_loop();
    bool get_cached_mat(cv::cuda::GpuMat &dst);
    bool should_update();
    void set_should_update(bool val);
}