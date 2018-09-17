#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#endif // BINOMIAL_HEAP_H
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <map>
#include <climits>
using std::vector;
template <typename V, typename E>
class DComporator
{
public:
    bool operator()(std::pair<V,E> x, std::pair<V,E> y)
    {
        return x.second > y.second;
    }
};
class Exception:public std::exception
{
    std::string str;
    public:
     Exception(const char * p):str(p){}
     ~Exception() throw(){}
     const char* what() const throw()
     {
        return str.c_str();
     }
};

template<typename T, class container , class compare> class BinomialHeap
{
public:
    struct Node
    {
        Node(const T & val) : value(val){}
        ~Node(){}
        T value;
        //int degree;
        std::shared_ptr <Node> parent;
        std::shared_ptr <Node> leftChild;
        std::shared_ptr <Node> rightSibling;


        std::shared_ptr <Node> merge(std::shared_ptr<Node> other) {
            Node* head_tmp = compare()(this->value , other->value) ? this : other.get();
            Node* tail_tmp = compare()(this->value , other->value) ? other.get() : this;
            std::shared_ptr<Node> head = std::make_shared<Node>(*head_tmp);
            std::shared_ptr<Node> tail = std::make_shared<Node>(*tail_tmp);
            if (!head->leftChild) {
                head->leftChild = tail;
                tail->parent = tail;
            }
                /*else {
                std::shared_ptr <Node> leftChild = head->leftChild;
                while (leftChild->rightSibling) {
                    leftChild = leftChild->rightSibling;
                }
                leftChild->rightSibling = tail;
            }*/
            else {
                std::shared_ptr <Node> left = head->leftChild;
                head->leftChild = tail;
                tail->rightSibling = left;
                tail->parent = head;
            }
            return head;
        }
    };
    std::shared_ptr<Node> tree(size_t i) {
        if (i >= trees.size()) {
            return nullptr;
        } else {
            return trees[i];
        }
    }
    T top() const
    {
        if (empty()) {
            throw Exception("Heap is empty");
        }
        return trees[min_key]->value;

    }
    void push(const T & val) {
        std::shared_ptr<Node> node(new Node(val));
        std::shared_ptr<BinomialHeap> heap (new BinomialHeap());
        indexes[val] = node;
        heap->trees.push_back(node);
        merge(heap);
    }
    void merge(std::shared_ptr<BinomialHeap> other) {
        if (!other) {
            throw Exception("BinomialHeap is empty");
        }
        int size = std::max(trees.size(), other->trees.size());
        trees.resize(size);
        std::shared_ptr<Node> carry;
        for (int i = 0; i < size; ++i) {
            size_t count = 3;
            std::shared_ptr<Node> operands[3] = {tree(i), other->tree(i), carry};
            carry.reset();
            for (size_t cur = 0; cur < count;) {
                if (operands[cur] == nullptr) {
                    operands[cur] = operands[count-1];
                    --count;
                }
                else {
                    ++cur;
                }
            }
            if (count == 1) {
                trees[i] = operands[0];
            }
            else if (count >= 2) {
                carry = operands[0]->merge(operands[1]);
                trees[i] = (count == 3 ? operands[2] : nullptr);
            }
        }
        if (carry != nullptr) {
            trees.push_back(carry);
        }
       T m;
       for (size_t i = 0 ; i < trees.size();++i) {
           if(tree(i) != nullptr)
           {
               m = tree(i)->value;
               m = std::min(tree(i)->value, m,compare());
               if(m == tree(i)->value)
                   min_key = i;
           }
       }
    }
    bool empty() const {
        if (trees.size() == 0) {
            return true;
        }
        for (std::shared_ptr<Node> tree : trees) {
            if (tree != nullptr) {
                return false;
            }
        }
        return true;
    }
    T pop() {
        if (empty()) {
            throw Exception("Heap is empty");
    }
    size_t min_index = 0;
    T m;
    for (size_t i = 0 ; i < trees.size();++i) {
        if(tree(i) != nullptr)
        {
            m = tree(i)->value;
            m = std::min(tree(i)->value, m,compare()); //TODO
            if(m == tree(i)->value)
                min_index = i;
        }
    }
      std::shared_ptr <Node> node = trees[min_index];
      T min = node->value;
      indexes.erase(min);
    if (node->leftChild != nullptr) {
        std::shared_ptr<BinomialHeap> new_heap (new BinomialHeap());
        std::shared_ptr<Node> child = node->leftChild;
        while (child) {
            new_heap->trees.push_back(child);
            child = child->rightSibling;
        }
        std::reverse(new_heap->trees.begin(),new_heap->trees.end());
        trees[min_index] = nullptr;
        merge(new_heap);
    }
    else {
        trees[min_index] = nullptr;
    }
    return min;
 }
    void decreaseKey(std::shared_ptr<Node>  from , T & to)
    {
        if(compare()(to , from->value) || indexes.find(from->value)->first)
            return ;
        std::shared_ptr<Node> tmp = from;
        indexes.erase(from->value);
        indexes[to] = tmp;
        std::shared_ptr<Node> parent = tmp->parent;
        while (parent != nullptr && to < parent->value)
        {
            std::swap(to, parent->value);
            parent = parent->parent;
        }
    }
    void Delete(const T & key)
    {
        int a = INT_MIN;
        std::shared_ptr<Node> tmp = indexes[key];
        decreaseKey(tmp,a);
        pop();
    }
     BinomialHeap() {}
    ~BinomialHeap() {}
private:
    std::map<T,std::shared_ptr<Node>> indexes;
    std::vector<std::shared_ptr<Node>> trees;
    int min_key;
};


