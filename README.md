# kd-tree

An implementation of a [kd-tree](https://en.wikipedia.org/wiki/K-d_tree).

The first implementation used a permutation operator. It was under the assumption that we can move from one sorted
dimension to the next sorted dimension by a simple permutation. This is indeed the case if all points are considered.

However, if we pick a subset of the first dimension, there is no straightforward permutator object to be derived. In
contrast, we need to iterate over the indices of the items already sorted according to the next dimension. By then
explicitly comparing with the median of the current dimension we can in one sweep do the partition. This means we
need to propgate the complete set (for all dimensions) of sorted indices from one node to its children.

# Example

An example of a kd-tree (constructed with seed `1569513768`):

![kd-tree example](output/points.png?raw=true "Example of a kd-tree")

# Remarks

The optimization with respect to not having to do ordering for each child node is my own. For example, in the
pseudocode on Wikipedia (the Python example), the point list is adjusted and sorted on every recursive function call.

It might very well be (I didn't look in detail into it) that it's the same optimization as proposed in the following
paper:

* Brown RA (2015). "Building a balanced k-d tree in O(kn log n) time". Journal of Computer Graphics Techniques. 4 (1): 50–68.

# Search

Currently search has also been implemented. Consider the points in the picture above. Online you will find quite a few
flawed search explanations. For example, here the circle represents the node of which we want the nearest neighbour, let
us call this `p`. The
root is node `1`. It separates the points through a vertical line at around 2.2. 

At this [quora post](https://www.quora.com/How-does-a-k-d-tree-find-the-K-nearest-neighbors) it is states that you
have to consider the left and right node from `1`, which are `2` and `4` respectively. The author state now to calculate
the difference between the `p` and `2` as well as between `p` and `4`. Then disregard the tree that is further away.
You see that `4` is further away and it would mean that we have to disregard `4` as well as `0`!

The right thing to do is to calculate the distance from `p` with (for example!) `4`. Then if we find that this 
distance is smaller then the distance between `p` and `1` we can disregard the "other" tree of `1` (where `4` does not
belong to, the tree `2`).  

# Uniqueness

Getting the median across a particular axis becomes ambiguous if there are duplicate values for a coordinate. For now
we use a simple N-order method where we increment a value with the next possible double.

# Rounding errors

We run 10.000 tests with around 50 nodes. We compare the results with an ordinary quicksort. The node with the closest
distance should be the same for the search through the kd-tree as for the ordinary quicksort. However, if the data is
discrete there can be multiple nodes at the same closest distance. If the nearest node is not unique, an assertion can
be raised. The rounding error with the distance is set to 0.000001.

# Copyright

Feel free to cite me or inform me of what you've done with this code. :-)

* Author: Anne van Rossum
* License: LGPLv3+, Apache License 2.0, and/or MIT (triple-licensed)
* Copyrights: Anne van Rossum
* Creation date: September 5th., 2019
