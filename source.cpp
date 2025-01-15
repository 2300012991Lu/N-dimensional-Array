#include <vector>

class VectorN { // We will use VectorN as the index of n-dimensional array
    size_t dim;
    std::vector<double> position;

    static void rec(int depth, int bound, const VectorN& lastVector, std::vector<VectorN>& vecs) {
        if (depth < 0) {
            vecs.push_back(lastVector);
            return;
        }
        for (int i = -bound; i <= bound; ++i) {
            VectorN v = lastVector;
            v[depth] = (double)i;
            rec(depth - 1, bound, v, vecs);
        }
    }
public:

    static std::vector<VectorN> GetNeighbours(const size_t _dim) {
        int bound = (int)std::ceil(std::sqrt(_dim));
        std::vector<VectorN> ret;
        std::vector<double> primary(_dim, 0);
        rec((int)_dim - 1, bound, VectorN(primary), ret);
        return ret;
    }

    VectorN(size_t _dim): dim(_dim) , position(_dim) {  }
    VectorN(const std::vector<double>& _position): dim(_position.size()), position(_position) {  }
    double& operator [] (const size_t where) { return position[where]; }
    const double& operator [] (const size_t where) const { return position[where]; }

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
        std::vector<double> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] + vec.position[i];
        }
        return VectorN(ret);
    }
    VectorN operator - (const VectorN& vec) const {
        if (dim != vec.dim) throw 2;
        std::vector<double> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] - vec.position[i];
        }
        return VectorN(ret);
    }
    VectorN operator * (const double k) const {
        std::vector<double> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = position[i] * k;
        }
        return VectorN(ret);
    }
    // I want to use division as integer division . If you want to use this as a normal one , you can remove "std::floor" .
    VectorN operator / (const double k) const {
        std::vector<double> ret(dim);
        for (size_t i = 0; i != dim; ++i) {
            ret[i] = std::floor(position[i] / k);
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

    bool InBoundary(int minval, int maxval) const {
        for (size_t i = 0; i != dim; ++i) {
            if (std::round(position[i]) < minval) return false;
            if (std::round(position[i]) >= maxval) return false;
        }
        return true;
    }

    int ToInt(int dimLength) const {
        int ret = 0;
        for (size_t i = 0; i != dim; ++i) {
            ret = ret * dimLength + (int)position[i];
        }
        return ret;
    }
    void FromInt(int index, int dimLength) {
        for (int i = (int)dim - 1; i >= 0; --i) {
            position[i] = double(index % dimLength);
            index /= dimLength;
        }
    }

    size_t GetDim() const { return dim; }
    const std::vector<double>& GetPosition() const { return position; }
};

template <typename T>
class ArrayN { // n-dimensional array
    size_t dim;
    size_t dimLength;
    std::vector<T> data;
public:
    friend class VectorN;
    ArrayN(size_t _dim, size_t _dimLength) : dim(_dim), dimLength(_dimLength), data((size_t)std::pow(_dimLength, _dim)) {  }
    ArrayN(size_t _dim, size_t _dimLength, const T& InitElement) : dim(_dim), dimLength(_dimLength), data((size_t)std::pow(_dimLength, _dim), InitElement) {  }

    T& At(const VectorN& pos) { // You can impletement operator[] instead
        if (pos.GetDim() != dim) throw 3;
        return data[pos.ToInt(dimLength)];
    }
};
