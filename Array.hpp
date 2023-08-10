#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <cmath>

template <class T>
class Array
{
    // Attributes
    private:
        uint64_t _numEntries;
        uint64_t _numBytes;
        T* _dataPtr;
    public:
    // Methods
    private:
        // Size check
        void checkSize(uint64_t numEntries)
        {
            // Check that array is not empty
            if (numEntries == 0)
            {
                throw std::invalid_argument("Array size cannot be zero");
            }
        }
        // Bounds size check
        void checkBounds(uint64_t index)
        {
            // Check that index is within bounds
            if (index >= _numEntries)
            {
                throw std::out_of_range("Index out of bounds");
            }
        }
    public:
        // Default Constructor
        Array()
        {
            _numEntries = 0;
            _numBytes = 0;
            _dataPtr = NULL;
        }
        // Parametrized Constructor
        Array(uint64_t numEntries)
        {
            // Check that array is not empty
            checkSize(numEntries);
            _numEntries = numEntries;
            _numBytes = numEntries * sizeof(T);
            _dataPtr = (T*)malloc(_numBytes);
        }
        // Copy Constructor
        Array(const Array& other)
        {
            _numEntries = other._numEntries;
            _numBytes = other._numBytes;
            _dataPtr = (T*)malloc(_numBytes);
            memcpy(_dataPtr, other._dataPtr, _numBytes);
        }
        // Destructor
        ~Array()
        {
            // If _dataPtr is not NULL, free memory
            if (_dataPtr != NULL)
            {
                free(_dataPtr);
            }
        }
        // Operator Overloading
        T& operator[](uint64_t index)
        {
            // Check that index is within bounds
            checkBounds(index);
            return _dataPtr[index];
        }
        // Getters
        uint64_t getNumEntries()
        {
            return _numEntries;
        }
        uint64_t getNumBytes()
        {
            return _numBytes;
        }
        T* getDataPtr()
        {
            return _dataPtr;
        }
        // Creator
        void create(uint64_t numEntries)
        {
            // Check that array is not empty
            checkSize(numEntries);
            _numEntries = numEntries;
            _numBytes = numEntries * sizeof(T);
            _dataPtr = (T*)malloc(_numBytes);
            // Initialize array to zero
            memset(_dataPtr, 0, _numBytes);
        }
        // Resizer: use realloc to resize array
        // TODO: Implement this
        // Chunk: retrieves a contiguous chunk of the array
        void chunk(uint64_t start, uint64_t end, Array<T>& slice)
        {
            // Check that start and end are within bounds
            checkBounds(start);
            checkBounds(end);
            // Check that start is less than end
            if (start >= end)
            {
                throw std::invalid_argument("Start index must be less than end index");
            }
            // Check that slice size equals range
            uint64_t sliceSize = end - start + 1;
            if (sliceSize != slice.getNumEntries())
            {
                throw std::invalid_argument("Slice size must equal range");
            }
            // Copy data
            memcpy(slice.getDataPtr(), _dataPtr + start, slice.getNumBytes());
        }
        // Partial: retrieves a non-contiguous chunk of the array given a list of indices
        void partial(Array<uint64_t>& indices, Array<T>& slice)
        {
            // Check that number of indices equals slice size
            if (indices.getNumEntries() != slice.getNumEntries())
            {
                throw std::invalid_argument("Number of indices must equal slice size");
            }
            // Check that no index is out of bounds
            uint64_t maxIdx =indices.maxval();
            checkBounds(maxIdx);
            // Copy data
            for (uint64_t i = 0; i < indices.getNumEntries(); i++)
            {
                slice[i] = _dataPtr[indices[i]];
            }
        }
        // Maxval: returns the maximum value in the array
        T maxval()
        {
            // Initialize maxval to first element
            T maxval = _dataPtr[0];
            // Iterate through array
            for (uint64_t i = 1; i < _numEntries; i++)
            {
                // If current element is greater than maxval, update maxval
                if (_dataPtr[i] > maxval)
                {
                    maxval = _dataPtr[i];
                }
            }
            return maxval;
        }
        // Minval: returns the minimum value in the array
        T minval()
        {
            // Initialize minval to first element
            T minval = _dataPtr[0];
            // Iterate through array
            for (uint64_t i = 1; i < _numEntries; i++)
            {
                // If current element is less than minval, update minval
                if (_dataPtr[i] < minval)
                {
                    minval = _dataPtr[i];
                }
            }
            return minval;
        }
        // Sum: returns the sum of all elements in the array
        T sum()
        {
            // Check that _dataPtr is of numeric  type
            // TODO: Implement this
            // Initialize sum to zero
            T sum = 0;
            // Iterate through array
            for (uint64_t i = 0; i < _numEntries; i++)
            {
                // Add current element to sum
                sum += _dataPtr[i];
            }
            return sum;
        }
        // Norm: returns the norm of the array
        double norm()
        {
            // If _dataPtr is an integer type, convert to double
            if(std::is_integral<T>::value)
            {
                Array<double> doubleArray(_numEntries);
                for (uint64_t i = 0; i < _numEntries; i++)
                {
                    doubleArray[i] = (double)_dataPtr[i];
                }
            }
            // If type is float,  convert to double
            if(std::is_same<T, float>::value)
            {
                Array<double> doubleArray(_numEntries);
                for (uint64_t i = 0; i < _numEntries; i++)
                {
                    doubleArray[i] = (double)_dataPtr[i];
                }
            }
            // If _dataPtr is a char, string or boolean type, throw exception
            // TODO: Implement this
            // Initialize norm to zero
            T norm = 0;
            // Iterate through array
            for (uint64_t i = 0; i < _numEntries; i++)
            {
                // Add square of current element to norm
                norm += _dataPtr[i] * _dataPtr[i];
            }
            return sqrt(norm);
        }
};

#endif // __ARRAY_HPP__