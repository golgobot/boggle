#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;


struct TrieNode {
    TrieNode() : letter('\0'), is_word(false), parent(0) {}
    char letter;
    bool is_word;
    TrieNode* parent;
    unordered_map<char, TrieNode*> children;

    std::string get_word() {
        string ret;
        ret.push_back(letter);
        TrieNode* p = parent;
        while (p) {
            ret.push_back(p->letter);
            p = p->parent;
        }
        std::reverse(ret.begin(), ret.end());
        return ret;
    }
};

TrieNode* create_word_dict() {
    std::ifstream infile("./wordlist.txt");
    std::string line;
    TrieNode* root = new TrieNode();
    while (std::getline(infile, line)) {
        const char* chars = line.c_str();
        auto node = root;
        for (int i = 0; i < line.size(); i++) {
            char c = chars[i];
            //if not found create an entry
            if (node->children.find(c) == node->children.end()) {
                //make a new one
                auto temp_node = new TrieNode();
                temp_node->letter = c;
                temp_node->parent = node;
                node->children[c] = temp_node;
            }
            //traverse to new node
            node = node->children[c];
            if (i == line.size() - 1) { node->is_word = true; }
        }
    }
    return root;
}

class BoggleBoard {
public:
    BoggleBoard(int size): size(size), board(size * size) {
        string alphaString = "qwertyuiopasdfghjklzxcvbnm";
        for(int i = 0; i < size * size; i++) {
            int index = rand() % (alphaString.size() - 1);
            board[i] = alphaString[index];
        }

    }
    void print() {
        for(int i = 0; i < board.size(); i++) {
            cout << board[i] << " ";
            if(((i + 1) % size) == 0) {
                cout << endl;
            }
        }
    }
    vector<char> board;
    unordered_set<char> alpha;
private:
    int size;
};

int main() {
    BoggleBoard b(4);
    b.print();
    return 0;
}