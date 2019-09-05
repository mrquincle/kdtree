void kdtree::kdtree() {
	N = 0;
	k = 0;
}

void kdtree::~kdtree() {
}

void kdtree::build(items_t items) {

	N = items.size();
	if (N == 0) return;

	k = items[0]->size();

	// build for each dimension k a sorted array
	for (int ik = 0; ik < k; ++ik) {
		indices_t *indices = new indices_t();
		k_indices_t.push_back(indices);

		std::sort(
	}

	// iterate over k
	for (int ik = 0; ik < k; ++ik) {


	}
}
