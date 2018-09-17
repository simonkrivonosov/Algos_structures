
#include"Treap.h"
int main()
{
//The Iosif Flaviy task
    std::shared_ptr<Treap<int,int>>  treap1 (new Treap<int,int>(0,rand(),nullptr,nullptr));
    int n;
    int m;
    std::cout<<"Quantity of people :"<<std::endl;
    std::cin>>n;
    std::cout<<"Type the order of killing :"<<std::endl;
    std::cin>>m;
    std::cout<<std::endl<<std::endl;
    for(int i = 1; i <= n-1; ++i)
        treap1->Insert(i);
  //treap1 = treap1->Delete(5);
   // treap1->Delete(2);
    int order = (m - 1)  % treap1->Size();
    while(treap1->size != 1)
    {
        //std::cout<<treap1->Size()<<"размер";
        std::cout<<order<<std::endl;
        treap1 = treap1->Delete(treap1->KthStatistic(order));
        order = (order + m-1)  % treap1->Size();
    }
    std::cout<<"And the winner is ..."<<treap1->Get_key();
//    std::shared_ptr<Treap<int,int>>  treap2 (new Treap<int,int>(12,0,nullptr,nullptr));
//    std::shared_ptr<Treap<int,int>> x(new Treap<int,int>());
//    x->Merge(treap1,treap2);
//    treap1->Insert(0);
//    treap1->Insert(2);
//    treap1->Insert(3);
//    treap1->Delete(2);
//    treap1->Insert(5);
//    treap1->Insert(7);
    return 0;
}

