#ifndef VECARRAY_HPP_EPBF1RNR
#define VECARRAY_HPP_EPBF1RNR

#include <array>
#include <vector>
#include <cassert>
#include <initializer_list>

#include <type_traits>

namespace ndata {


template <long T> using IsMinusOne = std::is_same<std::integral_constant<long, T>, std::integral_constant<long, -1>>;
template <typename T> using Negate = std::integral_constant<bool, !T::value>;
template <long T> using IsNotMinusOne = Negate<IsMinusOne<T>>;
template <long T> using MinusOne = std::enable_if<IsMinusOne<T>::value, std::integral_constant<long, T>>;
template <long T> using NotMinusOne = std::enable_if<IsNotMinusOne<T>::value, std::integral_constant<long, T>>;

const long DYNAMIC_SIZE = -1;

//base template
template<class T, long static_size, class Enable=void>
struct vecarray {
    static_assert(static_size>=-1, "Wrong static size!");
};

//specialization static vecarray
template<class T, long static_size>
struct vecarray<T, static_size, typename std::enable_if<(static_size >= 0)>::type> {

    static constexpr long static_size_or_dynamic = static_size;

    std::array<T, static_size> stackStorage;

    //provided for compatibility with dynamic version
    vecarray(long dynamic_size) {
        assert(dynamic_size == -1);
    }

    vecarray(long dynamic_size, std::vector<T> init) {
        assert(dynamic_size == -1);
        assert(init.size() <= static_size);

        //init members with content of the vector
        //imitates the behaviour of initializer lists
        for (size_t i = 0; i < init.size(); ++i) {
            stackStorage[i] = init[i];
        }
    }

    vecarray(std::array<T, static_size> iniArr):
        stackStorage(iniArr) {
    }

    vecarray(std::vector<T> ini) {
        //static_assert(static_size == 0, "This constructor always produce a dynamic vecarray");
        assert(ini.size() <= static_size);
        for (size_t i = 0; i < ini.size(); ++i) {
            stackStorage[i]=ini[i];
        }
    }

    //overloaded to avoid ambiguity btw vector and array
    vecarray(std::initializer_list<T> iniArr)
    {
        assert(iniArr.size() == static_size);// "Size of initializer list doesn't match");
        //static_assert(iniArr.size() == static_size, "");// "Size of initializer list doesn't match");

        size_t i = 0;
        for (T val: iniArr) {
            stackStorage[i] = val;
            i++;
        }
    }

    //empty initializer for later assignment
    vecarray() {};

    size_t size() {
        return static_size;
    }

    /**
     * For compatibility with dynarray.
     */
    constexpr long dynsize () { return -1;}

    T& operator[](size_t index) {
        return stackStorage[index];
    }

    void fill(T val) {
        for (size_t i = 0; i < size(); ++i) {
            this->operator[](i) = val;
        }
    }

    vecarray<T, static_size>
    reverse () {
        vecarray<T, static_size> new_vecarray;

        for (size_t i = 0; i < static_size; ++i) {
            long rev_ind = static_size-1-i;
            assert(rev_ind > 0);
            new_vecarray[rev_ind] = stackStorage[i];
        }

        return new_vecarray;
    }


    /**
     * Like push_back but returns new vecarray instead of mutating 
     *
     * Static version
     *
     */
    vecarray<T, static_size+1>
    append (T val) {
        vecarray<T, static_size+1> new_vecarray;

        for (size_t i = 0; i < size(); ++i) {
            new_vecarray[i] = operator[](i);
        }

        new_vecarray[static_size] = val;

        return new_vecarray;
    }

};

/**
 * Specialization for a dynamic vecarray
 */
template<class T, long static_size>
struct vecarray<T, static_size, typename std::enable_if<(static_size == -1)>::type > {

    static constexpr long static_size_or_dynamic = DYNAMIC_SIZE;

    std::vector<T> heapStorage;

    vecarray(long dynamic_size) {
        heapStorage = std::vector<T>(dynamic_size);
        assert(dynamic_size>=0);
    }

    vecarray(long dynamic_size, std::vector<T> init) {

        heapStorage = std::vector<T>(dynamic_size);
        assert(init.size() <= dynamic_size);

        assert(dynamic_size>=0);

        //init members with content of the vector
        //imitates the behaviour of initializer lists
        for (size_t i = 0; i < init.size(); ++i) {
            heapStorage[i] = init[i];
        }
    }

    vecarray(std::vector<T> ini) {
        heapStorage=ini;
    }

    vecarray(std::array<T, static_size> iniArr): heapStorage(iniArr.begin(), iniArr.end()) { }

    //overloaded to avoid ambiguity btw vector and array
    vecarray(std::initializer_list<T> iniArr) {
        size_t i = 0;
        for (T val: iniArr) {
            heapStorage[i] = val;
            i++;
        }
    }

    //empty initializer for later assignment
    vecarray() {};

    size_t size() {
        return heapStorage.size();
    }

    long dynsize () { return heapStorage.size();}

    T& operator[](size_t index) {
        return heapStorage[index];
    }

    void fill(T val) {
        for (size_t i = 0; i < size(); ++i) {
            this[i] = val;
        }
    }

    vecarray<T, -1>
    reverse () {
        vecarray<T, 0> new_vecarray (size());
        size_t vecsize = new_vecarray.size();

        for (size_t i = 0; i < vecsize; ++i) {
            long rev_ind = vecsize-1-i;
            assert(rev_ind > 0);
            new_vecarray[rev_ind] = this[i];
        }

        return new_vecarray;
    }


    /**
     * Like push_back but returns new vecarray instead of mutating 
     *
     * Dynamic version
     */
    vecarray<T, -1>
    append (T val) {
        vecarray<T, -1> new_vecarray (size()+1);

        for (size_t i = 0; i < size(); ++i) {
            new_vecarray[i] = this[i];
        }

        new_vecarray[size()] = val;

        return new_vecarray;
    }


};

} //end namespace

#endif /* end of include guard: VECARRAY_HPP_EPBF1RNR */
