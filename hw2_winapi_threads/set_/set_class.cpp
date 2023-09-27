#include <iostream>

template <typename T>
class TreeNode {
public:
    T data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class Set {
private:
    TreeNode<T>* root;

    // Helper function to insert a value into the BST
    TreeNode<T>* insert(TreeNode<T>* current, T value) {
        if (current == nullptr) {
            return new TreeNode<T>(value);
        }

        if (value < current->data) {
            current->left = insert(current->left, value);
        } else if (value > current->data) {
            current->right = insert(current->right, value);
        }

        return current;
    }

    // Helper function to check if a value exists in the BST
    bool contains(TreeNode<T>* current, T value) {
        if (current == nullptr) {
            return false;
        }

        if (value == current->data) {
            return true;
        } else if (value < current->data) {
            return contains(current->left, value);
        } else {
            return contains(current->right, value);
        }
    }

    // Helper function to remove a value from the BST
    TreeNode<T>* remove(TreeNode<T>* current, T value) {
        if (current == nullptr) {
            return current;
        }

        if (value < current->data) {
            current->left = remove(current->left, value);
        } else if (value > current->data) {
            current->right = remove(current->right, value);
        } else {
            // Node with only one child or no child
            if (current->left == nullptr) {
                TreeNode<T>* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                TreeNode<T>* temp = current->left;
                delete current;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest
            // in the right subtree)
            TreeNode<T>* temp = minValueNode(current->right);

            // Copy the inorder successor's content to this node
            current->data = temp->data;

            // Delete the inorder successor
            current->right = remove(current->right, temp->data);
        }

        return current;
    }

    // Helper function to find the node with the smallest value in a BST
    TreeNode<T>* minValueNode(TreeNode<T>* node) {
        TreeNode<T>* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    Set() : root(nullptr) {}

    // Add a value to the set
    bool add(T value) {
        if (!contains(root, value)) {
            root = insert(root, value);
            return true;
        }
        return false;
    }

    // Remove a value from the set
    bool remove(T value) {
        if (contains(root, value)) {
            root = remove(root, value);
            return true;
        }
        return false;
    }

    // Check if a value exists in the set
    bool contains(T value) {
        return contains(root, value);
    }
};

int main() {
    Set<int> intSet;
    
    intSet.add(10);
    intSet.add(5);
    intSet.add(15);
    intSet.add(7);

    if (intSet.contains(10)) {
        std::cout << "Set contains 10." << std::endl;
    } else {
        std::cout << "Set does not contain 10." << std::endl;
    }

    intSet.remove(15);

    if (intSet.contains(15)) {
        std::cout << "Set contains 15." << std::endl;
    } else {
        std::cout << "Set does not contain 15." << std::endl;
    }

    return 0;
}