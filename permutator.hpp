#pragma once

#include <assert.h>
#include <vector>
#include <initializer_list>

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

    private:
        typedef std::vector<int> indices_t;

        std::vector<int> indices;
    
    public:
        using size_type                 = std::size_t;

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

        permutator(int dim, bool wildcards = false) {
            indices.resize(dim);
            if (wildcards) {
              for (int i = 0; i < dim; ++i) {
                indices[i] = -1;
              }
            } else {
              for (int i = 0; i < dim; ++i) {
                indices[i] = i;
              }
            }
        }
        
        permutator(std::initializer_list<int> init): indices(init) {
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

        permutator * operator*(const permutator & other) {
            permutator *result = new permutator(0, false);
            for (int i = 0; i < size(); ++i) {
              // (*result)[i] = other[indices[i]];
              if (!(indices[i] >= other.size())) {
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
};
