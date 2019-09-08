#pragma once

#include <vector>
#include <permutator.hpp>

typedef std::vector<double> item_t;

typedef std::vector<item_t*> items_t;

typedef std::vector<double> items1dim_t;

typedef permutator indices_t;

typedef std::vector<indices_t*> k_indices_t;

template <typename T>
struct node_t {
	T value;
	indices_t *children;
	node_t<T> *left;
	node_t<T> *right;
};

/**
 * A kd-tree for 
 */
class kdtree {
	public:
		kdtree();

		~kdtree();

		void build(items_t & items);

	protected:
		void descend(int k_dim, node_t<int> *node);

		void print(node_t<int> *node);

	private:
		int N;

		int k;

		k_indices_t k_indices;

		k_indices_t k_maps;

		node_t<int> btree;
};
