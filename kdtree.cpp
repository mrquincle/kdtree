#include <kdtree.h>
#include <quicksort.hpp>
#include <math.h>
#include <iostream>
#include <numeric>
#include <limits>
#include <iomanip>

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

int kdtree::build(items_t & p_items, bool unique) {
   
   N = p_items.size();
   if (N == 0) return EXIT_FAILURE;
   k = p_items[0]->size();

   if (unique) {
      items = &p_items;
      original_items = &p_items;
   } else {
      items = new items_t();
      original_items = &p_items;
      // make a deep copy, we can't just add some epsilon to the values (how small should it be?)
      for (int in = 0; in < N; ++in) {
	 item_t *item = new item_t(k);
	 for (int ik = 0; ik < k; ++ik) {
	    (*item)[ik] = (*p_items[in])[ik]; 
	 }
	 this->items->push_back(item);
      }
   }	

   // build for each dimension k a sorted array
   for (int ik = 0; ik < k; ++ik) {
      indices_t *indices = new indices_t(N);
      k_indices.push_back(indices);

      for (int in = 0; in < N; ++in) {
	 (*indices)[in] = in;
      }

      items1dim_t *values = new items1dim_t(N);
      for (int in = 0; in < N; ++in) {
	 (*values)[in] = (*(*items)[in])[ik];
      }

      quicksort(values->begin(), values->end(), indices->begin(), indices->end());

      delete values;
   }
   
   if (!unique) {
      //std::cout << "Make unique" << std::endl;
      for (int ik = 0; ik < k; ++ik) {
	 indices_t &indices = *k_indices[ik];
	 double prev_value = (*(*items)[indices[0]])[ik];
	 double value;
	 for (int in = 1; in < N; ++in) {
	    value = (*(*items)[indices[in]])[ik];
	    if (value == prev_value) {
	       double next_value = std::nextafter(value, std::numeric_limits<double>::max());
	       //std::cout << "Change value from " << value << " to " << next_value << std::endl;
	       (*(*items)[indices[in]])[ik] = next_value;
	       assert(value != next_value);
	    }
	    prev_value = value;
	 }
	 //std::cout << std::endl;
      }
      //std::cout << std::endl;
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
	 indices_t *k_inv = !(*k_indices[ik]);
	 std::cout << *k_inv << std::endl;
      }
      std::cout << std::endl;
   }

   btree.parent = NULL;
   btree.level = 0;
   btree.sibling = NULL; 
   btree.max_values = new std::vector<double>(k);
   for (int ik = 0; ik < k; ++ik) {
      (*btree.max_values)[ik] = 10;
   }
   btree.min_values = new std::vector<double>(k);
   for (int ik = 0; ik < k; ++ik) {
      (*btree.min_values)[ik] = 0;
   }
   btree.children = &k_indices;
   return build_descend(0, &btree);

//   reset(&btree);
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
   case OutputMode::Boxes:
      write_boxes(os, node, 0);
      break;
   }
}

void kdtree::reset(node_t<int> * node) {
   /*
   node->visited = false;
   if (node->left != NULL) {
      reset(node->left);
   }
   if (node->right != NULL) {
      reset(node->right);
   }*/
}

// Just write the tree
void kdtree::write_tree(std::ostream &os, const node_t<int> & node) const {
   // do not show nodes without children
   if (!node.left && !node.right) {
      return;
   }
   char sep = '\t';

   // print indices in the form [node: left right] (no node = -1)
   os << node.index << sep;
   os << (node.left ? node.left->index : -1) << sep;
   os << (node.right ? node.right->index : -1) << sep;
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
   char sep = '\t';
   os << node.index << sep;
   os << (node.left ? node.left->index : -1) << sep;
   os << (node.right ? node.right->index : -1) << sep;
   
   // print values of the node
   if (original_items->size() < node.index) {
      std::cerr << "Wrong index used " << original_items->size() << " < " << node.index << std::endl;
   } else {
      item_t *item = (*original_items)[node.index];
      for (auto & it: *item) {
	 os << it << sep;
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

   os << node.level << sep << node.index << sep;
   os << (node.left ? node.left->index : -1) << sep;
   os << (node.right ? node.right->index : -1) << sep;
   
   // print values of the node
   if (original_items->size() < node.index) {
      std::cerr << "Wrong index used " << original_items->size() << " < " << node.index << std::endl;
   } else {
      item_t *item = (*original_items)[node.index];
      for (auto & it: *item) {
	 os << it << sep;
      }
   }

   double coordinates[k * 2];
   int ic = 0;
   for (int ik = 0; ik < k; ++ik) {
      coordinates[ic++] = (*node.min_values)[ik];
   }
   for (int ik = 0; ik < k; ++ik) {
      coordinates[ic++] = (*node.max_values)[ik];
   }
   coordinates[dim] = get_value(node.index, dim);
   coordinates[dim+k] = get_value(node.index, dim);

   for (int ic = 0; ic < k * 2; ++ic) {
      os << coordinates[ic] << sep;
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

void kdtree::write_boxes(std::ostream &os, const node_t<int> & node, int dim) const {
   // print indices in the form [node left right]
   char sep = '\t';

   os << node.level << sep << node.index << sep;
   os << (node.left ? node.left->index : -1) << sep;
   os << (node.right ? node.right->index : -1) << sep;
   
   // print values of the node
   if (original_items->size() < node.index) {
      std::cerr << "Wrong index used " << original_items->size() << " < " << node.index << std::endl;
   } else {
      item_t *item = (*original_items)[node.index];
      for (auto & it: *item) {
	 os << it << sep;
      }
   }

   double coordinates[k * 2];
   int ic = 0;
   for (int ik = 0; ik < k; ++ik) {
      coordinates[ic++] = (*node.min_values)[ik];
   }
   for (int ik = 0; ik < k; ++ik) {
      coordinates[ic++] = (*node.max_values)[ik];
   }
   for (int ic = 0; ic < k * 2; ++ic) {
      os << coordinates[ic] << sep;
   }
   os << std::endl;

   // print children on next boxes
   int k_next = (dim + 1 ) % k;
   if (node.left != NULL) {
      write_boxes(os, *node.left, k_next);
   }
   if (node.right != NULL) {
      write_boxes(os, *node.right, k_next);
   }
}

const item_t & kdtree::get_item(int i) const {
   item_t *item = (*items)[i];
   return *item;
}

double kdtree::get_value(int i, int k) const {
   return get_item(i)[k];
}


int kdtree::build_descend(int k_dim, node_t<int> *node) {

   assert(node != NULL);
   assert(node->children != NULL);

   // get all items sorted across all dimensions from the node's children
   k_indices_t & k_data_indices = *node->children;
  
#ifdef DEBUGGING 
   std::cout << "Children" << std::endl;
   for (int ik = 0; ik < k; ++ik) {
      indices_t & data_index = *(k_data_indices)[ik];
      for (int i = 0; i < data_index.size(); ++i) {
	 std::cout << data_index[i] << ' ';
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
   
   std::cout << "Children values, sorted on dimension " << k_dim << std::endl;
   for (int ik = 0; ik < k; ++ik) {
      indices_t & data_index = *(k_data_indices)[ik];
      for (int i = 0; i < data_index.size(); ++i) {
	 std::cout << (*(*items)[data_index[i]])[k_dim] << ' ';
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
#endif

   // get main sorting dimension
   indices_t & data_indices = *k_data_indices[k_dim];

   // divide it amongst <left> median <right>
   int median = floor((data_indices.size() - 1) / 2);

   assert(median < data_indices.size());
   node->index = data_indices[median];

   int leftSize = median;
   int rightSize = data_indices.size() - leftSize - 1;

   // partitioning across "previous" dimension involves a comparison over all elements, order(N)
   item_t & median_data = *(*items)[data_indices[median]];
   
   // all the time compare with median in dimension k_dim
   double median_value = median_data[k_dim];

#ifdef DEBUGGING
   std::cout << "Median: " << node->index << std::endl;
   std::cout << "Median value here: " << median_value << std::endl;
   std::cout << "Left size: " << leftSize << std::endl;
   std::cout << "Right size: " << rightSize << std::endl;
#endif
   if (leftSize > 0) {
      node->left = new node_t<int>();
      node->left->parent = node;
      node->left->children = new k_indices_t();
      // generate for each dimension [k] indices
      for (int ik = 0; ik < k; ++ik) {
	 indices_t *temp = new indices_t(leftSize);
	 node->left->children->push_back(temp);
      }
      node->left->level = node->level+1;

      node->left->max_values = new std::vector<double>(k);
      for (int ik = 0; ik < k; ++ik) {
	 (*node->left->max_values)[ik] = (*node->max_values)[ik];
      }
      node->left->min_values = new std::vector<double>(k);
      for (int ik = 0; ik < k; ++ik) {
	 (*node->left->min_values)[ik] = (*node->min_values)[ik];
      }
      
      item_t & orig_median_data = *(*original_items)[data_indices[median]];
      double orig_median_value = orig_median_data[k_dim];
      (*node->left->max_values)[k_dim] = orig_median_value;
   }
   if (rightSize > 0) {
      node->right = new node_t<int>();
      node->right->parent = node;
      node->right->children = new k_indices_t();
      for (int ik = 0; ik < k; ++ik) {
	 indices_t *temp = new indices_t(rightSize);
	 node->right->children->push_back(temp);
      }
      node->right->level = node->level+1;
      
      node->right->max_values = new std::vector<double>(k);
      for (int ik = 0; ik < k; ++ik) {
	 (*node->right->max_values)[ik] = (*node->max_values)[ik];
      }
      node->right->min_values = new std::vector<double>(k);
      for (int ik = 0; ik < k; ++ik) {
	 (*node->right->min_values)[ik] = (*node->min_values)[ik];
      }
      
      item_t & orig_median_data = *(*original_items)[data_indices[median]];
      double orig_median_value = orig_median_data[k_dim];
      (*node->right->min_values)[k_dim] = orig_median_value;
   }
   
   if (leftSize > 0 && rightSize > 0) {
      node->right->sibling = node->left;
      node->left->sibling = node->right;
   }
  
   // iterate over data_indices, we use each time a differently sorted list (according to dim ik)
      //std::cout << "Sort the children" << std::endl;
   for (int ik = 0; ik < k; ++ik) {
      // compare with median at dimension i_k
      indices_t & sorted_indices = *(*node->children)[ik];
      for (int i = 0, il = 0, ir = 0; i < sorted_indices.size(); ++i) {

	 if (sorted_indices[i] == data_indices[median]) {
	    //std::cout << "Equal to median: " << sorted_indices[i] << " at position " << i << std::endl;
	    continue;
	 }

	 item_t &data_i  = *(*items)[sorted_indices[i]];
	 // std::cout << "Compare " << median_value << " with "  << data_i[ik] << std::endl;
	 if (data_i[k_dim] == median_value) {
	    std::cout << "Compare " << median_value << " with "  << data_i[ik] << std::endl;
	   // assert(false);
	    return EXIT_FAILURE;
	 }
	 else if (data_i[k_dim] < median_value) {
	    assert(leftSize > 0);
	    k_indices_t *leftChildren = node->left->children;
	    indices_t & left = (*(*leftChildren)[ik]);
	    //std::cout << "Add to left [#" << il << "]: " << sorted_indices[i] << std::endl;
	    left[il++] = sorted_indices[i];
	 } else {
	    assert(rightSize > 0);
	    k_indices_t *rightChildren = node->right->children;
	    indices_t & right = (*(*rightChildren)[ik]);
	    //std::cout << "Add to right [#" << ir << "]: " << sorted_indices[i] << std::endl;
	    right[ir++] = sorted_indices[i];
	 }
      }
      //std::cout << std::endl;
   }

#ifdef DEBUGGING 
   if (leftSize > 0) {
      std::cout << "Left children [#" << node->left->children->size() << "]: " << std::endl;
      k_indices_t *leftChildren = node->left->children;
      for (int ik = 0; ik < k; ++ik) {
	 indices_t & left = *(*leftChildren)[ik];
	 for (int i = 0; i < left.size(); ++i) {
	    std::cout << left[i] << ' ';
	 }
	 std::cout << std::endl;
      }
      std::cout << std::endl;
   }
  
   if (rightSize > 0) {
      std::cout << "Right children [#" << node->right->children->size() << "]: " << std::endl;
      k_indices_t *rightChildren = node->right->children;
      for (int ik = 0; ik < k; ++ik) {
	 indices_t & right = *(*rightChildren)[ik];
	 for (int i = 0; i < right.size(); ++i) {
	    std::cout << right[i] << ' ';
	 }
	 std::cout << std::endl;
      }
      std::cout << std::endl;
   }
#endif

   int k_next = (k_dim + 1) % k;
  
   bool exit_code = EXIT_SUCCESS;
   if (leftSize > 0) {
      exit_code = build_descend(k_next, node->left);
   }
   if (exit_code != EXIT_SUCCESS) {
      return exit_code;
   }
   if (rightSize > 0) {
      exit_code = build_descend(k_next, node->right);
   }

   return exit_code;
}
		
int kdtree::get_nearest_neighbour(item_t & data) {
   //clear();
   current_best_distance = distance(data, get_item(btree.index));
   current_best = btree.index;
   search_descend(0, &btree, data);
   return current_best;
}

template<typename T>
T euclidean(T x, T y) {
   return (x-y)*(x-y);
}

double kdtree::distance(const item_t & item0, const item_t & item1) const {
   double init = 0.0; 
   return std::sqrt(std::inner_product(item0.begin(), item0.end(), item1.begin(), init, std::plus<double>(), euclidean<double>));
}

void kdtree::search_descend(int k_dim, node_t<int> *node, item_t &data) {

   assert(node != NULL);

   //std::cout << "Visit node " << node->index << std::endl;
   
   node->distance = distance(data, get_item(node->index));
   if (node->distance < current_best_distance) {
      current_best_distance = node->distance;
      current_best = node->index;
   }

   int k_next = (k_dim + 1) % k;
   
   bool go_left = false;
   double dist_hyperplane = data[k_dim] - get_value(node->index, k_dim);
   if (dist_hyperplane < 0) {
      go_left = true;
   }
   //std::cout << "From " << node->index << " go " << (go_left ? "left" : "right") << std::endl;
   
   if (go_left) {
      if (node->left) {
	 //std::cout << "Descend left to node: " << node->left->index << std::endl;
	 search_descend(k_next, node->left, data);
      }
   } else {
      if (node->right) {
	 //std::cout << "Descend right to node: " << node->right->index << std::endl;
         search_descend(k_next, node->right, data);
      }
   }

   if (abs(dist_hyperplane) < current_best_distance) {
      bool other_left = !go_left;
      //std::cout << "From " << node->index << " go also " << (other_left ? "left" : "right") << std::endl;
      if (other_left) {
	 if (node->left) {
	    //std::cout << "Descend left to node: " << node->left->index << std::endl;
	    search_descend(k_next, node->left, data);
	 }
      } else {
	 if (node->right) {
	    //std::cout << "Descend right to node: " << node->right->index << std::endl;
	    search_descend(k_next, node->right, data);
	 }
      }

   }
}

std::ostream& operator<<(std::ostream& os, const kdtree & tree)
{
   tree.write(os, tree.btree);
   return os;
}
