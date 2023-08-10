#ifndef __ARRAYDEVICE_CUH__
#define __ARRAYDEVICE_CUH__

#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <cmath>
#include <cuda.h>
#include <cuda_runtime.h>
#include "Array.hpp"

template <class T>
class ArrayDevice
{
    // Attributes
    private:
        uint64_t _size;
        uint64_t _sizeBytes;
        T* _data;
    // Methods
    private:
        // Check for numerical type
        // TODO: implement
    public:
        // Default constructor
        ArrayDevice()
        {
            _size = 0;
            _sizeBytes = 0;
            _data = NULL;
        }
        // Parameterized constructor
        ArrayDevice(uint64_t size)
        {
            // Check size is not 0
            if (size == 0)
            {
                throw std::invalid_argument("ArrayDevice size cannot be 0");
            }
            _size = size;
            _sizeBytes = size * sizeof(T);
            cudaMalloc((void**)&_data, _sizeBytes);
            // Initialize to 0
            cudaMemset(_data, 0, _sizeBytes);
        }
        // Copy constructor
        ArrayDevice(const ArrayDevice &other)
        {
            _size = other._size;
            _sizeBytes = other._sizeBytes;
            cudaMalloc((void**)&_data, _sizeBytes);
            cudaMemcpy(_data, other._data, _sizeBytes, cudaMemcpyDeviceToDevice);
        }
        // Destructor
        ~ArrayDevice()
        {
            // If data is not NULL, free it
            if (_data != NULL)
            {
                cudaFree(_data);
            }
        }
        // Operator overloading
        T& operator[](uint64_t index)
        {
            // Check index is within bounds
            if (index >= _size)
            {
                throw std::out_of_range("ArrayDevice index out of bounds");
            }
            return _data[index];
        }
        // Getters
        uint64_t size() const
        {
            return _size;
        }
        T* data() const
        {
            return _data;
        }
        // Creator
        void create(uint64_t size)
        {
            // Check size is not 0
            if (size == 0)
            {
                throw std::invalid_argument("ArrayDevice size cannot be 0");
            }
            // If data is not NULL, abort
            if (_data != NULL)
            {
                throw std::runtime_error("ArrayDevice already created");
            }
            _size = size;
            _sizeBytes = size * sizeof(T);
            cudaMalloc((void**)&_data, _sizeBytes);
            // Initialize to 0
            cudaMemset(_data, 0, _sizeBytes);
        }
        // Chunk: retrieve a contiguous chunk of the array
        void chunk(uint64_t start, uint64_t end, ArrayDevice<T>& slice)
        {
            // Check start and end are within bounds
            if (start >= _size || end >= _size)
            {
                throw std::out_of_range("ArrayDevice chunk start or end out of bounds");
            }
            // Check start is before end
            if (start > end)
            {
                throw std::invalid_argument("ArrayDevice chunk start must be before end");
            }
            // Check slice size is correct
            if (slice._size != end - start + 1)
            {
                throw std::invalid_argument("ArrayDevice chunk slice size is incorrect");
            }
            cudaMemcpy(slice._data, _data + start, slice._sizeBytes, cudaMemcpyDeviceToDevice);
        }
};

#endif