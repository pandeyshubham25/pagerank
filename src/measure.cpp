
#include <filesystem>

int main() {
    namespace fs = std::filesystem;
    for (fs::directory_entry data_file : fs::directory_iterator("data")) {
        
    }
}
