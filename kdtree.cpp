#include <kdtree.h>
#include <quicksort.hpp>
#include <math.h>
#include <iostream>

kdtree::kdtree(): output_mode(OutputMode::Tree), verbosity(LogLevel::DEBUG) {
    N = 0;
    k = 0;
}

kdtree::~kdtree() {
    for (auto iter: k_indices) {
	delete iter;
    }
    k_indices.clear();
}

void kdtree::build(items_t & items) {

    this->items = &items;

    N = items.size();
    if (N == 0) return;

    k = items[0]->size();

    // build for each dimension k a sorted array
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
    
    if (verbosity > LogLevel::NOTICE) {
	std::cout << "Sorted indices" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
	    std::cout << *k_indices[ik] << std::endl;
	}
	std::cout << std::endl;
    }
    
    if (verbosity > LogLevel::NOTICE) {
	std::cout << "Sorted indices, inverted" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
//	    indices_t *k_identity= *k_indices[ik] | *k_indices[ik];
	    indices_t *k_inv = !(*k_indices[ik]);
	    std::cout << *k_inv << std::endl;
	}
	std::cout << std::endl;
    }

    // prepare maps to quickly go from one sorted index to the next without having to sort again
    // A'B = B | A (that is invert A and then apply B is the same as the B | A operation)
    for (int ik = 0; ik < k; ++ik) {
	int next_k = (ik + 1) % k;
	indices_t *k_inverted = !(*k_indices[ik]);
	indices_t *map_indices = *k_indices[next_k] * *k_inverted;
	delete k_inverted;
	k_maps.push_back(map_indices);
    }
   
    if (verbosity > LogLevel::NOTICE) {
	std::cout << "Map contents" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
	    std::cout << *k_maps[ik] << std::endl;
	}
	std::cout << std::endl;
    }


    indices_t *incr = new indices_t(N);
    indices_t *mincr = (*k_maps[0] * *incr);
    if (verbosity > LogLevel::NOTICE) {
	std::cout << "On identity vector" << std::endl;
	for (int in = 0; in < N; ++in) {
	    std::cout << (*incr)[in] << ' ';
	}
	std::cout << std::endl;
	for (int in = 0; in < N; ++in) {
	    std::cout << (*mincr)[in] << ' ';
	}
	std::cout << std::endl;
    }
    std::cout << std::endl;
    
    if (verbosity > LogLevel::INFO) {
	std::cout << "Check if sort works: yes" << std::endl;

	for (int ik = 0; ik < k; ++ik) {
	    indices_t & indices = *k_indices[ik];
	    for (int in = 0; in < N; ++in) {
		int index = (indices)[in];
		item_t & item = *items[index];
		std::cout << index << ':';
		char sep = '[';
		for (int id = 0; id < item.size(); ++id, sep = ',') {
		    std::cout << sep << item[id];
		}
		std::cout << "] ";
	    }
	    std::cout << std::endl;
	    std::cout << std::endl;
	}

	std::cout << "Check if maps work: yes" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
	    std::cout << "Map sorted indices [" << ik << "] via map [" << ik << "] to sorted indices [" << (ik + 1) % k << "]" << std::endl;
	    indices_t *map = k_maps[ik];
	    // multiply indices of dimension k with maps of dimension k (which maps to next dim)
	    indices_t & indices = *(*k_maps[ik] * *k_indices[ik]);
	    for (int in = 0; in < indices.size(); ++in) {
		std::cout << indices[in] << ' ';
	    }
	    std::cout << std::endl;
	    
	    for (int in = 0; in < indices.size(); ++in) {
		int index = (indices)[in];
		item_t & item = *items[index];
		std::cout << index << ':';
		char sep = '[';
		for (int id = 0; id < item.size(); ++id, sep = ',') {
		    std::cout << sep << item[id];
		}
		std::cout << "] ";
	    }
	    std::cout << std::endl;
	    std::cout << std::endl;
	}
    
//	exit(EXIT_SUCCESS);


	std::cout << "Check if subset range of maps work: yes, sort of" << std::endl;
	for (int ik = 0; ik < k; ++ik) {
	    std::cout << "Map subset of sorted indices [" << ik << "] via map [" << ik << "] to sorted indices [" << (ik + 1) % k << "]" << std::endl;
	    indices_t *subset = k_indices[ik];
	    subset->restrict(0, N/2);
	    // multiply indices of dimension k with maps of dimension k (which maps to next dim)
	    indices_t & children = *(*k_maps[ik] * *subset);
	    subset->release();
	    for (int in = 0; in < children.upper_limit(); ++in) {
		std::cout << children[in] << ' ';
	    }
	    std::cout << std::endl;
	    for (int in = 0; in < children.size(); ++in) {
		int index = (children)[in];
		item_t & item = *items[index];
		std::cout << index << ':';
		char sep = '[';
		for (int id = 0; id < item.size(); ++id, sep = ',') {
		    std::cout << sep << item[id];
		}
		std::cout << "] ";
	    }
	    std::cout << std::endl;
	    std::cout << std::endl;
	}
	
	std::cout << "Why sort of works? This only picks a subset of indices from 0 and independently of 1 and then comes up with the order in the other dimension" << std::endl;
	std::cout << "It does not sort a particular set of indices." << std::endl;
	std::cout << std::endl;
	
	std::cout << "Check if subsets of maps work: no " << std::endl;

	// indices[0] map[0] -> indices[1] (sorted according to)
	// indices[1] map[1] -> indices[0] (sorted according to)

//	indices_t subset {4, 8, 14, 1, 7, 9};
	// leave out 14
	// let us consider only a subset of the data points, then we have to fabricate a new map
	indices_t subset {4, 8, 1, 7, 9};
	for (int in = 0; in < subset.upper_limit(); ++in) {
	    std::cout << subset[in] << ' ';
	}
   
	/*
	for (int ik = 0; ik < k; ++ik) {
	    int next_k = (ik + 1) % k;
	    indices_t *k_inverted = !(*k_indices[ik]);
	    indices_t *map_indices = *k_indices[next_k] * *k_inverted;
	    delete k_inverted;
	    k_maps.push_back(map_indices);
	}*/

	//indices_t *map_subset = 

	std::cout << std::endl;
	for (int in = 0; in < subset.size(); ++in) {
	    int index = (subset)[in];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	std::cout << std::endl;

	indices_t & children = *(*k_maps[0] * subset);
	for (int in = 0; in < children.upper_limit(); ++in) {
	    std::cout << children[in] << ' ';
	}
	std::cout << std::endl;
	for (int in = 0; in < children.size(); ++in) {
	    int index = (children)[in];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	std::cout << "Check if subsets work when using with data indices" << std::endl;
	
	indices_t & sorted_indices = *k_indices[0];
	indices_t subset1 {0, 1, 3}; //, 4, 5};



	std::cout << "Subset: ";
	for (int in = 0; in < subset1.upper_limit(); ++in) {
	    std::cout << subset1[in] << ' ';
	}
	std::cout << std::endl;
	std::cout << "Sorted indices ";
	indices_t *sind1 = subset1 * sorted_indices;
	for (int in = 0; in < subset1.upper_limit(); ++in) {
	    std::cout << (*sind1)[in] << ' ';
//	    std::cout << sorted_indices[subset1[in]] << ' ';
	}
	std::cout << std::endl;
	for (int in = 0; in < subset1.size(); ++in) {
	    int index = sorted_indices[subset1[in]];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	std::cout << std::endl;

	std::cout << "Original indices ";
	indices_t *oind1 = (*sind1 * (*!sorted_indices));
	for (int in = 0; in < oind1->upper_limit(); ++in) {
	    std::cout << (*oind1)[in] << ' ';
//	    std::cout << sorted_indices[subset1[in]] << ' ';
	}
	std::cout << std::endl;
    

	// first map 4, 8, 1 to 7, 5, 6

	indices_t *map1= (subset1 * *k_indices[1]);
//	indices_t *map1 = (subset1 * *k_maps[1]);
//	indices_t *map1 = (*k_maps[1] * subset1);
	std::cout << "Equivalent indices in dim 2: ";
	for (int in = 0; in < map1->upper_limit(); ++in) {
	    std::cout << (*map1)[in] << ' ';
	}
	std::cout << std::endl;

	indices_t *map2 = (*k_maps[0] * *map1);
	std::cout << "Equivalent indices in dim 2: ";
	for (int in = 0; in < map2->upper_limit(); ++in) {
	    std::cout << (*map2)[in] << ' ';
	}
	std::cout << std::endl;


//	indices_t *map1= (*k_maps[1] * subset1);
	indices_t &children1 = *(*map2 * *(!*k_indices[1]));
	//indices_t &children1 = *(*map1 | *k_indices[1]);
//	indices_t &children1 = *(*map1* subset1);
//	indices_t & children1 = *(*k_maps[0] * subset1);
//	sorted_indices = *k_indices[1];
	std::cout << "Inverse: ";
	for (int in = 0; in < children1.upper_limit(); ++in) {
	    std::cout << children1[in] << ' ';
	    //std::cout << sorted_indices[children1[in]] << ' ';
	}
	std::cout << std::endl;
	for (int in = 0; in < children1.size(); ++in) {
	    int index = sorted_indices[children1[in]];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	/*
	std::cout << "Check subset of subset" << std::endl;
	indices_t *indices = k_indices[0];
	indices->restrict(0, N/2);
	indices_t *children = (*k_maps[0] * *indices);
	for (int in = 0; in < children->upper_limit(); ++in) {
	    std::cout << (*children)[in] << ' ';
	}
	indices->release();
	std::cout << " (which is sorted on the second coordinate)" << std::endl;
	for (int in = 0; in < children->size(); ++in) {
	    int index = (*children)[in];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
//	children->restrict(0, 3);
//	indices_t *grandchildren = (*children * *k_maps[1]);
	indices_t *grandchildren = (*k_maps[1] * *children); // * *k_maps[1] ));
//	children->release();
	std::cout << "Sorted on first coordinate" << std::endl;
	for (int in = 0; in < grandchildren->size(); ++in) {
	    int index = (*grandchildren)[in];
	    item_t & item = *items[index];
	    std::cout << index << ':';
	    char sep = '[';
	    for (int id = 0; id < item.size(); ++id, sep = ',') {
		std::cout << sep << item[id];
	    }
	    std::cout << "] ";
	}
	*/
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	/*
	std::cout << "Check reuse of indices" << std::endl;
	// does not work like this, k_indices[1] is not a permutation of k_indices[0] 
	indices_t *indices = k_indices[0];
	for (int ikk = 0; ikk < 2*k; ++ikk) {
	    int ik = ikk % k;
	    std::cout << "Map sorted indices [" << ik << "] via map [" << ik << "] to sorted indices [" << (ik + 1) % k << "]" << std::endl;
	    indices_t *map = k_maps[ik];
	    indices->restrict(0, N/2);
	    indices_t *children = (*k_maps[ik] * *indices);
	    //indices = (*k_maps[ik] * *indices);
	    indices->release();
	    for (int in = 0; in < children->size(); ++in) {
		std::cout << (*children)[in] << ' ';
	    }
	    std::cout << std::endl;
	    // multiply children of dimension k with maps of dimension k (which maps to next dim)
	    for (int in = 0; in < children->size(); ++in) {
		int index = (*children)[in];
		item_t & item = *items[index];
		std::cout << index << ':';
		char sep = '[';
		for (int id = 0; id < item.size(); ++id, sep = ',') {
		    std::cout << sep << item[id];
		}
		std::cout << "] ";
	    }
	    std::cout << std::endl;
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
    }

    btree.min_value = 0.0;
    btree.max_value = 10.0;
    btree.parent = NULL;
    btree.is_left = true;
    btree.is_inverted = false;
    btree.level = 0;
    // set children to sorted indices across dimension 0
    btree.children = new indices_t(N);
 //   btree.children = k_indices[0];
    descend(0, &btree);
}
		
void kdtree::set_output_mode(OutputMode output_mode) {
    this->output_mode = output_mode;
}

void kdtree::write(std::ostream & os, const node_t<int> & node) const {
    switch(output_mode) {
    case OutputMode::Tree:
	write_tree(os, node);
	break;
    case OutputMode::TreeData:
	write_tree_data(os, node);
	break;
    case OutputMode::Lines:
	write_lines(os, node, 0);
	break;
    }
}

// Just write the tree
void kdtree::write_tree(std::ostream &os, const node_t<int> & node) const {
    // do not show nodes without children
    if (!node.left && !node.right) {
	return;
    }
    // print indices in the form [node: left right] (no node = -1)
    os << node.value << ": ";
    if (node.left != NULL) {
	os << node.left->value << ' ';
    } else {
	os << "-1 ";
    }
    if (node.right != NULL) {
	os << node.right->value;
    } else {
	os << "-1";
    }
    os << std::endl;
    // print children on next lines
    if (node.left != NULL) {
	write_tree(os, *node.left);
    }
    if (node.right != NULL) {
	write_tree(os, *node.right);
    }
}

// Write the tree and the data
void kdtree::write_tree_data(std::ostream &os, const node_t<int> & node) const {
    // print indices in the form [node left right]
    os << node.value << " ";
    if (node.left != NULL) {
	os << node.left->value << ' ';
    } else {
	os << "-1 ";
    }
    if (node.right != NULL) {
	os << node.right->value << ' ';
    } else {
	os << "-1 ";
    }
    // print values of the node
    if (items->size() < node.value) {
	std::cerr << "Wrong index used " << items->size() << " < " << node.value << std::endl;
    } else {
	item_t *item = (*items)[node.value];
	for (auto & it: *item) {
	    os << it << ' ';
	}
    }
    os << std::endl;
    // print children on next lines
    if (node.left != NULL) {
	write_tree_data(os, *node.left);
    }
    if (node.right != NULL) {
	write_tree_data(os, *node.right);
    }
}

// Write the kd-tree lines that chop up the space
void kdtree::write_lines(std::ostream &os, const node_t<int> & node, int dim) const {
    // print indices in the form [node left right]
    char sep = '\t';

    os << node.level << sep << node.value << sep;
    if (node.left != NULL) {
	os << node.left->value;
    } else {
	os << "-1";
    }
    os << sep;
    if (node.right != NULL) {
	os << node.right->value;
    } else {
	os << "-1";
    }
    os << sep;
    // print values of the node
    if (items->size() < node.value) {
	std::cerr << "Wrong index used " << items->size() << " < " << node.value << std::endl;
    } else {
	item_t *item = (*items)[node.value];
	for (auto & it: *item) {
	    os << it << sep;
	}
    }

    if (node.left || node.right) {
	// not yet k-d proof
	if (dim % k) {
	    os << node.min_value << sep << get_value(node.value,dim) << sep << 
		node.max_value << sep << get_value(node.value,dim);
	} else {
	    os << get_value(node.value,dim) << sep << node.min_value << sep << 
		get_value(node.value,dim) << sep << node.max_value;
	}
    } else {
	os << 0 << sep << 0 << sep << 0 << sep << 0;
    }
    os << std::endl;

    // print children on next lines
    int k_next = (dim + 1 ) % k;
    if (node.left != NULL) {
	write_lines(os, *node.left, k_next);
    }
    if (node.right != NULL) {
	write_lines(os, *node.right, k_next);
    }
}
		
const item_t & kdtree::get_item(int i) const {
    item_t *item = (*items)[i];
    return *item;
}

double kdtree::get_value(int i, int k) const {
    return get_item(i)[k];
}


// hard to get lines right, if descending the tree we get left, left, left, at our left side we have now
// the minimal value, at the right our k-parent
void kdtree::descend(int k_dim, node_t<int> *node) {

    // get all items attached to a node..., then divide it amongst <left> median <right>
    indices_t & data_indices = *node->children;
    indices_t & sorted_indices = *k_indices[k_dim];

    assert(data_indices.size());

    // get median element
    int median = floor((data_indices.size() - 1) / 2);

    // get data data_indices sorted according to map 
//    data_indices_t *new_data_indices = *k_maps[k_dim] * data_indices;

    assert(median < data_indices.size());
    //node->value = data_indices[median];
    node->value = sorted_indices[data_indices[median]];

    std::cout << std::endl;
    std::cout << "Median: " << node->value << std::endl;

    if (node->is_left) {
	node->min_value = 0.0;
    } else {
	node->max_value = 10.0;
    }


    int leftSize = median;
    int rightSize = data_indices.size() - leftSize - 1;

    if (leftSize > 0) {
	node->left = new node_t<int>();
	node->left->parent = node;
	node->left->level = node->level + 1;
	node->left->is_left = true;
	node->left->is_inverted = node->is_inverted;
//	node->left->min_value = node->min_value;
	node->left->max_value = get_value(node->value, k_dim);

	// data_indices are sorted according to dim k
	// we now want to sort them to dim (k + 1) % K
	// we do this by restricting the data_indices to the ones that belong to this particular child node
	// then we calculate their order by using k_maps
	
	int k_prev = (k_dim - 1 + k) % k;
	int k_next = (k_dim + 1 ) % k;

	data_indices.restrict(0, leftSize);
	std::cout << "Before: ";
	for (int ic = 0; ic < data_indices.upper_limit(); ++ic) {
	    std::cout << data_indices[ic] << ' ';
	}
	std::cout << std::endl;
	std::cout << "Before (sorted indices): ";
	for (int ic = 0; ic < data_indices.upper_limit(); ++ic) {
	    std::cout << sorted_indices[data_indices[ic]] << ' ';
	}
	std::cout << std::endl;
	indices_t *temp0 = *k_maps[k_dim] * data_indices;
	node->left->children = temp0;

//	std::cout << "Left {" << k_dim << "} " << node->value << " [" << leftSize << "#] -> ";
//	std::cout << std::endl;
	std::cout << "After:  ";
	for (int ic = 0; ic < leftSize; ++ic) {
	    std::cout << (*temp0)[ic] << ' ';
	}
	std::cout << std::endl;
	std::cout << "After (sorted indices):  ";
	for (int ic = 0; ic < leftSize; ++ic) {
	    std::cout << sorted_indices[(*temp0)[ic]] << ' ';
	}
	std::cout << std::endl;

//	indices_t *temp1 = *k_maps[k_dim] * *temp0;

	data_indices.release();

	descend(k_next, node->left);
    }

    /*
    if (rightSize > 0) {
	node->right = new node_t<int>();
	node->right->parent = node;
	node->right->level = node->level + 1;
	node->right->min_value = get_value(node->value, k_dim);
//	node->right->max_value = node->max_value;
	node->right->is_left = false;
	node->right->is_inverted = !node->is_inverted;
	
	int k_prev = (k_dim - 1 + k) % k;
	int k_next = (k_dim + 1 ) % k;

	// same as for left node
	data_indices.restrict(leftSize + 1, data_indices.size());
	node->right->children = *k_maps[k_dim] * data_indices;
	data_indices.release();
	
	std::cout << "Right: " << node->value << " [" << rightSize << "#] -> ";
	permutator &perm = *node->right->children;
	for (int ic = 0; ic < rightSize; ++ic) {
	    std::cout << perm[ic] << ' ';
	}
	std::cout << std::endl;
	
	descend(k_next, node->right);
    }
*/
}

std::ostream& operator<<(std::ostream& os, const kdtree & tree)
{
    tree.write(os, tree.btree);
    return os;
}
