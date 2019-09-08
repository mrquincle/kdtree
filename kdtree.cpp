#include <kdtree.h>
#include <quicksort.hpp>
#include <math.h>
#include <iostream>

kdtree::kdtree() {
	N = 0;
	k = 0;
}

kdtree::~kdtree() {
}

void kdtree::build(items_t & items) {

	N = items.size();
	if (N == 0) return;

	k = items[0]->size();

	// build for each dimension k a sorted array

	std::cout << "Sort items" << std::endl;

	for (int ik = 0; ik < k; ++ik) {

		indices_t *indices = new indices_t(N);
		k_indices.push_back(indices);

		for (int in = 0; in < N; ++in) {
			(*indices)[in] = in;
		}

		items1dim_t *values = new items1dim_t(N);
		for (int in = 0; in < N; ++in) {
			(*values)[in] = (*items[in])[ik];
		}

		quicksort(values->begin(), values->end(), indices->begin(), indices->end());

		delete values;
	}

	for (int ik = 0; ik < k; ++ik) {
		std::cout << *k_indices[ik] << std::endl;
	}
	std::cout << std::endl;

	// prepare maps to quickly go from one sorted index to the next without having to sort again
	// A'B = B | A (that is invert A and then apply B is the same as the B | A operation)
	for (int ik = 0; ik < k; ++ik) {
		int next_k = (ik + 1) % k;
		indices_t *k_inverted = !(*k_indices[ik]);
		indices_t *map_indices = *k_indices[next_k] * *k_inverted;
		delete k_inverted;
		k_maps.push_back(map_indices);
	}

	std::cout << "Map contents" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
		std::cout << *k_maps[ik] << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Construct tree" << std::endl;

	btree.children = k_indices[0];
	descend(0, &btree);

	print(&btree);

	for (auto iter: k_indices) {
		delete iter;
	}
	k_indices.clear();
}

void kdtree::print(node_t<int> *node) {
	if (!node->left && !node->right) {
		return;
	}
	std::cout << node->value << ": ";
	if (node->left != NULL) {
		std::cout << node->left->value << ' ';
	}
	if (node->right != NULL) {
		std::cout << node->right->value;
	}
	std::cout << std::endl;

	if (node->left != NULL) {
		print(node->left);
	}
	if (node->right != NULL) {
		print(node->right);
	}
}

void kdtree::descend(int k_dim, node_t<int> *node) {

	// get all items attached to a node..., then divide it amongst <left> median <right>
	indices_t & indices = *node->children;

	int median = floor(indices.size() / 2);
	node->value = indices[median];

	int leftSize = median;
	int rightSize = indices.size() - leftSize - 1;

	if (leftSize > 0) {
		node->left = new node_t<int>();

		// indices are sorted according to dim k
		// we now want to sort them to dim (k + 1) % K
		// we do this by restricting the indices to the ones that belong to this particular child node
		// then we calculate their order by using k_maps
		indices.restrict(0, leftSize);
		node->left->children = *k_maps[k_dim] * indices;
		//		indices.release();

		int k_next = (k_dim + 1 ) % k;
		descend(k_next, node->left);
	}

	if (rightSize > 0) {
		node->right = new node_t<int>();

		// same as for left node
		indices.restrict(leftSize + 1, indices.size());
		node->right->children = *k_maps[k_dim] * indices;
		//		indices.release();

		int k_next = (k_dim + 1 ) % k;
		descend(k_next, node->right);
	}

}
