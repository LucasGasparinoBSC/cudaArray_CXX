#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <cstdio>
#include "Array.hpp"
#include "ArrayDevice.cuh"

int main()
{
    // Create a host array of float type
    Array<float> a_h(100);

    // Create a device array
    ArrayDevice<float> a_d(100);

    // Create a chunk of a_d
    ArrayDevice<float> a_d_chunk(10);
    a_d.chunk(0,9,a_d_chunk);
    return 0;
}