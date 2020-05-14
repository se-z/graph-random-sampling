
#include "root.h"
#include "string.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::max;
using std::pow;
using std::stod;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

namespace INIT_SPACE {

ROOT::ROOT(const char *structured_file, std::string out_root_dir, int rg_num,
           int rg_max_size, int rg_under_size, double uniform_parameter)
    : myreader(structured_file, &ALL_TRANS), OutDirRootPath(out_root_dir),
      traverser(rg_num, rg_max_size, rg_under_size, uniform_parameter,
                &ALL_TRANS, &RANDOM_GRAPH, &RANDOM_GRAPH_CODE){};

void ROOT::sampling() { traverser.traverse(); }
} // namespace INIT_SPACE
