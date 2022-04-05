#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>


//Unsure if we want this os-specific so added the logic for it and left the same for now.
#if defined(__APPLE__)
    #define PROCESSORS 9
#else
    #define PROCESSORS 9
#endif

#define NUM_ITERS 100
#define ALPHA 0.85


int main() {
#ifdef TEST
    std::vector<std::string> PR_VERSIONS = {"test/pr_mock_long", "test/pr_mock", "test/pr_mock_short"};
#else
    std::vector<std::string> PR_VERSIONS = {"pagerank"};
#endif
    namespace fs = std::filesystem;
    std::ofstream gp_data("gnuplot_data", std::ofstream::out );
    bool first = true;
    for (std::string pr_version : PR_VERSIONS) {
        if (first) {
            first = false;
            gp_data << "Data,";
        } else {
            gp_data << ",";
        }
        gp_data << pr_version;
    }
    gp_data << "\n";
        for (fs::directory_entry data_file : fs::directory_iterator("data")) {
        std::vector<double> runtimes;
        first = true;
    for (std::string pr_version : PR_VERSIONS) {
            char pr_command[300];
            std::snprintf(pr_command, 300, "./%s %d, %d, %f, data/%s\n", pr_version.c_str(), PROCESSORS, NUM_ITERS, ALPHA, data_file.path().filename().string().c_str());
            std::cout << pr_command;
            FILE *pr_output = popen(pr_command, "r");
            char time_str[50];
            fgets(time_str, 50, pr_output);
            double runtime = atof(time_str);
            if (first) {
                first = false;
                gp_data << data_file.path().filename().string() << ",";
            } else {
                gp_data << ",";
            }
            gp_data << runtime;
            pclose(pr_output);
        }
        gp_data << "\n";
    }
    std::ofstream gp_instructions("gnuplot_instructions", std::ofstream::out );
    gp_instructions << "set key autotitle columnhead noenhanced\n";
    gp_instructions << "set datafile separator \",\"\n";
    char plot_command[500];
    std::snprintf(plot_command, 500, "plot for [i=2:%lu] \"gnuplot_data\"  using (column(0)):i:xtic(1)\n", PR_VERSIONS.size() + 1);
    gp_instructions << plot_command;
    gp_data.close();
    gp_instructions.close();
    pclose(popen("gnuplot gnuplot_instructions -p", "w"));
    remove("gnuplot_data");
    remove("gnuplot_instructions");
}
