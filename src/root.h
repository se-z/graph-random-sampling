#ifndef DCDM_H
#define DCDM_H

#include <cassert>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h> // CLI用ヘッダ
#include <vector>

#include "graph.h"
#include "randomSampling.h"
#include "reader.h"
using namespace GRAPH;
using std::ifstream;
using std::vector;

namespace INIT_SPACE {

class ROOT {

  private:
    string OutDirRootPath;
    vector<Graph> ALL_TRANS;
    vector<Graph> RANDOM_GRAPH;
    vector<DFSCode> RANDOM_GRAPH_CODE;
    READ::myReader myreader;
    vector<int> make_rand_array_shuffle(const size_t, int, int, double);
    void outResult(double);
    FULL_TRAVERSE::gRandomTraverser traverser;

  public:
    int ALL_DATA_SIZE;
    double working_set_traverse_space_size;
    ROOT(const char *, std::string, int, int, int, double);
    void sampling();
};

}; // namespace INIT_SPACE

#endif // DCDM_H
