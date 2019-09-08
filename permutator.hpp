#pragma once

#include <assert.h>
#include <vector>
#include <initializer_list>
#include <ostream>

/**
 * The permutator goes beyond an ordinary vector. It is meant to be an index into a vector or other container. It's
 * main purpose is to define a permutation. It can be used in sorting operations. 
 *
 * A multiplication of a permutator with another permutator leads to a single permutator that has the same result 
 * as the two permutations consecutively. Order is important.
 *
 * An inverse operator is available as well. The permutation with a permutator and its inverse leads to the original
 * values.
 */
class permutator {
        
    using size_type                 = std::size_t;

    private:
        typedef std::vector<int> indices_t;

        std::vector<int> indices;
  
        bool restricted;

        size_type _lower_limit;
        size_type _upper_limit;
    public:

        typedef indices_t::iterator iterator;

        typedef indices_t::const_iterator const_iterator;

        iterator begin() { 
            return indices.begin();
        }

        iterator end() {
            return indices.end();
        }

        size_type size() const {
            return indices.size();
        }

        int front() {
            return indices.front();
        }

        void resize(size_type s) {
            indices.resize(s);
        }

        size_type upper_limit() const {
            if (restricted) return _upper_limit;
            return indices.size();
        }
        
        size_type lower_limit() const {
            if (restricted) return _lower_limit;
            return 0;
        }

        void restrict(int lower, int upper) {
            restricted = true;
            _lower_limit = lower;
            _upper_limit = upper;
        }

        void release() {
            restricted = false;
        }

        permutator(int dim): restricted(false) {
            indices.resize(dim);
            for (int i = 0; i < dim; ++i) {
                indices[i] = i;
            }
        }
        
        permutator(std::initializer_list<int> init): indices(init), restricted(false) {
        }

        int& operator[](const int &i) {
            return indices[i];
        }
        
        const int& operator[](const int &i) const {
            return indices[i];
        }

        void push_back(const int &i) {
          indices.push_back(i);
        }

        /**
         * The multiplication operator is more or less the inverse of the "division" | operator. It is written such
         * that the "other" permutator does not need to be exactly the same size as this permutator. If the latter
         * would the case, it can also be written (without the if-statement) as:
         *
         *     (*result)[i] = other[indices[i]];
         * 
         * This makes it easier to see as the inverse of the division operator:
         *
         *     (*result)[indices[i]] = other[i];
         *
         * Returns a new allocated object. Caller is responsible for deallocation.
         */
        permutator * operator*(const permutator & other) {
            permutator *result = new permutator(0);
            for (int i = 0; i < size(); ++i) {
              if (indices[i] >= other.lower_limit() && indices[i] < other.upper_limit()) {
              //if (indices[i] < other.size()) {
                result->push_back(other[indices[i]]);
              }
            }
            return result;
        }
       
        permutator * operator|(const permutator & other) {
            permutator *result = new permutator(size());
            for (int i = 0; i < size(); ++i) {
                (*result)[indices[i]] = other[i];
            }
            return result;
        }
      
        // same as X / [0 1 2 3 ..]
        permutator * operator!() {
            permutator *result = new permutator(size());
            for (int i = 0; i < size(); ++i) {
                (*result)[indices[i]] = i;
            }
            return result;
        }

    friend std::ostream& operator<<(std::ostream& os, const permutator & perm) {
        for (int i = 0; i < perm.size(); ++i) {
            os << perm[i] << ' ';
        }
        return os;
    }

};

/*
std::ostream& operator<<(std::ostream& os, const permutator & perm)
{
    for (int i = 0; i < perm.size(); ++i) {
        os << perm[i] << ' ';
    }
    return os;
}*/
