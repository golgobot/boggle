#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <ctime>

using namespace std;


struct TrieNode {
    TrieNode() : letter('\0'), is_word(false), parent(0) {}
    char letter;
    bool is_word;
    TrieNode* parent;
    unordered_map<char, TrieNode*> children;

    //returns the word that is represented at this node
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

TrieNode* createTrie() {
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
        srand(time(0));
        string alphaString = "qwertyuiopasdfghjklzxcvbnm";
        //randomly generate our board
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

    /**
     * Uses a trie to find all valid words in a phrase
     */
    vector<string> findWordsInPhrase(const vector<char>& chars, TrieNode* root) {
        vector<string> ret;
        unordered_set<TrieNode*> nodes;
        //always have root
        nodes.insert(root);
        for(char value : chars) {
            //create sets of nodes to add and delete from our node set
            unordered_set<TrieNode*> del;
            unordered_set<TrieNode*> add;
            //go through each node in our set and check if the current
            //character is a child
            for(auto it = nodes.begin(); it != nodes.end(); ++it) {
                TrieNode* node = *it;
                if(node->children.count(value) > 0) {
                    TrieNode* child = node->children[value];
                    add.insert(child);
                }
                del.insert(node);
            }
            //delete these nodes first
            for(auto it = del.begin(); it != del.end(); ++it) {
                nodes.erase(*it);
            }
            //add nodes
            for(auto it = add.begin(); it != add.end(); ++it) {
                TrieNode* node = *it;
                //if we're adding this node and this node is a word
                //then add this to our return list
                if(node->is_word) {
                    ret.push_back(node->get_word());
                }
                nodes.insert(*it);
            }
            
            //always make sure root is in our set
            nodes.insert(root);
        }
        return ret;
    }

    vector<vector<string>> findAllWords(TrieNode* root) {
        vector<vector<string>> ret;
        //for each row create phrases that are
        //a: that row
        //b: the diagonal of that row
        for(int row = 0; row < size; row++) {
            vector<char> chars;
            for(int col = 0; col < size; col++) {
                char value = board[getIndex(row, col)];
                chars.push_back(value);
            }
            ret.push_back(findWordsInPhrase(chars, root));
            vector<char> diagChars;
            for(int r = row, c = 0; getIndex(r, c) < board.size(); r++, c++) {
                char value = board[getIndex(r, c)];
                diagChars.push_back(value);
            }
            ret.push_back(findWordsInPhrase(diagChars, root));
        }
        //for each column create phrases that are
        //a: that column
        //b: the diagonal of that column
        for(int col = 0; col < size; col++) {
            vector<char> chars;
            for(int row = 0; row < size; row++) {
                char value = board[getIndex(row, col)];
                chars.push_back(value);
            }
            ret.push_back(findWordsInPhrase(chars, root));
            vector<char> diagChars;
            for(int r = 0, c = col; getIndex(r, c) < board.size(); r++, c++) {
                char value = board[getIndex(r, c)];
                diagChars.push_back(value);
            }
            ret.push_back(findWordsInPhrase(diagChars, root));
        }
        return ret;
    }

    //use a single vector to represent our board
    vector<char> board;
    unordered_set<char> alpha;
private:
    int size;
    int getRow(int index) {
        return static_cast<int>(index / size);
    }
    int getCol(int index) {
        return index % size;
    }
    int getIndex(int row, int col) {
        return (row * size) + col;
    }
};

int main() {
    TrieNode* root = createTrie();
    BoggleBoard b(10);
    b.print();
    auto wordLists = b.findAllWords(root);
    for(auto words : wordLists) {
        for(string word : words) {
            cout << word << " ";
        }
    }
    cout << endl;
    return 0;
}