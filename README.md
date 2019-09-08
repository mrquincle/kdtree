# kd-tree

An implementation of a [kd-tree](https://en.wikipedia.org/wiki/K-d_tree).

It uses a permutator object which defines a permutation.  The permutator objects can be multiplied which correspond
to sequentially executed multiple permutations.

Setting up a kd-tree means that we sort over dimensions independently. It would be a pity to have to run a sort
operations multiple times. To sort once for each dimension should be sufficient.

Regarding implementation, what this means is that a particular subset of sorted values in dimension 0 should be
mapped to (a different) sorted series of values in dimension 1. There is a unique map that maps from sorted values
in dimension 0 to sorted values in dimension 1.

Next, the implementation has to allow for only a subset of values to be part of this operation. This is done in 
the multiplication operator by calculating the results like:

    result->push_back(other[indices[i]]);

Rather then

    (*result)[i] = other[indices[i]];

Moreover, to remove the need to copy the required indices to a temporary object, there is an additional operator which
limits the indices over which the multiplication operator will be performed:

    void restrict(int lower, int upper);

This will restrict the multiplication operator:

    if (indices[i] >= other.lower_limit() && indices[i] < other.upper_limit()) {
      result->push_back(other[indices[i]]);
	}

Rather then

    if (indices[i] < other.size()) {
      result->push_back(other[indices[i]]);
	}

If the permutator object is used for other purposes, it will run faster if these optimizations are removed.

# Remarks

The optimization with respect to not having to do ordering for each child node is my own. For example, in the
pseudocode on Wikipedia (the Python example), the point list is adjusted and sorted on every recursive function call.

# To do

This only construct a kd-tree. To use it for nearest neighbour etc., it will also need to be traversed.

# Copyright

Feel free to cite me or inform me of what you've done with this code. :-)

* Author: Anne van Rossum
* License: LGPLv3+, Apache License 2.0, and/or MIT (triple-licensed)
* Copyrights: Anne van Rossum
* Creation date: September 5th., 2019
