# kd-tree

An implementation of a [kd-tree](https://en.wikipedia.org/wiki/K-d_tree).

The first implementation used a permutation operator. It was under the assumption that we can move from one sorted
dimension to the next sorted dimension by a simple permutation. This is indeed the case if all points are considered.

However, if we pick a subset of the first dimension, there is no straightforward permutator object to be derived. In
contrast, we need to iterate over the indices of the items already sorted according to the next dimension. By then
explicitly comparing with the median of the current dimension we can in one sweep do the partition. This means we
need to propgate the complete set (for all dimensions) of sorted indices from one node to its children.

# Example

An example of a kd-tree:

![kd-tree example](output/points.png?raw=true "Example of a kd-tree")

# Remarks

The optimization with respect to not having to do ordering for each child node is my own. For example, in the
pseudocode on Wikipedia (the Python example), the point list is adjusted and sorted on every recursive function call.

It might very well be (I didn't look in detail into it) that it's the same optimization as proposed in the following
paper:

* Brown RA (2015). "Building a balanced k-d tree in O(kn log n) time". Journal of Computer Graphics Techniques. 4 (1): 50–68.

# To do

This only constructs a kd-tree. To use it for nearest neighbour etc., it will also need to be traversed.

# Copyright

Feel free to cite me or inform me of what you've done with this code. :-)

* Author: Anne van Rossum
* License: LGPLv3+, Apache License 2.0, and/or MIT (triple-licensed)
* Copyrights: Anne van Rossum
* Creation date: September 5th., 2019
