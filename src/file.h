#include <fstream>

using namespace std;

class File {
    public:
        void open_file(string filename);
        void close_file();
        string read_chunk();
        u_long get_file_length();
        bool is_eof;
    private:
        u_long len;
        ifstream file;
        string chunk;
        int i;
};