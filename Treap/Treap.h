#ifndef TREAP_H
#define TREAP_H
#endif // TREAP_H
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>
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
template <typename K,typename P>
class Treap{
private:
   K x;
   P y;
   std::shared_ptr<Treap<K,P>> left, right;
public:
   int size;
   int Size() const
   {
       return size;
   }
   K Get_key() const
   {
       return x;
   }
   P Get_priority() const
   {
       return y;
   }

   Treap(){size = 1;}
   Treap(K _x, P _y,std::shared_ptr<Treap<K,P>> _left, std::shared_ptr<Treap<K,P>> _right ) :x(_x), y(_y), left(_left), right(_right)
   {
       size = 1 + (_left == nullptr ? 0 : _left->size) + (_right == nullptr ? 0 : _right->size);
   }
    std::shared_ptr<Treap<K,P>> Merge(std::shared_ptr<Treap<K,P>> & Left, std::shared_ptr<Treap<K,P>> & Right) {
       if (Left == nullptr)
       {
           return Right;
       }
       if (Right == nullptr)
       {
            return Left;
       }
       std::shared_ptr<Treap<K,P>> result(new Treap<K,P>());//(new Treap<K,P>(this->x,this->y,this->left,this->right));
       if (Left->y > Right->y) {
           std::shared_ptr<Treap<K,P>> ptr1 = ptr1->Merge(Left->right, Right);
           std::shared_ptr<Treap<K,P>> p(new Treap<K,P>(Left->x,Left->y,Left->left,ptr1));
           *result = *p;
           result->size = 1 + (result->left == nullptr ? 0 : result->left->size) + (result->right == nullptr ? 0 : result->right->size);
           //*this = p
           return result;
       }
       else {
           std::shared_ptr<Treap<K,P>> ptr2 = ptr2->Merge(Left, Right->left);
           std::shared_ptr<Treap<K,P>> p (new Treap<K,P>(Right->x,Right->y,ptr2,Right->right));
           *result = *p;
           result->size = 1 + (result->left == nullptr ? 0 : result->left->size) + (result->right == nullptr ? 0 : result->right->size);
           //*this = p;
           return result;
       }
       //this->size = 1 + (Left == nullptr ? 0 : Left->size) + (Right == nullptr ? 0 : Right->size);
   }
   void Split(K k, std::shared_ptr<Treap<K,P>> & Left, std::shared_ptr<Treap<K,P>> & Right) {
        std::shared_ptr<Treap<K,P>> result = nullptr;
        if(this == nullptr)
        {
            Left = nullptr;
            Right = nullptr;
            return;
        } // may be unnessasary
        else
        {
            if(k >= x)
            {
                if(this->right == nullptr)
                   Right = nullptr;
                else
                   this->right->Split(k,result,Right);
                if(Left == nullptr)
                {
                    std::shared_ptr<Treap<K,P>> p(new Treap<K,P>());
                    Left = p;
                }
                *Left = *this;
                Left->right = result;
                Left->size = 1 + (Left->left == nullptr ? 0 : Left->left->size) + (Left->right == nullptr ? 0 : Left->right->size);
            }
            else
            {
                if(this->left == nullptr)
                    Left = nullptr;
                else
                    this->left->Split(x,Left,result);
                if(Right == nullptr)
                {
                    std::shared_ptr<Treap<K,P>> p(new Treap<K,P>());
                    Right = p;
                }
                *Right = *this;
                Right->left = result;
                Right->size = 1 + (Right->left == nullptr ? 0 : Right->left->size) + (Right->right == nullptr ? 0 : Right->right->size);
            }
        }
    }
   K KthStatistic(int order)// begins with 0 number
   {
       std::shared_ptr<Treap<K,P>> ptr(new Treap<K,P>(this->x,this->y,this->left,this->right));
       while (ptr != nullptr)
       {
           int sizeLeft = ptr->left == nullptr ? 0 : ptr->left->size;
           if (sizeLeft == order)
               return ptr->x;
           ptr = sizeLeft > order ? ptr->left : ptr->right;
           if (sizeLeft < order)
               order -= sizeLeft + 1;
       }
       return -1;
   }
   void Insert(K a)
   {
       std::shared_ptr<Treap<K,P>> result(new Treap<K,P>(this->x,this->y,this->left,this->right));
       std::shared_ptr<Treap<K,P>> Left = nullptr;
       std::shared_ptr<Treap<K,P>> Right = nullptr;
       Split(a, Left , Right);
       int c =  rand();
       std::shared_ptr<Treap<K,P>> tmp(new Treap<K,P>(a,c,nullptr,nullptr));
       result = result->Merge(Left, tmp);
       std::shared_ptr<Treap<K,P>> t(new Treap<K,P>(result->x,result->y,result->left,result->right));
       result = result->Merge(t, Right);
       *this = *result;
   }
    std::shared_ptr<Treap<K,P>> Delete (K a)
   {

       if(x == a)
       {
           if(left == nullptr && right == nullptr)
           {
                return nullptr;
           }
           else return this->Merge(this->left,this->right);
       }
       std::shared_ptr<Treap<K,P>> result(new Treap<K,P>(this->x,this->y,this->left,this->right));
       std::shared_ptr<Treap<K,P>> elem(new Treap<K,P>(result->x,result->y,result->left,result->right));
       std::shared_ptr<Treap<K,P>> parent(new Treap<K,P>(result->x,result->y,result->left,result->right));
       char check;
       while(elem != nullptr)
       {
           if(elem->x == a)
           {
               if(elem->left == nullptr && elem->right == nullptr)
               {
                    if(check == 'l')
                        parent->left = nullptr;
                    else
                        parent->right = nullptr;
                    if(parent->x == result->x && parent->y == result->y)
                    {
                       result = parent;
                    }
                    break;
               }
                   *elem = *elem->Merge(elem->left,elem->right);
                   break;
           }
           else if(elem->x > a)
           {
               parent = elem;
               elem = elem->left;
               check = 'l';
           }
           else if(elem->x < a)
           {
               parent = elem;
               elem = elem->right;
               check = 'r';
           }
       }
           std::shared_ptr<Treap<K,P>> find(new Treap<K,P>(result->x,result->y,result->left,result->right));
           while(find->x != parent->x)
           {
               find->size--;
               if(find->x > parent->x)
                   find = find->left;
               else
                   find = find->right;
           }
           find->size--;
           //result->size--;
           *this = *result;
           result->size = 1 + (result->left == nullptr ? 0 : result->left->size) + (result->right == nullptr ? 0 : result->right->size);
           return result;
   }
   std::pair<bool,std::shared_ptr<Treap<K,P>>> Find (K a)
   {
       std::shared_ptr<Treap<K,P>> elem(new Treap<K,P>(x,y,left,right));
       while(elem != nullptr)
       {
           if(elem->x == a)
               return {true,elem};
           if(elem->x > a)
               elem = elem->left;
           else
               elem = elem->right;
       }
       return {false,nullptr};
   }
};

