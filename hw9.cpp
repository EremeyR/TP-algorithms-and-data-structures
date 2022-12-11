#include <iostream>
#include <stack>
#include <cassert>
#include <sstream>

class IsLessInt {
public:
    bool operator()(const int left, const int right) {
        return left < right;
    }
};

template<typename T, typename IsLess = IsLessInt>
class AVLTree {
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree();
    size_t Add(const T& key);
    bool Has(const T& key);
    void Remove(const T &key);
    void RemoveByPosition(size_t position);
    T GetKStat(int k);

private:
    struct Node {
        Node(const T &key) : key(key), left(nullptr), right(nullptr), height(0), count(1) {};
        T key;
        Node* left;
        Node* right;
        size_t height;
        size_t count;
    };

    Node* InnerAdd(const T &key, Node* node, size_t&  position) {
        if (!node) {
            return new Node(key);
        }
        if (is_less(key, node->key)) {
            node->right = InnerAdd(key, node->right, position);
        } else {
            position -= node->right? (node->right->count + 1) : 1;
            node->left = InnerAdd(key, node->left, position);
        }
        return Balance(node);
    }

    Node* InnerRemove(Node* node, const T &key) {
        if (!node) {
            return nullptr;
        }

        if (is_less(key, node->key)) {
            node->right = InnerRemove(node->right, key);
        } else if (is_less(node->key, key)) {
            node->left = InnerRemove(node->left, key);
        } else {
            node = RemoveNode(node);
        }

        return node? Balance(node) : nullptr;
    }

    Node* RemoveNode(Node* node) {
        Node* left = node->left;
        Node* right = node->right;

        delete node;

        if (GetHeight(left) >= GetHeight(right)) {
            if (!right) {
                return left;
            }

            Node* min;
            Node* min_right = FindAndRemoveMin(right, &min);
            min->right = min_right;
            min->left = left;
            return Balance(min);
        } else {
            if (!left) {
                return right;
            }

            Node* max;
            Node* max_left = FindAndRemoveMax(left, &max);

            max->left = max_left;
            max->right = right;
            return Balance(max);
        }
    }

    Node* InnerGetKStat(Node* node, int key) {
        if (!node) {
            return nullptr;
        } else if (GetCount(node->left) > key) {
            return InnerGetKStat(node->left, key);
        } else if (GetCount(node->left) == key) {
            return node;
        }

        return InnerGetKStat(node->right, key - GetCount(node->left) - 1);
    }

    Node* FindAndRemoveMin(Node* node, Node** min) {
        if (node->left) {
            node->left = FindAndRemoveMin(node->left, min);
            return Balance(node);
        }
        *min = node;
        return node->right;
    }

    Node* FindAndRemoveMax(Node* node, Node** max) {
        if (node->right) {
            node->right = FindAndRemoveMax(node->right, max);
            return Balance(node);
        } else {
            *max = node;
            return node->left;
        }
    }

    size_t GetHeight(Node* node) {return node ? node->height : 0;}

    size_t GetCount(Node* node) {return node ? node->count : 0;}

    void FixHeight(Node* node);

    void FixCount(Node* node) {
        node->count = GetCount(node->left) + GetCount(node->right) + 1;
    }

    int GetBalance(Node* node) {
        return GetHeight(node->right) - GetHeight(node->left);
    }

    Node* Balance(Node* node) {
        FixCount(node);
        FixHeight(node);

        switch (GetBalance(node)) {
            case 2: {
                if (GetBalance(node->right) < 0) {
                    node->right = rotateRight(node->right);
                }
                return rotateLeft(node);
            }
            case -2: {
                if (GetBalance(node->left) > 0) {
                    node->left = rotateLeft(node->left);
                }
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node* rotateLeft(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        FixHeight(node);
        FixHeight(tmp);
        FixCount(node);
        FixCount(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        FixHeight(node);
        FixHeight(tmp);
        FixCount(node);
        FixCount(tmp);
        return tmp;
    }

    void Delete(Node* node);

    Node* root;
    IsLess is_less;
};

template<typename T, typename IsLess>
AVLTree<T, IsLess>::~AVLTree() {
    Delete(root);
}

template<typename T, typename IsLess>
void AVLTree<T, IsLess>::Delete(AVLTree::Node* node) {
    if (node != nullptr ) {
        Delete(node->left);
        Delete(node->right);
        //std::cout << std::endl << node->key << " | " << node->height << " | " << node->count;
        delete node;
    }
}


template<typename T, typename IsLess>
size_t AVLTree<T, IsLess>::Add(const T &key) {
    size_t result = root ? root->count: 0;
    root = InnerAdd(key, root, result);
    return result;
}

template<typename T, typename IsLess>
bool AVLTree<T, IsLess>::Has(const T &key) {
    Node *current = root;
    while (current) {
        if (is_less(current->key, key)){
            current = current->right;
        } else if (is_less(key, current->key)){
            current = current->left;
        } else {
            return true;
        }
    }
    return false;
}

template<typename T, typename IsLess>
void AVLTree<T, IsLess>::Remove(const T &key) {
    root = InnerRemove(root, key);
}

template<typename T, typename IsLess>
T AVLTree<T, IsLess>::GetKStat(int k) {
    return InnerGetKStat(root, k)->key;
}

template<typename T, typename IsLess>
void AVLTree<T, IsLess>::FixHeight(AVLTree::Node *node) {
    node->height = std::max(GetHeight(node->left), GetHeight(node->right)) + 1;
}

template<typename T, typename IsLess>
void AVLTree<T, IsLess>::RemoveByPosition(size_t position) {
    Node* node = InnerGetKStat(root, position);
    root = InnerRemove(root, node->key);
}


int run(std::istream& input, std::ostream& output) {
    AVLTree<int> avl_tree;
    int number_of_values = 0;
    input >> number_of_values;
    int command = 0;
    int value = 0;
    for (size_t i = 0; i < number_of_values; ++i) {
        input >> command >> value;
        if (command == 2) {
            avl_tree.RemoveByPosition(value);
        } else {
            output << avl_tree.Add(value) << "\n";
        }
    }

    return 0;
}
void Tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 1 100 1 200 1 50 2 1 1 150";
        run(input, output);
        assert(output.str() == "0\n0\n2\n1\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 100 1 100 2 0";
        run(input, output);
        assert(output.str() == "0\n0\n");
    }
}
int main() {
    //Tests();
    run(std::cin, std::cout);
    return 0;
}