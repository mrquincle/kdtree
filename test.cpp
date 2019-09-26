#include <quicksort.hpp>

#include <iostream>
#include <vector>
#include <kdtree.h>
#include <fstream>

#include <permutator.hpp>

int test_quicksort() {

    //double seed = 1231;
    double seed = time(NULL);
    srand(seed);

    std::vector<double> test;
    std::vector<double> test_copy;

    int N = 30;

    for (int i = 0; i < N; ++i) {
        double value = (rand() % 1000) / 100.;
        test.push_back(value);
        test_copy.push_back(value);
    }
    
    std::cout << "Test: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test[i] << " ";
    }
    std::cout << std::endl;

    std::vector<int> indices;
    for (int i = 0; i < N; ++i) {
        indices.push_back(i);
    }

    quicksort(test.begin(), test.end(), indices.begin(), indices.end());
    
    std::cout << "Indices: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << indices[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Sorted: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Sorted: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test_copy[indices[i]] << " ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

int test_kdtree() {
    std::cout << "---------" << std::endl;
    std::cout << "Next test" << std::endl;
    std::cout << "---------" << std::endl;

    items_t items;

    std::ofstream ofile;


    int number_of_data_items = 9;

    double d_seed = time(NULL);
    int seed = rand();
//    seed = 1188221626;
    std::cout << "Seed used " << seed << std::endl;
    std::cout << std::endl;
    ofile.open("kd_seed.txt", std::ios::out);
    ofile << seed << std::endl;;
    ofile.close();

    srand(seed);

    int k = 2;

#define RANDOM

#ifndef RANDOM
    item_t item0 { 7.96, 5.07 };
    item_t item1 { 7.57, 9.82 };
    item_t item2 { 4.20, 3.78 };
    item_t item3 { 6.14, 0.96 };
    item_t item4 { 2.58, 2.49 };
    item_t item5 { 8.36, 0.31 };
    item_t item6 { 0.19, 8.54 };
    item_t item7 { 8.39, 4.85 };
    item_t item8 { 2.19, 7.48 };
    item_t item9 { 3.65, 7.57 };
    item_t itemA { 2.86, 6.55 };
    item_t itemB { 2.49, 9.21 };
    item_t itemC { 9.52, 8.15 };

    items.push_back(&item0);
    items.push_back(&item1);
    items.push_back(&item2);
    items.push_back(&item3);
    items.push_back(&item4);
    items.push_back(&item5);
    items.push_back(&item6);
    items.push_back(&item7);
    items.push_back(&item8);
    items.push_back(&item9);
    items.push_back(&itemA);
    items.push_back(&itemB);
    items.push_back(&itemC);
    
    int N = items.size();
#else
    int N = number_of_data_items;
    for (int i = 0; i < N; ++i) {
        item_t *item = new item_t(k);
        for (int ik = 0; ik < k; ++ik) {
            (*item)[ik] = (rand() % 1000) / 100.;
  //          (*item)[ik] = (rand() % 10000) / 1000.;
        }
        items.push_back(item);
    }
#endif
    std::cout << "Number of items: " << N << std::endl;

    std::cout << "Data" << std::endl;
    for (int ik = 0; ik < k; ++ik) {
        for (int i = 0; i < N; ++i) {
            std::cout << (*items[i])[ik] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    kdtree tree;
    int exit_code = tree.build(items);
    if (exit_code != EXIT_SUCCESS) {
        std::cout << "Cannot be constructed (non unique values?)" << std::endl;
        assert(false);
        return exit_code;
    }

#ifdef VERBOSE
    std::cout << "The tree: " << std::endl;
    std::cout << tree << std::endl;

    tree.set_output_mode(OutputMode::TreeData);
    std::cout << "The tree and data: " << std::endl;
    std::cout << tree << std::endl;

    tree.set_output_mode(OutputMode::Lines);
    std::cout << "The tree, data, and line coordinates: " << std::endl;
    std::cout << tree << std::endl;
#endif

    tree.set_output_mode(OutputMode::Lines);
    ofile.open("kd_lines.txt", std::ios::out);
    ofile << tree;
    ofile.close();
    
    tree.set_output_mode(OutputMode::Boxes);
    std::cout << "The tree and boxes: " << std::endl;
    std::cout << tree << std::endl;

    item_t *search_item = new item_t(k);
    for (int ik = 0; ik < k; ++ik) {
        (*search_item)[ik] = (rand() % 1000) / 100.;
    }
     
    std::cout << "Search item" << std::endl;
    for (int ik = 0; ik < k; ++ik) {
        std::cout << (*search_item)[ik] << ' ';
    }
    std::cout << std::endl;
    std::cout << std::endl;

    int nn = tree.get_nearest_neighbour(*search_item);
    std::cout << "Nearest neighbour: " << nn << std::endl;

    ofile.open("kd_search.txt", std::ios::out);
    for (int ik = 0; ik < k; ++ik) {
        ofile << (*search_item)[ik] << '\t';
    }
    ofile << std::endl;
    ofile.close();

    std::cout << "Which has coordinates:" << std::endl;
    for (int ik = 0; ik < k; ++ik) {
        std::cout << (*items[nn])[ik] << "\t";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Distance: " << tree.distance(*search_item, *items[nn]) << std::endl;
    
    // brute-force
    std::vector<double> distances(N);
    for (int i = 0; i < N; ++i) {
        distances[i] =  tree.distance(*search_item, *items[i]);
    }
    indices_t d_indices(N);
    quicksort(distances.begin(), distances.end(), d_indices.begin(), d_indices.end());
    
    std::cout << std::endl;
    
    for (int i = 0; i < N; ++i) {
        std::cout << "Distance [" << d_indices[i] << "]\t"  << distances[i] << std::endl;
    }
    std::cout << std::endl;
    if (nn != d_indices[0]) {
        std::cout << "Warning, algorithm retuns the wrong index to be the one of the shortest distance!" << std::endl;
    }
    assert(nn == d_indices[0]);

    return EXIT_SUCCESS;
}

struct name_t
{
    std::string first;
    std::string last;
    std::string prefix;
};

std::ostream& operator<<(std::ostream& os, const name_t & name)
{
    os << name.first << ' ' << name.prefix << ' ' << name.last;
    return os;
}



int test_permutator() {

    std::vector<name_t> names;
    std::vector<std::string> first_names;
    std::vector<std::string> last_names;

    // Peet Tooren (4,4)
    // Bart Vliet (3,5)
    // Anne Rossum (1,3) 
    // Arend Jonge (2,1) 
    // Alex Mulder (0,2)
    // Teresa Dam (5,0)
    name_t n0 { "Peet", "Tooren", "van" };
    name_t n1 { "Bart", "Vliet", "van" };
    name_t n2 { "Anne", "Rossum", "van" };
    name_t n3 { "Arend", "Jonge", "de" };
    name_t n4 { "Alex", "Mulder", "de"  };
    name_t n5 { "Teresa", "Dam", "van" };
    names.push_back(n0);
    names.push_back(n1);
    names.push_back(n2);
    names.push_back(n3);
    names.push_back(n4);
    names.push_back(n5);

    std::cout << std::endl;
    std::cout << "Permutator objects" << std::endl;
    std::cout << "  - operator order: A|AB, first AB then A|AB" << std::endl;
    std::cout << "  - operator order: AA', first A' then AA'" << std::endl;
    std::cout << "  - operator order: A|A', first A' then A|A'" << std::endl;
    std::cout << std::endl;

    permutator a {4, 2, 3, 1, 0, 5};
    std::cout << "A           : ";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
   
    permutator b {5, 3, 4, 2, 0, 1};
    //permutator b {3, 2, 0, 1, 4, 5};
    std::cout << "B           : ";
    for (int i = 0; i < b.size(); ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "The first permutator sorts on first, the second on last names" << std::endl;
    std::cout << std::endl;

    std::cout << "a: ";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << names[a[i]] << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "b: ";
    for (int i = 0; i < b.size(); ++i) {
        std::cout << names[b[i]] << ", ";
    }
    std::cout << std::endl;

    
    std::cout << std::endl;
    std::cout << "Introduce multiplication" << std::endl;
    std::cout << "  - AB[0] = B[A[0]] = B[4] = 4, AB[1] = B[A[1]] = B[2] = 0, etc" << std::endl;
    std::cout << std::endl;

    permutator &ab = *(a * b);
    std::cout << "AB          : ";
    for (int i = 0; i < ab.size(); ++i) {
        std::cout << ab[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ba = *(b * a);
    std::cout << "BA          : ";
    for (int i = 0; i < ba.size(); ++i) {
        std::cout << ba[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Introduce \"division\" (it is not true division, hence the | symbol)" << std::endl;
    std::cout << std::endl;
    
    permutator &apipeb = *(b | a);
    std::cout << "A|B         : ";
    for (int i = 0; i < apipeb.size(); ++i) {
        std::cout << apipeb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &bpipea = *(a | b);
    std::cout << "B|A         : ";
    for (int i = 0; i < bpipea.size(); ++i) {
        std::cout << bpipea[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Inverses" << std::endl;
    std::cout << std::endl;

    permutator &ainv = *(!a);
    std::cout << "A'          : ";
    for (int i = 0; i < ainv.size(); ++i) {
        std::cout << ainv[i] << " ";
    }
    std::cout << std::endl;
   
    permutator &binv = *(!b);
    std::cout << "B'          : ";
    for (int i = 0; i < binv.size(); ++i) {
        std::cout << binv[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Products with inverses" << std::endl;
    std::cout << std::endl;
    
    permutator &a_binv = *(a * binv);
    std::cout << "A(B')       : ";
    for (int i = 0; i < a_binv.size(); ++i) {
        std::cout << a_binv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &b_ainv = *(b * ainv);
    std::cout << "B(A')       : ";
    for (int i = 0; i < b_ainv.size(); ++i) {
        std::cout << b_ainv[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Chain rules are remarkable A | AB becomes B, but AB | A becomes B'" << std::endl;
    std::cout << std::endl;

    // a | ab should result in b
    permutator &apipeab = *(a | ab);
    std::cout << "A|AB=B      : ";
    for (int i = 0; i < apipeab.size(); ++i) {
        std::cout << apipeab[i] << " ";
    }
    std::cout << std::endl;
    
    // ab | a is b inverse
    permutator &perm5 = *(ab | a);
    std::cout << "AB|A=B'     : ";
    for (int i = 0; i < perm5.size(); ++i) {
        std::cout << perm5[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainvb = *(ainv * b);
    std::cout << "A'B=B|A     : ";
    for (int i = 0; i < ainvb.size(); ++i) {
        std::cout << ainvb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &binva = *(binv * a);
    std::cout << "B'A=A|B     : ";
    for (int i = 0; i < binva.size(); ++i) {
        std::cout << binva[i] << " ";
    }
    std::cout << std::endl;
    
    permutator identity(6);
   
    std::cout << std::endl;
    std::cout << "Play around with identity (0, 1, 2, 3, 4, 5)" << std::endl;
    std::cout << std::endl;

    permutator &perminv1 = *(a * ainv);
    std::cout << "AA'=I       : ";
    for (int i = 0; i < perminv1.size(); ++i) {
        std::cout << perminv1[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainva = *(ainv * a);
    std::cout << "A'A=I       : ";
    for (int i = 0; i < ainva.size(); ++i) {
        std::cout << ainva[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &apipea = *(a | a);
    std::cout << "A|A=I       : ";
    for (int i = 0; i < apipea.size(); ++i) {
        std::cout << apipea[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ipipei = *(identity | identity);
    std::cout << "I=I|I       : ";
    for (int i = 0; i < ipipei.size(); ++i) {
        std::cout << ipipei[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    permutator &ia = *(identity * a);
    std::cout << "IA=A        : ";
    for (int i = 0; i < ia.size(); ++i) {
        std::cout << ia[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ai = *(a * identity);
    std::cout << "AI=A        : ";
    for (int i = 0; i < ai.size(); ++i) {
        std::cout << ai[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ipipea = *(identity | a);
    std::cout << "I|A=A       : ";
    for (int i = 0; i < ipipea.size(); ++i) {
        std::cout << ipipea[i] << " ";
    }
    
    std::cout << std::endl;
    permutator &ainvinv = *(!ainv);
    std::cout << "A''=A       : ";
    for (int i = 0; i < ainvinv.size(); ++i) {
        std::cout << ainvinv[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    permutator &apipei = *(a | identity);
    std::cout << "A'=A|I      : ";
    for (int i = 0; i < apipei.size(); ++i) {
        std::cout << apipei[i] << " ";
    }
    std::cout << std::endl;
    

    std::cout << std::endl;
    std::cout << "Quadratic form" << std::endl;
    std::cout << std::endl;
    
    permutator &aa = *(a * a);
    std::cout << "AA          : ";
    for (int i = 0; i < aa.size(); ++i) {
        std::cout << aa[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &bb = *(b * b);
    std::cout << "BB          : ";
    for (int i = 0; i < bb.size(); ++i) {
        std::cout << bb[i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << "Following same rules as above" << std::endl;
    std::cout << std::endl;

    permutator &aapipea = *(aa | a);
    std::cout << "AA|A=A'     : ";
    for (int i = 0; i < aapipea.size(); ++i) {
        std::cout << aapipea[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "The inverse can be kind of seen as a mirroring operator" << std::endl;
    std::cout << std::endl;

    permutator &apipeb_inv = *(!apipeb);
    std::cout << "(A|B)'=B|A  : ";
    for (int i = 0; i < apipeb_inv.size(); ++i) {
        std::cout << apipeb_inv[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "The inverse and pipe operator together" << std::endl;
    std::cout << std::endl;
    
    permutator &apipeainv = *(a | ainv);
    std::cout << "A|A'=(AA)'  : ";
    for (int i = 0; i < apipeainv.size(); ++i) {
        std::cout << apipeainv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainvpipea = *(ainv | a);
    std::cout << "A'|A=AA     : ";
    for (int i = 0; i < ainvpipea.size(); ++i) {
        std::cout << ainvpipea[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Interesting other identities " << std::endl;
    std::cout << std::endl;
    
    permutator &binvainv = *(binv * ainv);
    std::cout << "B'A'        : ";
    for (int i = 0; i < binvainv.size(); ++i) {
        std::cout << binvainv[i] << " ";
    }
    std::cout << std::endl;

    permutator &abinv = *(!ab);
    std::cout << "(AB)'       : ";
    for (int i = 0; i < abinv.size(); ++i) {
        std::cout << abinv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &bpipeainv = *(b | ainv);
    std::cout << "B|A'        : ";
    for (int i = 0; i < bpipeainv.size(); ++i) {
        std::cout << bpipeainv[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    permutator &ainvbinv = *(ainv * binv);
    std::cout << "A'B'        : ";
    for (int i = 0; i < ainvbinv.size(); ++i) {
        std::cout << ainvbinv[i] << " ";
    }
    std::cout << std::endl;
  
    permutator &bainv = *(!ba);
    std::cout << "(BA)'       : ";
    for (int i = 0; i < bainv.size(); ++i) {
        std::cout << bainv[i] << " ";
    }
    std::cout << std::endl;

    permutator &apipebinv = *(a | binv);
    std::cout << "A|B'        : ";
    for (int i = 0; i < apipebinv.size(); ++i) {
        std::cout << apipebinv[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    permutator &apipeba = *(a | ba);
    std::cout << "A|BA        : ";
    for (int i = 0; i < apipeba.size(); ++i) {
        std::cout << apipeba[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &bapipea = *(ba | a);
    
    permutator &bapipeainv = *(!bapipea);
    std::cout << "(BA|A)'     : ";
    for (int i = 0; i < bapipeainv.size(); ++i) {
        std::cout << bapipeainv[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    std::cout << "BA|A        : ";
    for (int i = 0; i < bapipea.size(); ++i) {
        std::cout << bapipea[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &apipebainv = *(!apipeba);
    std::cout << "(A|BA)'     : ";
    for (int i = 0; i < apipebainv.size(); ++i) {
        std::cout << apipebainv[i] << " ";
    }
    std::cout << std::endl;
   
#ifdef PLAYGROUND

    std::cout << std::endl;
    std::cout << "Not naturally simplified" << std::endl;
    std::cout << std::endl;
    
    permutator &abainv = *(ab * ainv);
    std::cout << "ABA'        : ";
    for (int i = 0; i < abainv.size(); ++i) {
        std::cout << abainv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainvba = *(ainvb * a);
    std::cout << "(A'B)A      : ";
    for (int i = 0; i < ainvba.size(); ++i) {
        std::cout << ainvba[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainvbainv = *(ainvb * ainv);
    std::cout << "(A'B)A'     : ";
    for (int i = 0; i < ainvbainv.size(); ++i) {
        std::cout << ainvbainv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &ainv_ba = *(ainv * ba);
    std::cout << "A'(BA)      : ";
    for (int i = 0; i < ainv_ba.size(); ++i) {
        std::cout << ainv_ba[i] << " ";
    }
    std::cout << std::endl;

    permutator &babinv = *(ba * binv);
    std::cout << "BAB'        : ";
    for (int i = 0; i < babinv.size(); ++i) {
        std::cout << babinv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &bainvbinv = *(bainv * binv);
    std::cout << "BA'B'       : ";
    for (int i = 0; i < bainvbinv.size(); ++i) {
        std::cout << bainvbinv[i] << " ";
    }
    std::cout << std::endl;

    permutator &binvab = *(binva * b);
    std::cout << "B'AB        : ";
    for (int i = 0; i < binvab.size(); ++i) {
        std::cout << binvab[i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    
    permutator &aab = *(aa * b);
    std::cout << "AA*B        : ";
    for (int i = 0; i < aab.size(); ++i) {
        std::cout << aab[i] << " ";
    }
    std::cout << std::endl;
   
    permutator &aapipeb = *(aa | b);
    std::cout << "AA|B        : ";
    for (int i = 0; i < aapipeb.size(); ++i) {
        std::cout << aapipeb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aabinv = *(aa * binv);
    std::cout << "AA*B'       : ";
    for (int i = 0; i < aabinv.size(); ++i) {
        std::cout << aabinv[i] << " ";
    }
    std::cout << std::endl;
   
    permutator &aapipebinv = *(aa | binv);
    std::cout << "AA|B'       : ";
    for (int i = 0; i < aapipebinv.size(); ++i) {
        std::cout << aapipebinv[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    permutator &aainv = *(!aa);
    std::cout << "(AA)'       : ";
    for (int i = 0; i < aainv.size(); ++i) {
        std::cout << aainv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aainvb = *(aainv * b);
    std::cout << "(AA)'B      : ";
    for (int i = 0; i < aainvb.size(); ++i) {
        std::cout << aainvb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aainvpipeb = *(aainv | b);
    std::cout << "(AA)'|B     : ";
    for (int i = 0; i < aainvpipeb.size(); ++i) {
        std::cout << aainvpipeb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aainvpipebinv = *(aainv | binv);
    std::cout << "(AA)'|B'    : ";
    for (int i = 0; i < aainvpipebinv.size(); ++i) {
        std::cout << aainvpipebinv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aapipebb = *(aa | bb);
    std::cout << "(AA)|(BB)   : ";
    for (int i = 0; i < aapipebb.size(); ++i) {
        std::cout << aapipebb[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &abpipeba = *(ab | ba);
    std::cout << "AB|BA       : ";
    for (int i = 0; i < abpipeba.size(); ++i) {
        std::cout << abpipeba[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &abba = *(ab * ba);
    std::cout << "AB*BA       : ";
    for (int i = 0; i < abba.size(); ++i) {
        std::cout << abba[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aapipeba = *(aa | ba);
    std::cout << "AA|BA       : ";
    for (int i = 0; i < aapipeba.size(); ++i) {
        std::cout << aapipeba[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aapipebainv = *(aa | bainv);
    std::cout << "AA|BA'      : ";
    for (int i = 0; i < aapipebainv.size(); ++i) {
        std::cout << aapipebainv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aapipeab = *(aa | ab);
    std::cout << "AA|AB=B|A   : ";
    for (int i = 0; i < aapipeab.size(); ++i) {
        std::cout << aapipeab[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &aapipeabinv = *(aa | abinv);
    std::cout << "AA|AB'      : ";
    for (int i = 0; i < aapipeabinv.size(); ++i) {
        std::cout << aapipeabinv[i] << " ";
    }
    std::cout << std::endl;
#endif

    std::cout << std::endl;
    std::cout << "Mapping from A to B" << std::endl;
    std::cout << std::endl;
    
    permutator &m = bpipea;
    std::cout << "M=B|A       : ";
    for (int i = 0; i < m.size(); ++i) {
        std::cout << m[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &am = *(a * m);
    std::cout << "AM=B        : ";
    for (int i = 0; i < am.size(); ++i) {
        std::cout << am[i] << " ";
    }
    std::cout << std::endl;
   
#ifdef PLAYGROUND
    permutator &mm = *(m * m);
    std::cout << "MM          : ";
    for (int i = 0; i < mm.size(); ++i) {
        std::cout << mm[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &minv = *(!m);
    std::cout << "M'          : ";
    for (int i = 0; i < minv.size(); ++i) {
        std::cout << minv[i] << " ";
    }
    std::cout << std::endl;
    
    permutator &mminv = *(m * minv);
    std::cout << "MM'=I       : ";
    for (int i = 0; i < mminv.size(); ++i) {
        std::cout << mminv[i] << " ";
    }
    std::cout << std::endl;
#endif
    
    std::cout << std::endl;
    std::cout << "Mapping from B to A" << std::endl;
    std::cout << std::endl;
    
    permutator n = b_ainv;
    std::cout << "N=B(A')     : ";
    for (int i = 0; i < n.size(); ++i) {
        std::cout << n[i] << " ";
    }
    std::cout << std::endl;

    permutator &na = *(n * a);
    std::cout << "NA=B        : ";
    for (int i = 0; i < na.size(); ++i) {
        std::cout << na[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "A shorter vector (a is short A, b is short B)" << std::endl;
    std::cout << std::endl;
   
    // a smaller vector
    permutator ashort(3);

    std::copy(a.begin(), a.begin() + 3, ashort.begin());
    
    std::cout << "a           : ";
    for (int i = 0; i < ashort.size(); ++i) {
        std::cout << ashort[i] << " ";
    }
    std::cout << std::endl;

    permutator &ashortm = *(ashort * m);
    std::cout << "aM=b        : ";
    for (int i = 0; i < ashortm.size(); ++i) {
        std::cout << ashortm[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Now we only want a subset of values as indicated in a, but sorted according to b" << std::endl;
    std::cout << std::endl;
   
    permutator &nashort = *(n * ashort);
    std::cout << "Na          : ";
    for (int i = 0; i < nashort.size(); ++i) {
        std::cout << nashort[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Sorted according to first name: " << std::endl << "  ";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << names[ a[i] ] << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Sorted according to last name (by using m as index): " << std::endl << "  ";
    for (int i = 0; i < m.size(); ++i) {
        std::cout << names[ m [a[i]] ] << ", ";
    }
    std::cout << std::endl;

    std::cout << "Sorted according to last name (by using m as multiplication, should be the same): " << std::endl << "  ";
    for (int i = 0; i < am.size(); ++i) {
        std::cout << names[ am[i] ] << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Pick first three of a and sort using last name (b): " << std::endl << "  ";
    for (int i = 0; i < nashort.size(); ++i) {
        std::cout << names[ nashort[i] ] << ", ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    
    int success;

//    success = test_quicksort();

//    success = test_permutator();

    int T = 10000;
//    T = 5;
    for (int t = 0; t < T; ++t) {
        success = test_kdtree();
    }

    return success;
}

