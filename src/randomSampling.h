#ifndef FULLN_T_H
#define FULLN_T_H

#include "projected.h"
#include <random> //　メルセンヌ・ツイスターによる乱数生成に変えた方がよい

using PROJECT::DFSCode;
using PROJECT::EdgeList;
using PROJECT::get_backward;
using PROJECT::get_forward_pure;
using PROJECT::get_forward_rmpath;
using PROJECT::get_forward_root;
using PROJECT::gTraverse;
using PROJECT::History;
using PROJECT::PDFS;
using PROJECT::Projected;
using PROJECT::Projected_map2;
using PROJECT::Projected_map3;
using PROJECT::RMPath;

using std::cout;
using std::endl;
using std::string;

namespace FULL_TRAVERSE {

class gRandomTraverser : gTraverse {

  private:
    vector<Graph> *TRANSACTION;
    vector<Graph> *RANDOM_GRAPH;
    vector<DFSCode> *RANDOM_GRAPH_DFS_Code;
    void __traverse(int, Projected &);
    const int DATA_SIZE;
    const int MAX_SIZE;
    const int RANDOM_GRAPH_NUM;
    const int RANDOM_GRAPH_UNDER_SIZE;
    const double UNIFORM_PROBABILITY;

  public:
    void traverse();
    gRandomTraverser(int random_Graph_num, int random_graph_max_size,
                     int random_graph_under_size, int uniform_parameter,
                     vector<Graph> *random_space, vector<Graph> *random_graph,
                     vector<DFSCode> *random_graph_dfs_code);
};

} // namespace FULL_TRAVERSE

#endif