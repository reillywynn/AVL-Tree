#include <iostream>
#include <string>
#include <vector>
using namespace std;


class AVLTree
{
public:
    struct Node
    {
        string name;
        string id;
        Node* left;
        Node* right;
        int balanceFactor;
        Node() : left(nullptr), right(nullptr), name(""), id("") {}
        Node(string name, string id) : name(name), id(id), left(nullptr), right(nullptr) {}
    };

    /*=====Insert Remove Search Functions=====*/
    bool insertID(string& name, string& id);
    bool removeID(string& id);
    Node* searchID(Node* node, string& id);
    vector<Node*>searchName(Node* node, string& name);
    bool removeInorder(int n);

    /*=====Print Functions=====*/
    void printInorder(Node* head);
    void printPreorder(Node* head);
    void printPostorder(Node* head);
    void printLevelCount(Node* head);

    /*=====Helper Functions=====*/
    int height(Node* node);
    void balanceFactor(Node* node);
    Node* minNode(Node* node);
    Node* MaxNode(Node* node);
    Node* balance(Node* node);
    void storeInorder(Node* node);

    Node* head = nullptr;
    vector<Node*> list;

private:
    /*=====Rotate Functions=====*/
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeftRight(Node* node);
    Node* rotateRightLeft(Node* node);

    /*=====Recursive Insert/Remove Helper Functions=====*/
    Node* insertHelper(Node* node, string& name, string& id);
    Node* removeHelper(Node* node, string& id);
};


bool AVLTree::insertID(string& name, string& id)
{
    if (searchID(head, id) == nullptr) { // Makes sure ID doesn't exist already
        head = insertHelper(head, name, id);
        head = balance(head); // Balances the tree
        balanceFactor(head); // Updates balance factor
        return true;
    }
    else {
        return false;
    }
}

AVLTree::Node* AVLTree::insertHelper(Node* node, string& name, string& id)
{
    if (node == nullptr) {
        node = new Node(name, id); // Creates new node
        return node;
    }
    else if (id < node->id) { // Left subtree
        node->left = insertHelper(node->left, name, id);
    }
    else { // Right subtree
        node->right = insertHelper(node->right, name, id);
    }
    return node;
}

bool AVLTree::removeID(string& id)
{
    if (searchID(head, id) != nullptr) { // Makes sure the ID exists
        head = removeHelper(head, id); // Removes node
        head = balance(head); // Rebalances the tree after removal
        balanceFactor(head); //Updates balance factors
        return true;
    }
    else {
        return false;
    }
}

AVLTree::Node* AVLTree::removeHelper(Node* node, string& id)
{
    if (node == nullptr) {
        return node;
    }
    if (id < node->id) { //Traverses left subtree until ID is found
        node->left = removeHelper(node->left, id);
        return node;
    }
    else if (id > node->id) { //Traverses right subtree until ID is found
        node->right = removeHelper(node->right, id);
        return node;
    }
    else {

        if (node->left == nullptr || node->right == nullptr) {
            Node* temp = node;
            if (temp->left == nullptr) { //1 child on the right case
                node = temp->right;
                return node;
            }
            else if (temp->right == nullptr) { //1 child on the left case
                node = temp->left;
                return node;
            }
            else { //No children
                *node = *temp;
            }
            delete temp;
        }
        else {
            Node* temp = minNode(node->right); // copies parent and children nodes to the temporary then reasigns the node
            temp->left = node->left;
            temp->right = temp->right;
            node->id = temp->id;
            node->name = temp->name;
            *node = *temp;
            delete temp;
            return node;
        }
    }
}

AVLTree::Node* AVLTree::minNode(Node* node) { //Finds the minimum value ID node
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}
AVLTree::Node* AVLTree::MaxNode(Node* node) { //Finds the maximum value ID node
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

vector<AVLTree::Node*> AVLTree::searchName(Node* node, string& name)
{
    vector<AVLTree::Node*> sameName; //Vector for nodes with same name
    list.clear();
    storeInorder(head);
    for (int i = 0; i < list.size(); i++) {
        if (list[i]->name == name) { //If the name is equal to the key value it adds to the vector
            sameName.push_back(list[i]);
        }
    }
    return sameName;
}

void AVLTree::storeInorder(Node* node) //Stores the inorder traversal in a vector
{
    if (node == nullptr) {
        return;
    }
    else {
        storeInorder(node->left);
        list.push_back(node);
        storeInorder(node->right);
    }
}

AVLTree::Node* AVLTree::searchID(Node* node, string& id) // recursive searchID function
{
    if (node == nullptr) {
        return nullptr;
    }
    else if (id < node->id) {
        return searchID(node->left, id);
    }
    else if (id > node->id) {
        return searchID(node->right, id);
    }
    else {
        return node;
    }
}

int AVLTree::height(Node* node) //Finds the height of a node
{
    if (node == nullptr) { //Empty case
        return 0;
    }
    if (node->left == nullptr && node->right == nullptr) //1 head node case
        return 1;

    int left = 0;
    if (node->left != nullptr)
        left = height(node->left);

    int right = 0;
    if (node->right != nullptr)
        right = height(node->right);

    return (max(left, right) + 1);
}

void AVLTree::balanceFactor(Node* node)
{
    if (node == nullptr) {
        return;
    }

    else {
        node->balanceFactor = height(node->left) - height(node->right); //Subtracts left height from right height
        balanceFactor(node->left);
        balanceFactor(node->right);
    }
}

AVLTree::Node* AVLTree::balance(Node* node)
{
    if (node == nullptr) {
        return node;
    }

    balanceFactor(head);
    node->left = balance(node->left);
    node->right = balance(node->right);
    balanceFactor(head);

    if (abs(node->balanceFactor) >= 2) {
        if (node->balanceFactor == 2 && node->left->balanceFactor == 1) { //Right rotation
            node = rotateRight(node); //Left-Left case
        }

        else if (node->balanceFactor == -2 && node->right->balanceFactor == -1) { //Left rotation
            node = rotateLeft(node); //Right-Right case
        }

        else if (node->balanceFactor == 2 && node->left->balanceFactor == -1) { //Left-Right rotation
            node = rotateLeftRight(node); //Left-Right case
        }

        else if (node->balanceFactor == -2 && node->right->balanceFactor == 1) { //Right-Left rotation
            node = rotateRightLeft(node); //Right-Right case
        }
    }

    return node;
}

void AVLTree::printInorder(Node* head)
{
    if (head == nullptr) {
        return;
    }

    else {
        printInorder(head->left);
        cout << head->name;
        if (head == MaxNode(this->head)) { // prints a new line once the max node in the tree is printed
            cout << endl;
        }
        else {
            cout << ", ";
        }
        printInorder(head->right);
    }

}

void AVLTree::printPreorder(Node* head)
{
    if (head == nullptr) {
        return;
    }

    else {
        cout << head->name;
        if (head == MaxNode(this->head)) { //prints a new line once the max node in the tree is printed
            if (head->left == nullptr) {
                cout << endl;
            }
            else {
                cout << ", " << head->left->name << endl;
            }
            return;
        }
        else {
            cout << ", ";
        }
        printPreorder(head->left);
        printPreorder(head->right);
    }
}
void AVLTree::printPostorder(Node* head)
{
    if (head == nullptr) {
        return;
    }

    else {
        printPostorder(head->left);
        printPostorder(head->right);
        cout << head->name;
        if (head == this->head) {
            cout << endl;
        }
        else {
            cout << ", ";
        }
    }

}

void AVLTree::printLevelCount(Node* root) //Returns the height of the head node (height of the tree)
{
    int h = height(root);
    cout << h << endl;
}

bool AVLTree::removeInorder(int n)
{
    storeInorder(head); //Stores the AVL tree in order vector then removes the nth node
    if (n < 0 || n > list.size() - 1) {
        return false;
    }
    return removeID(list[n]->id);
}

AVLTree::Node* AVLTree::rotateLeft(Node* node)
{
    Node* rightChild = node->right;
    node->right = rightChild->left;
    rightChild->left = node;
    return rightChild;
}

AVLTree::Node* AVLTree::rotateRight(Node* node)
{
    Node* leftChild = node->left;
    node->left = leftChild->right;
    leftChild->right = node;
    return leftChild;
}

AVLTree::Node* AVLTree::rotateLeftRight(Node* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

AVLTree::Node* AVLTree::rotateRightLeft(Node* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

/*======================== main ========================*/

bool nameCheck(string& name)
{
    for (int i = 0; i < name.size(); i++) {
        if (!isalpha(name[i]) && !isblank(name[i])) {
            return false;
        }
    }
    return true;
}

bool IDCheck(string& id)
{
    if (id.size() != 8) {
        return false;
    }

    for (int i = 0; i < id.size(); i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }
    return true;
}

int main()
{
    string command;
    int numCommands = 0;
    string name;
    string id;
    AVLTree gatorAVL;

    cin >> numCommands;
    for (int i = 0; i < numCommands; i++) {
        cin >> command;
        if (command == "insert") {
            string space;
            getline(cin, space, '\"');
            getline(cin, name, '\"');
            cin >> id;

            if (nameCheck(name) && IDCheck(id)) {
                if (gatorAVL.insertID(name, id) != false) {
                    cout << "successful" << endl;
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        else if (command == "remove") {
            cin >> id;
            if (IDCheck(id) != false) {
                if (gatorAVL.removeID(id) == true) {
                    cout << "successful" << endl;
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        else if (command == "search") {
            string space;
            string searchType;
            getline(cin, space, ' ');
            getline(cin, searchType);

            try {
                id = stoi(searchType);
                id = searchType;
                if (IDCheck(id) == true) {
                    AVLTree::Node* matchingNode = gatorAVL.searchID(gatorAVL.head, id);
                    if (matchingNode != nullptr) {
                        cout << matchingNode->name << endl;
                    }
                    else {
                        cout << "unsuccessful" << endl;
                    }
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }

            catch (invalid_argument) {
                name = searchType.substr(1, searchType.size() - 2);
                if (nameCheck(name) == true) {
                    vector<AVLTree::Node*> sameName = gatorAVL.searchName(gatorAVL.head, name);
                    if (!sameName.empty()) {
                        for (int j = 0; j < sameName.size(); j++) {
                            cout << sameName.at(j)->id << endl;
                        }
                    }
                    else {
                        cout << "unsuccessful" << endl;
                    }
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
        }

        else if (command == "printInorder") {
            gatorAVL.printInorder(gatorAVL.head);
        }

        else if (command == "printPreorder") {
            gatorAVL.printPreorder(gatorAVL.head);
        }

        else if (command == "printPostorder") {
            gatorAVL.printPostorder(gatorAVL.head);
        }

        else if (command == "printLevelCount") {
            gatorAVL.printLevelCount(gatorAVL.head);
        }

        else if (command == "removeInorder") {
            int N = 0;
            cin >> N;
            if (gatorAVL.removeInorder(N))
                cout << "successful" << endl;
            else
                cout << "unsuccessful" << endl;
        }
    }

    return 0;
}