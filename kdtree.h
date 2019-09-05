#pragma once

#include <vector>

typedef std::vector<double> item_t;

typedef std::vector<item_t*> items_t;

typedef std::vector<int> indices_t;

typedef std::vector<indices_t*> k_indices_t;

/**
 * A kd-tree for 
 */
class kdtree {
	public:
		kdtree();

		~kdtree();

		void build(items_t items);

	private:
		int N;

		int k;

		k_indices_t k_indices;

}
