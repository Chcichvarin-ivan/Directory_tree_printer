/*
 * @Author: Ivan Chichvarin ichichvarin@humanplus.ru
 * @Date: 2024-04-07 19:02:58
 * @LastEditors: Ivan Chichvarin ichichvarin@humanplus.ru
 * @LastEditTime: 2024-04-07 22:04:50
 * @FilePath: /DirectoryTree/directory_tree.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintTree(ostream& dst, const path& p);

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(out, "test_dir"_p);
    assert(out.str() ==
        "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"s
    );
}

void PrintTree(ostream& out, const path& p, const filesystem::file_status& in_status, int in_offset) {
    
    out << string(in_offset, ' ') << p.filename().string() << endl;
    
    if (in_status.type() == filesystem::file_type::directory) {//we havent reached the leaf
        vector<filesystem::directory_entry> objects;

        for (const auto& dir_entry : filesystem::directory_iterator(p)) {
            objects.push_back(dir_entry);
        }
        
        sort(objects.begin(), objects.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.path().filename() > rhs.path().filename();
            });
        
        for (const auto& dir_entry : objects) {
            PrintTree(out, dir_entry.path(), dir_entry.status(), in_offset + 2);
        }
    }
}

void PrintTree(ostream& dst, const path& p){
    error_code error;

    auto file_status = std::filesystem::status(p, error);

    if(error){
        return;
    }   

    PrintTree(dst, p, file_status, 0);
}