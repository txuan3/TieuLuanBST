#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <ctime>
#include <functional>

using namespace std;

unsigned long long  _global_compare_count=0;
enum Color { RED, BLACK };

struct Node{
    int key;
    Node *left;
    Node *right;
    // thuộc tính của AVL
    int height;
    //thuộc tính của RB
     Color color;

     Node (int val){
        key=val;
        left=nullptr;
        right=nullptr;
        height=1;
        color=RED;
     }
};


//tính chiều cao cây
int getTreeHeight(Node *root){
    if (root==nullptr){
        return 0;
    }

    int leftHeight=getTreeHeight(root ->left);
    int rightHeight=getTreeHeight(root ->right);
    return 1+ max(leftHeight,rightHeight);
    
}

//sinh mang ngẫu nhiên (không trùng)
vector <int> randomData(int n){
    vector<int>data(n);
    for (int i = 0; i <n; i++){
       data[i]=i+1;
    }
        random_device rd;
        mt19937 g(rd());
        shuffle(data.begin(), data.end(), g);
    return data;
}

//sinh mảng đã sắp xếp (xấu nhất của BST)
vector<int> sortData(int n){
    vector<int> data (n);
    for (int i = 0; i < n; i++){
        data[i]=i+1;
    }
    return data;
}

//do thgian
//C1
/*double measureExecutionTime(const function<void()>& func){
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}*/

//C2
double measureExecutionTime(const function<void()>& func){
    auto start=chrono::high_resolution_clock::now();//ghi thgian bắt đầu
    func();
    auto end=chrono::high_resolution_clock::now();//ghi thgian kết thúc
    return chrono::duration<double,milli>(end-start).count();
}

//bst

void addNode(Node*& root, const int x)
{
    if (root == nullptr){
        root = new Node(x);
            return;
    }
     _global_compare_count++;      
    if (root->key > x){
         addNode(root->left, x);
    }
    else if (root->key < x){
        _global_compare_count++;
        addNode(root->right, x);
    }
}


Node* searchNode(Node* root, const int x)
{
    _global_compare_count++;
    if (root == nullptr || root->key == x)
        return root;

    _global_compare_count++;
    if (root->key > x)
        return searchNode(root->left, x);
    else
        return searchNode(root->right, x);
}


void deleteNode(Node*& root, const int x)
{
    if (root == nullptr)
        return;
    _global_compare_count++;

    if (x < root->key)
        deleteNode(root->left, x);
    else if (x > root->key){
    _global_compare_count++;
        deleteNode(root->right, x);
    } 

    else{
        if (root->left == nullptr)
        {
            Node* temp = root;
            root = root->right;
            delete temp;
        }
        else if (root->right == nullptr)
        {
            Node* temp = root;
            root = root->left;
            delete temp;
        }
        else
        {
            Node* temp = root->right;

            while (temp->left != nullptr)
                temp = temp->left;

            root->key = temp->key;
            deleteNode(root->right, temp->key);
        }
    }
}
void LNR(Node* root)
{
    if (root != nullptr){
        LNR(root->left);
        cout << root->key << " ";
        LNR(root->right);
    }
}

//avl 
int height(Node* n) {
     return n ? n->height : 0; 
     }

void updateHeight(Node* n) {
    n->height = 1 + max(height(n->left), height(n->right));
}

// Hệ số cân bằng = h(left) - h(right)
int balanceFactor(Node* n) {
     return n ? height(n->left) - height(n->right) : 0; 
     }

// Xoay phải
Node* rotateRight(Node* y) {
    Node* x  = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left  = T2;
    updateHeight(y);         // cập nhật y trước vì y giờ là con của x
    updateHeight(x);
    return x;                // x là gốc mới
}

// Xoay trái
Node* rotateLeft(Node* x) {
    Node* y  = x->right;
    Node* T2 = y->left;
    y->left  = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

// Cân bằng lại một nút sau khi thêm/xóa (dùng chung cho cả insert và remove)
Node* rebalance(Node* n) {
    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1) {                                   // lệch trái
        if (balanceFactor(n->left) < 0)             // Trường hợp LR
            n->left = rotateLeft(n->left);
        return rotateRight(n);                      // Trường hợp LL (hoặc bước 2 của LR)
    }
    if (bf < -1) {                                  // lệch phải
        if (balanceFactor(n->right) > 0)            // Trường hợp RL
            n->right = rotateRight(n->right);
        return rotateLeft(n);                       // Trường hợp RR (hoặc bước 2 của RL)
    }
    return n;                                       // đã cân bằng
}

// Thêm nút
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);             // 1. Chèn như BST thường

    _global_compare_count++;
    if (key < root->key){
        root->left  = insert(root->left, key);
    }      
    else if (key > root->key){
        _global_compare_count++;
        root->right = insert(root->right, key);
    } 
    else {
        _global_compare_count++;
             return root;    // không chèn khóa trùng
    }                               
    return rebalance(root);                         // 2. Cân bằng lại trên đường quay về
}

// Xóa nút
Node* minNode(Node* n) {
    while (n->left) n = n->left;
    return n;
}

Node* remove(Node* root, int key) {
    if (!root) return nullptr;                      // không tìm thấy

    _global_compare_count++;
    if (key < root->key) {
        root->left = remove(root->left, key);
    }    
    else if (key > root->key){
        _global_compare_count++;
         root->right = remove(root->right, key);
    }
    else {
        // Nút có 0 hoặc 1 con
        if (!root->left || !root->right) {
            Node* child = root->left ? root->left : root->right;
            delete root;
            return child;                           // có thể là nullptr
        }
        // Nút có 2 con: thay bằng nút kế tiếp (nhỏ nhất bên phải)
        Node* succ = minNode(root->right);
        root->key = succ->key;
        root->right = remove(root->right, succ->key);
    }
    return rebalance(root);                         // cân bằng lại
}

//  Duyệt & giải phóng 
void inorder(Node* root) {
    if (!root){
        return;
    } 
    inorder(root->left);
    cout << root->key << "(bf=" << balanceFactor(root) << ") ";
    inorder(root->right);
}

void destroy(Node* root) {
    if (!root){
        return;
    } 
    destroy(root->left);
    destroy(root->right);
    delete root;
}

//red-black 
//RED BLACK
struct RBNode {
    int key;
    Color color;
    RBNode *left, *right, *parent;
};

class RedBlackTree {
private:
    RBNode* root;
    RBNode* NIL;

    RBNode* newNode(int key) {
        return new RBNode{key, RED, NIL, NIL, nullptr};
    }

    void rotateLeft(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        if (x->right != NIL) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;
        x->right = y;
        y->parent = x;
    }

    void insertFixup(RBNode* z) {
        while (z->parent && z->parent->color == RED) {
            RBNode* gp = z->parent->parent;
            if (z->parent == gp->left) {
                RBNode* uncle = gp->right;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateRight(gp);
                }
            } else {
                RBNode* uncle = gp->left;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateLeft(gp);
                }
            }
        }
        root->color = BLACK;
    }

    void destroy(RBNode* n) {
        if (n == NIL) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    int calcHeight(RBNode* n) {
        if (n == NIL) return 0;
        return 1 + max(calcHeight(n->left), calcHeight(n->right));
    }

public:
    RedBlackTree() {
        NIL = new RBNode{0, BLACK, nullptr, nullptr, nullptr};
        root = NIL;
    }
    ~RedBlackTree() { destroy(root); delete NIL; }

    void insert(int key) {
        RBNode* y = nullptr;
        RBNode* x = root;
        while (x != NIL) {
            y = x;
            _global_compare_count++;
            if (key < x->key) x = x->left;
            else if (key > x->key) {
                _global_compare_count++;
                x = x->right;
            } else {
                _global_compare_count++;
                return;
            }
        }
        RBNode* z = newNode(key);
        z->parent = y;
        if (y == nullptr) root = z;
        else if (key < y->key) y->left = z;
        else y->right = z;

        insertFixup(z);
    }

    int getHeight() { return calcHeight(root); }
};
int main(){
  vector<int> test_sizes = {100, 1000, 5000};
    cout << "======== KET QUA TEST ========\n";

    for (int n : test_sizes) {
        cout << "\nKich thuoc N = " << n << endl;
        vector<int> randData = randomData(n);
        vector<int> sortedData = sortData(n);

        //đo thời gian thử nghiệm bằng Lambda [&]
        //đo bst
        Node* rootBST = nullptr;
        _global_compare_count = 0;
        double timeBST = measureExecutionTime([&]() {
            for (int val : randData) {
                addNode(rootBST, val);
            }
        });
        int heightBST = getTreeHeight(rootBST);
        cout << "BST: Thoi gian: " << timeBST << " ms | So so sanh: " << _global_compare_count 
             << " | Chieu cao h: " << heightBST << endl;

        //đo avl
        Node* rootAVL = nullptr;
        _global_compare_count = 0;
        double timeAVL = measureExecutionTime([&]() {
            for (int val : randData) {
                rootAVL = insert(rootAVL, val);
            }
        });
        int heightAVL = getTreeHeight(rootAVL);
        cout << "AVL: Thoi gian: " << timeAVL << " ms | So so sanh: " << _global_compare_count 
             << " | Chieu cao h: " << heightAVL << endl;

        //đo RB
        RedBlackTree rbTree;
        _global_compare_count = 0;
        double timeRB = measureExecutionTime([&]() {
            for (int val : randData) rbTree.insert(val);
        });
        int heightRB = rbTree.getHeight();
        cout << "[Red-Black] Time: " << timeRB << " ms | Compare count: " << _global_compare_count 
             << " | Height h: " << heightRB << endl;
       
        
         //giải phòng
        destroy(rootBST);
        destroy(rootAVL);
    }

    return 0;
}