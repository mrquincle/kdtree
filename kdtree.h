#pragma once

#include <vector>
#include <permutator.hpp>
#include <ostream>

enum class OutputMode { Tree, TreeData, Lines, Boxes };

// Ordinary syslog levels
enum class LogLevel { EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG };

typedef std::vector<double> item_t;

typedef std::vector<item_t*> items_t;

typedef std::vector<double> items1dim_t;

typedef permutator indices_t;

typedef std::vector<indices_t*> k_indices_t;

template <typename T>
struct node_t {
	T index;
	node_t<T> *left;
	node_t<T> *right;
	node_t<T> *parent;
	int level;
	k_indices_t *children;

	node_t<T> *sibling;

	std::vector<double> *max_values;
	std::vector<double> *min_values;

	//bool visited;
	double distance;
};

/**
 * A kd-tree for point clouds. It can be used to quickly find nearest neighbours.
 */
class kdtree {
	public:
		// Constructor
		kdtree();

		// Destructor (TODO)
		~kdtree();

		// Construct the tree
		int build(items_t & items);
		
		// Reset 
		void reset(node_t<int> * node);

		// Get index of nearest neighbour
		int get_nearest_neighbour(item_t & data);

		// Define how the kd-tree is printed
		void set_output_mode(OutputMode output_mode);

		// Wrapper function for possible writes
		void write(std::ostream &os, const node_t<int> & node) const;
		
		// Distance between two items
		double distance(const item_t & item0, const item_t & item1) const;
	protected:
		// Recursive function to construct the tree
		int build_descend(int k_dim, node_t<int> *node);
		
		// Recursive function to search the tree
		void search_descend(int k_dim, node_t<int> *node, item_t &data);

		//void search_ascend(int k_dim, node_t<int> *node, item_t &data);
		
		// Just write the tree
		void write_tree(std::ostream &os, const node_t<int> & node) const;
		
		// Write the tree and the data
		void write_tree_data(std::ostream &os, const node_t<int> & node) const;

		// Write the kd-tree lines that chop up the space
		void write_lines(std::ostream &os, const node_t<int> & node, int dim) const;

		// Write data and box dimensions
		void write_boxes(std::ostream &os, const node_t<int> & node, int dim) const;

		// Get item i (in *items)
		const item_t & get_item(int i) const;

		// Get value k of an item i (in *items)
		double get_value(int i, int k) const;

	private:
		LogLevel verbosity;

		int N;

		int k;

		k_indices_t k_indices;

		node_t<int> btree;

		items_t *items;

		OutputMode output_mode;

		int current_best;
		double current_best_distance;

		friend std::ostream& operator<<(std::ostream& os, const kdtree & tree);
};
