#include <cstring>
#include <toml.hpp>

#include "root.h"

using std::string;

bool dirIsExist(std::string dir) {
    struct stat statBuf;
    return !(stat(dir.c_str(), &statBuf) == -1);
}

int main(int argc, char **argv) {

    std::string setting_path;
    int cli_opt;
    while((cli_opt = getopt(argc, argv, "a:")) != -1) {
        switch(cli_opt) {
        case 'a':
            setting_path = optarg;
            break;
        }
    }

    const auto data = toml::parse(setting_path);

    string title, data_set_name, struct_file_name;
    int rg_num, rg_max_size, rg_under_size;
    double uniform_parameter;

    title = toml::find<std::string>(data, "data_path");
    struct_file_name = toml::find<std::string>(data, "struct_file_name");

    rg_num = toml::find<int>(data, "random_graph_num");
    rg_max_size = toml::find<int>(data, "random_graph_max_size");
    rg_under_size = toml::find<int>(data, "random_graph_under_size");
    uniform_parameter =
        toml::find<double>(data, "random_graph_uniform_sampling_parameter");

    string DB_struct_path_buf = title + "/" + struct_file_name;
    const char *DB_struct_path = DB_struct_path_buf.c_str();

    std::string root_dir = "./out/";
    if(!dirIsExist(root_dir)) {
        auto err = mkdir(root_dir.c_str(), 0777);
        if(err == 1) {
            cout << "[ERROR]: mkdir faild" << endl;
            return 0;
        }
    }

    INIT_SPACE::ROOT fRoot(DB_struct_path, root_dir, rg_num, rg_max_size,
                           rg_under_size, uniform_parameter);
    fRoot.sampling();

    return 0;
}
