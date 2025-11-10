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
    
    void deleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    ZipTree() : root(nullptr) {
        // Seed the random number generator once.
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    ~ZipTree() {
        deleteTree(root);
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
        
        // Search down until we find position where new node should go
        // Stop when: current.rank < r OR (current.rank == r AND current.key >= x)
        while (current != nullptr && (current->rank > r || (current->rank == r && current->key < x))) {
            if (x == current->key) {
                return; // Key already exists
            }
            parent = current;
            if (x < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        // Check if key already exists in subtree below
        Node* u = current;
        while (u != nullptr) {
            if (u->key == x) {
                return;
            }
            if (x < u->key) {
                u = u->left;
            } else {
                u = u->right;
            }
        }
        
        // Unzip: split current subtree into left path P and right path Q
        u = current;
        Node* newNode = new Node(x, r);
        Node* leftP = nullptr;
        Node* rightQ = nullptr; 

        while (u != nullptr) {
            if (u->key < x) {
                // Node goes to left path P
                if (newNode->left == nullptr) {
                    newNode->left = u;
                    leftP = u;
                } 
                else {
                    leftP->right = u;
                    leftP = u;
                }
                u = u->right;
            } else {
                // Node goes to right path Q
                if (newNode->right == nullptr) {
                    newNode->right = u;
                    rightQ = u;
                } 
                else {
                    rightQ->left = u;
                    rightQ = u; 
                }
                u = u->left;
            }
        }
        
        // Clean up ends of paths
        if (leftP != nullptr) {
            leftP->right = nullptr;
        }
        if (rightQ != nullptr) {
            rightQ->left = nullptr;
        }
        
        // Connect new node to parent
        if (parent == nullptr) {
            root = newNode;
        } 
        else if (x < parent->key) {
            parent->left = newNode;
        } 
        else {
            parent->right = newNode;
        }
    }

    // Delete the node with value x.
    void delete_val(int x) {
      Node* current = root;
      Node* parent = nullptr;
      
      // Search for node with key x
      while(current != nullptr && current->key != x){
        parent = current;
        if (x < current->key) {
            current = current->left;
        } 
        else {
            current = current->right;
        }
      }
      
      if (current == nullptr) {
          return; // Node not found
      }
      
      Node* deleting = current; 
      Node* left = current->left;
      Node* right = current->right; 
      Node* newRoot = nullptr;
      Node* tail = nullptr;
      
      // Zip: merge left and right subtrees
      // Walk down right spine of left subtree and left spine of right subtree
      // Merge them in decreasing rank order
      while (left != nullptr && right != nullptr) {
        // Choose node with higher rank (or smaller key if ranks are equal)
        if (left->rank > right->rank || (left->rank == right->rank && left->key < right->key)) {
            if (newRoot == nullptr) {
                newRoot = left;
            } 
            else {
                // Place based on key comparison with tail
                if (left->key < tail->key) {
                    tail->left = left;
                } 
                else {
                    tail->right = left;
                }
            }
            tail = left;
            left = left->right; // Continue down right spine of left subtree
        } 
        else {
            if (newRoot == nullptr) {
                newRoot = right;
            } 
            else {
                // Place based on key comparison with tail
                if (right->key < tail->key) {
                    tail->left = right;
                } 
                else {
                    tail->right = right;
                }
            }
            tail = right;
            right = right->left; // Continue down left spine of right subtree
        }
      }
      
      // Attach remaining nodes
      if (tail != nullptr) {
        if (left != nullptr) {
            if (left->key < tail->key) {
                tail->left = left;
            } 
            else {
                tail->right = left;
            }
        } 
        else if (right != nullptr) {
            if (right->key < tail->key) {
                tail->left = right;
            } 
            else {
                tail->right = right;
            }
        }
      } 
      else {
        // One or both subtrees were empty
        if (left != nullptr) {
            newRoot = left;
        } 
        else {
            newRoot = right;
        }
      }
      
      // Replace deleted node with merged result
      if (parent == nullptr) {
        root = newRoot;
      } 
      else if (parent->left == deleting) {
        parent->left = newRoot;
      } 
      else {
        parent->right = newRoot;
      }
      
      delete deleting;
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
    int getdepth(int x) {
        Node* current = root;
        int depth = 0;
        while (current != nullptr) {
            if (x == current->key) {
                return depth;
            } 
            else if (x < current->key) {
                current = current->left;
            } 
            else {
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
