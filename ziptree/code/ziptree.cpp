#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

struct Node {
    int key;
    int rank;
    Node *left, *right;

    Node(int k, int r) : key(k), rank(r), left(nullptr), right(nullptr) {}
};

class ZipTree {
private:
    Node* root;
    int getRandomRank() {
        int rank = 0;
        // Keep flipping "heads" (rand() % 2 == 1)
        while (rand() % 2 == 1) {
            rank++;
        }
        return rank;
    }
    
    Node* traverse(Node* location, int x){
        if (location == nullptr){
            return nullptr;
        }
        else if (x < location->left->key)
        {
            return traverse(location->left, x);
            
        }
        else if (x < location->right->key)
        {
            return traverse(location->right, x);
        }
        else {
            return location;
        }
    }
    // Add your own private helper functions here.  

public:
    ZipTree() : root(nullptr) {
        // Seed the random number generator once.
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Functions for you to implement:

    void insert(int x) {
        int rank = getRandomRank();
        insert_with_rank(x, rank);
    }

    // Insert key x with a specified rank r.
    void insert_with_rank(int x, int r) {
        Node* parent = nullptr;
        Node* current = root;
        
        while (current != nullptr && ((current->rank > r) || (current->rank == r && current->key < x))){
            parent = current;
            if (x < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        Node* u = current;
        Node* newNode = new Node(x, r);
        Node* leftP = nullptr;
        Node* rightQ = nullptr; 

        while (u != nullptr) {
            if (u->key < x) {
                if (newNode->left == nullptr) {
                    newNode->left = u;
                    leftP = u;
                } else {
                    leftP->right = u;
                    leftP = u;
                }
                u = u->right;
            } else {
                if (newNode->right == nullptr) {
                    newNode->right = u;
                    rightQ = u;
                } else {
                    rightQ->left = u;
                    rightQ = u; 
                }
                u = u->left;
            }
        }
        if (leftP != nullptr){
            leftP->right = nullptr;
        }
        if (rightQ != nullptr) {
            rightQ->left = nullptr;
        }
        if (parent == nullptr) {
            root = newNode;
        } else if (x < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }

    // Delete the node with value x.
    void delete_val(int x) {
      Node* current = root;
      Node* parent = nullptr;     
      while(current->key != x){
        parent = current;
        if (x < current->key) {
            current = current->left;
        } 
        else if (x > current->key) {
            current = current->right;
        }
        else if (current == nullptr){
            return;
        }
      }
      Node* lt = current->left;
      Node* rt = current->right; 
      
      while (lt != nullptr && rt != nullptr) {
        if (lt->rank > rt->rank) {
            if (parent == nullptr) {
                root = lt;
            } 
            else if (parent->left == current) {
                parent->left = lt;
            } 
            else {
                parent->right = lt;
            }
            parent = lt;
            lt = lt->right;
        } 
        else {
            if (parent == nullptr) {
                root = rt;
            } 
            else if (parent->left == current) {
                parent->left = rt;
            } 
            else {
                parent->right = rt;
            }
            parent = rt;
            rt = rt->left;
        }
      }
      
      if (parent == nullptr) {
        if (lt != nullptr) {
            root = lt;
        } else {
            root = rt;
        }
      } else {
        if (lt != nullptr) {
            parent->right = lt;
        } else {
            parent->left = rt;
        }
      }
      
      delete current;
    }

    // Determine whether the tree contains x.
    bool contains(int x) {
      Node* current = root;
      while (current != nullptr){
        if (x == current->key){
            return true;
        }
        else if (x < current->key){
            current = current->left;
        }
        else {
            current = current->right;
        }
      }
      return false;
    }

    void printcontains(int x){
      if (contains(x)){
        std::cout <<"true" << std::endl;
      }
      else {
        std::cout << "false" << std::endl;
      }
    }

    // Finds the depth of the node x in the tree.
    // If it's not in the tree, then return -1.
    int getdepth(int x) {
        Node* current = root;
        int depth = 0;
        while (current != nullptr) {
            if (x == current->key) {
                return depth;
            } else if (x < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
            depth++;
        }
        return -1;
    }

    void printdepth(int x) {
      std::cout << getdepth(x) << std::endl;
    }

};

// You do not need to modify this section.

std::vector<std::string> get_args(const std::string& line) {
    std::vector<std::string> args;
    std::stringstream ss(line);
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

int main() {
    ZipTree tree;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::vector<std::string> args = get_args(line);
        if (args.empty()) continue;
        
        const std::string& command = args[0];

        try {
            if (command == "insert" && args.size() == 2) {
                tree.insert(std::stoi(args[1]));
            } else if (command == "insert_with_rank" && args.size() == 3) {
                tree.insert_with_rank(std::stoi(args[1]), std::stoi(args[2]));
            } else if (command == "delete" && args.size() == 2) {
                tree.delete_val(std::stoi(args[1]));
            } else if (command == "contains" && args.size() == 2) {
                tree.printcontains(std::stoi(args[1]));
            } else if (command == "getdepth" && args.size() == 2) {
                tree.printdepth(std::stoi(args[1]));
            } else {
                std::cout << "Error: Unknown command or incorrect arguments." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: Invalid argument." << std::endl;
        }
    }
    return 0;
}
