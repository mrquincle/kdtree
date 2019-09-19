#pragma once

#include <vector>
#include <permutator.hpp>
#include <ostream>

enum class OutputMode { Tree, TreeData, Lines };

// Ordinary syslog levels
enum class LogLevel { EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG };

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
	node_t<T> *parent;
	double min_value;
	double max_value;
	int level;
	node_t<T> *k_parent;
	bool is_left;
	bool is_inverted;
};

/**
 * A kd-tree for point clouds. It can be used to quickly find nearest neighbours.
 */
class kdtree {
	public:
		kdtree();

		~kdtree();

		void build(items_t & items);

		// Define how the kd-tree is printed
		void set_output_mode(OutputMode output_mode);

	protected:
		void descend(int k_dim, node_t<int> *node);
		
		// Wrapper function for possible writes
		void write(std::ostream &os, const node_t<int> & node) const;

		// Just write the tree
		void write_tree(std::ostream &os, const node_t<int> & node) const;
		
		// Write the tree and the data
		void write_tree_data(std::ostream &os, const node_t<int> & node) const;

		// Write the kd-tree lines that chop up the space
		void write_lines(std::ostream &os, const node_t<int> & node, int dim) const;

		const item_t & get_item(int i) const;

		double get_value(int i, int k) const;
	private:
		LogLevel verbosity;

		int N;

		int k;

		k_indices_t k_indices;

		k_indices_t k_maps;

		node_t<int> btree;

		items_t *items;

		OutputMode output_mode;

		friend std::ostream& operator<<(std::ostream& os, const kdtree & tree);
};
