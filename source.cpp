#include <cmath>
#include <vector>

template <typename IndexType>
class VectorN { // We will use VectorN as the index of n-dimensional array
    size_t dim;
    std::vector<IndexType> position;
public:

    VectorN(size_t _dim): dim(_dim) , position(_dim) {  }
    VectorN(const std::vector<IndexType>& _position): dim(_position.size()), position(_position) {  }
    IndexType& operator [] (const size_t where) { return position[where]; }
    const IndexType& operator [] (const size_t where) const { return position[where]; }

    void operator = (const VectorN& vec) {
        dim = vec.dim;
        position.clear();
        position.reserve(dim);
        for (int i = 0; i < dim; ++i) {
            position.push_back(vec.position[i]);
        }
    }

    // operators
    VectorN operator + (const VectorN& vec) const {
        if (dim != vec.dim) throw 2;
        std::vector<IndexType> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] + vec.position[i];
        }
        return VectorN(ret);
    }
    VectorN operator - (const VectorN& vec) const {
        if (dim != vec.dim) throw 2;
        std::vector<IndexType> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] - vec.position[i];
        }
        return VectorN(ret);
    }
    template <typename dtype>
    VectorN operator * (const dtype k) const {
        std::vector<IndexType> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] * k;
        }
        return VectorN(ret);
    }
    VectorN operator / (const dtype k) const {
        std::vector<IndexType> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] / k;
        }
        return VectorN(ret);
    }
    
    double Distance(const VectorN& vec) const {
        if (dim != vec.dim) throw 2;
        double D2 = 0;
        for (size_t i = 0; i != dim; ++i) {
            double delta = (position[i] - vec.position[i]);
            D2 += delta * delta;
        }
        return std::sqrt(D2);
    }
    double Length() const {
        double D2 = 0;
        for (size_t i = 0; i != dim; ++i) {
            D2 += position[i] * position[i];
        }
        return std::sqrt(D2);
    }

    bool InBoundary(IndexType minval, IndexType maxval) const {
        for (size_t i = 0; i != dim; ++i) {
            if (std::round(position[i]) < minval) return false;
            if (std::round(position[i]) >= maxval) return false;
        }
        return true;
    }

    IndexType ToIndex(IndexType dimLength) const {
        IndexType ret = 0;
        for (size_t i = 0; i != dim; ++i) {
            ret = ret * dimLength + position[i];
        }
        return ret;
    }
    void FromIndex(IndexType index, IndexType dimLength) {
        for (int i = (int)(dim - 1); i >= (int)0; --i) {
            position[i] = (IndexType)(index % dimLength);
            index /= dimLength;
        }
    }

    size_t GetDim() const { return dim; }
    const std::vector<IndexType>& GetPosition() const { return position; }
};

template <typename dtype, typename IndexType>
class ArrayN { // n-dimensional array
    size_t dim;
    size_t dimLength;
    std::vector<dtype> data;
public:
    ArrayN(size_t _dim, size_t _dimLength) : dim(_dim), dimLength(_dimLength), data((size_t)std::pow(_dimLength, _dim)) {  }
    ArrayN(size_t _dim, size_t _dimLength, const dtype& InitElement) : dim(_dim), dimLength(_dimLength), data((size_t)std::pow(_dimLength, _dim), InitElement) {  }

    dtype& At(const VectorN<IndexType>& pos) { // You can impletement operator[] instead
        if (pos.GetDim() != dim) throw 3;
        return data[pos.ToIndex(dimLength)];
    }
};
