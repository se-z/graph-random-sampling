#include "randomSampling.h"

namespace FULL_TRAVERSE {

gRandomTraverser::gRandomTraverser(int random_Graph_num,
                                   int random_graph_max_size,
                                   int random_graph_under_size,
                                   int uniform_parameter,
                                   vector<Graph> *random_space,
                                   vector<Graph> *random_graph,
                                   vector<DFSCode> *random_graph_dfs_code)
    : TRANSACTION(random_space), RANDOM_GRAPH(random_graph),
      RANDOM_GRAPH_DFS_Code(random_graph_dfs_code),
      MAX_SIZE(random_graph_max_size),
      RANDOM_GRAPH_UNDER_SIZE(random_graph_under_size),
      DATA_SIZE((*TRANSACTION).size()), RANDOM_GRAPH_NUM(random_Graph_num),
      UNIFORM_PROBABILITY(uniform_parameter){

      };

void gRandomTraverser::traverse() {

    while((*RANDOM_GRAPH).size() < RANDOM_GRAPH_NUM) {

        EdgeList edges;
        Projected_map3 root;
        DFS_CODE.clear();

        for(int graph_id = 0; graph_id < DATA_SIZE; ++graph_id) {
            Graph &g = (*TRANSACTION)[graph_id];
            for(unsigned int from = 0; from < g.size(); ++from) {
                if(get_forward_root(g, g[from], edges)) {
                    for(auto &edg : edges) {
                        root[g[from].label][edg->elabel][g[edg->to].label].push(
                            graph_id, edg, 0);
                    }
                }
            }
        }

        int rootnum = 0;
        for(auto &[fromlabel, map2] : root) {
            for(auto &[elabel, map1] : map2) {
                for(auto &[tolabel, projected_obj] : map1) {
                    DFS_CODE.push(0, 1, fromlabel, elabel, tolabel);
                    int depth = 0;
                    __traverse(depth, projected_obj);
                    DFS_CODE.pop();
                }
            }
        }
    }

    cout << "----------------------------------------------------" << endl;
    cout << "[INFO] : sampling done. size = " << (*RANDOM_GRAPH_DFS_Code).size()
         << endl;
    for(int i = 0; i < (*RANDOM_GRAPH_DFS_Code).size(); ++i) {
        // [todo] if you want to save graph file, write save code here.
        cout << (*RANDOM_GRAPH_DFS_Code)[i] << endl;
    }

    return;
}

void gRandomTraverser::__traverse(int depth, Projected &projected) {

    depth++;

    if(depth > MAX_SIZE) {
        return;
    }

    if((*RANDOM_GRAPH_DFS_Code).size() >= RANDOM_GRAPH_NUM) {
        return;
    }

    if(!is_min()) {
        return;
    }

    if(RANDOM_GRAPH_UNDER_SIZE <= depth) {
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<> rand100(0, 1);

        if(rand100(mt) > UNIFORM_PROBABILITY) {
            DFSCode normal;
            DFS_CODE.normalizeRM(normal);
            (*RANDOM_GRAPH_DFS_Code).push_back(normal);
            Graph g;
            DFS_CODE.toGraph(g);
            (*RANDOM_GRAPH).push_back(g);

            if((*RANDOM_GRAPH_DFS_Code).size() >= RANDOM_GRAPH_NUM) {
                return;
            }
        }
    }

    const RMPath &rmpath = DFS_CODE.buildRMPath();
    int startLabel = DFS_CODE[0].fromlabel;
    int endToID = DFS_CODE[rmpath[0]].to;
    Projected_map3 new_fwd_root;
    Projected_map2 new_bck_root;
    EdgeList edges;

    for(unsigned int J = 0; J < projected.size(); ++J) {

        unsigned int graph_id = projected[J].id;
        PDFS *cur = &projected[J]; // projectedに格納されてる1レコード
        History history((*TRANSACTION)[graph_id], cur);

        for(int i = (int)rmpath.size() - 1; i >= 1; --i) {
            Edge *e = get_backward((*TRANSACTION)[graph_id], history[rmpath[i]],
                                   history[rmpath[0]], history);
            if(e) {
                new_bck_root[DFS_CODE[rmpath[i]].from][e->elabel].push(graph_id,
                                                                       e, cur);
            }
        }

        if(get_forward_pure((*TRANSACTION)[graph_id], history[rmpath[0]],
                            startLabel, history, edges)) {
            for(auto &e : edges) {
                new_fwd_root[endToID][e->elabel]
                            [(*TRANSACTION)[graph_id][e->to].label]
                                .push(graph_id, e, cur);
            }
        }

        for(int i = 0; i < (int)rmpath.size(); ++i) {
            if(get_forward_rmpath((*TRANSACTION)[graph_id], history[rmpath[i]],
                                  startLabel, history, edges)) {
                for(auto &e : edges) {
                    new_fwd_root[DFS_CODE[rmpath[i]].from][e->elabel]
                                [(*TRANSACTION)[graph_id][e->to].label]
                                    .push(graph_id, e, cur);
                }
            }
        }
    }

    for(auto &[to, mp_1] : new_bck_root) {
        for(auto &[elabel, prject_candidate] : mp_1) {
            DFS_CODE.push(endToID, to, -1, elabel, -1);
            __traverse(depth, prject_candidate);
            DFS_CODE.pop();
        }
    }

    for(auto &[from, mp] : new_fwd_root) {
        for(auto &[elabel, prj] : mp) {
            for(auto &[tolabel, prject_candidate] : prj) {
                DFS_CODE.push(from, endToID + 1, -1, elabel, tolabel);
                __traverse(depth, prject_candidate);
                DFS_CODE.pop();
            }
        }
    }

    return;
}

} // namespace FULL_TRAVERSE