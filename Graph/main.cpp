#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <functional>
#include <exception>
#include <string>
#include <stack>
#include <deque>
#include <iterator>
#include <cstdio>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <climits>
#include <queue>
#include "BInomialheap.h"
#include "DSU.h"
#include "Pytnashki.h"
enum {WHITE, GRAY, BLACK};
class Comporator
{
public:
    bool operator() (std::set<int> a, std::set<int>  b)
    {
        if (a.size() != b.size())
            return a.size() < b.size();
        else
            return *(a.begin()) < *(b.begin());
    }
};
template <class V,class E>
class Graph
{
public:
    Graph() {
        _oriented = true;
    }
    ~Graph() {}
    class Edge
    {
    public:
        Edge(){}
        ~Edge(){}
        Edge(int from,int to,E value)
        {
            _from = from;
            _to = to;
            _value = value;
        }
        void Set_value(E value)
        {
            _value = value;
        }
        void Set_from(int from)
        {
            _from = from;
        }
        void Set_to(int to)
        {
            _to = to;
        }
        size_t Get_from() const
        {
            return _from;
        }
        size_t Get_to() const
        {
            return _to;
        }
        E Get_value() const
        {
            return _value;
        }
        Edge Reverse()
        {
            Edge reverse;
            reverse.Set_from(_to);
            reverse.Set_to(_from);
            reverse.Set_value(_value);
            return reverse;
        }
        Edge & operator=(const Edge  & edge)
        {
            _from = edge.Get_from();
            _to = edge.Get_to();
            _value = edge.Get_value();
            return *this;
        }
    private:
        size_t _from;
        size_t _to;
        E _value;
    };
    class Vertex
    {
    public:
        Vertex()
        {
            _colour = WHITE;
        }
        Vertex(int number,V value)
        {
            _value = value;
            _number = number;
            _colour = WHITE;
        }
        ~Vertex(){}
        void Set_value(V value)
        {
            _value = value;
        }
        void Set_index(int index)
        {
            _index = index;
        }
        void Set_number(int number)
        {
            _number = number;
        }
        int Get_number()
        {
            return _number;
        }
        V Get_value()
        {
            return _value;
        }
        int Get_index()
        {
            return _index;
        }
        void Decrease_index()
        {
            --_index;
        }
        friend bool operator == (const Vertex & vert1,const Vertex & vert2)
        {
            if(vert1._number != vert2._number) return false;
            if(vert1._value != vert2._value) return false;
            return true;
        }
        friend bool operator != (const Vertex & vert1,const Vertex & vert2)
        {
            return !(vert1 == vert2);
        }
        Vertex & operator =(const Vertex & vert1)
        {
            _index = vert1._index;
            _value = vert1._value;
            _number = vert1._number;
            _colour = vert1._colour;
            first_time = vert1.first_time;
            second_time = vert1.second_time;
            return *this;
        }
        void Set_colour(size_t colour)
        {
            _colour = colour;
        }
        size_t Get_colour()
        {
            return _colour;
        }
        int first_time;
        int second_time;
    private:
        int _index;
        V _value;
        int _number;
        size_t _colour;
    };

    class BFS_iterator
    {
    public:
        BFS_iterator(const Graph<V,E> & graph)
        {
            _graph = graph;
        }
        BFS_iterator(int id,const Graph<V,E> & graph)
        {
            _graph = graph;
            _data =  _graph._vertices_list.at(id);
            _deq.push_back(_data);
        }
        ~BFS_iterator(){}
        Vertex * operator ->()
        {
            return &_data;
        }
        friend bool operator ==(const BFS_iterator & iter1,const BFS_iterator & iter2)
        {
            if(iter1._deq.empty() && iter2._deq.empty()) return true;
            if (iter1._data != iter2._data) return false;
            if (iter1._deq.back() != iter2._deq.back()) return false;
            return true;
        }
        bool operator !=(const  BFS_iterator & iter)
        {
            return !(*this==iter);
        }
        BFS_iterator & operator=(const BFS_iterator & iter)
        {
            _data = iter._data;
            _deq = iter._deq;
            _index = iter._index;
            _size = iter._size;
            _iteration = iter._iteration;
            _graph = iter._graph;
        }

        BFS_iterator & operator ++()
        {
            if(_iteration == _size)
            {
                if(_deq.empty())
                    return *this;
                _iteration = 0;
                _size = _graph._edges_list[_deq.front().Get_index()].size();
            }
            if(_iteration == 0)
            {
                _index = _deq.front().Get_index();
                _graph._vertices_list[_deq.front().Get_number()].Set_colour(BLACK);
                _data = _graph._vertices_list[_deq.front().Get_number()];
                _deq.pop_front();
            }
            if(_iteration < _size)
            {
                if(_graph._vertices_list[_graph->_edges_list[_index][_iteration].Get_to()].Get_colour() == WHITE)
                {
                    _graph._vertices_list[_graph->_edges_list[_index][_iteration].Get_to()].Set_colour(GRAY);
                    _data = _graph._vertices_list[_graph->_edges_list[_index][_iteration].Get_to()];
                    _iteration++;
                    _deq.push_back(_data);
                }
                else
                {
                    _iteration++;
                    ++*this;
                }
            }
            if( _data.Get_colour() == BLACK && !_deq.empty())
                ++*this;
            return *this;
        }
        const BFS_iterator operator ++(int)
        {
            BFS_iterator iter;
            iter = *this;
            ++*this;
            return iter;
        }
        void Set_iteration(size_t it)
        {
            _iteration = it;
        }
        void Set_size(size_t size)
        {
            _size = size;
        }
        void Set_index(size_t index)
        {
            _index = index;
        }
        size_t Get_index()
        {
            return _index;
        }
        size_t Get_iteration()
        {
            return _iteration;
        }
        size_t Get_size()
        {
            return _size;
        }
        std::deque<Vertex>  Get_deq()
        {
            return _deq;
        }
        void Make_White()
        {
            for(typename std::map<int,std::shared_ptr<Vertex>,std::less<int> >::iterator it = _graph._vertices_list.begin(); it != _graph._vertices_list.end(); ++it)
            {
                it->second->Set_colour(WHITE);
            }
        }
    private:
        size_t _index;
        Vertex  _data;
        std::deque<Vertex> _deq;
        Graph<V,E>  _graph;
        size_t _iteration;
        size_t _size;
    };

    class DFS_iterator
    {
    public:
        int strong_check;
        size_t _index;
        std::shared_ptr<Vertex>  _data;
        std::stack <std::shared_ptr<Vertex> > _st;
        Graph<V,E> _graph;
        std::vector<int> _time;//for kasarayu
        DFS_iterator(Graph<V,E> & graph)
        {
            _graph = graph;
        }
        ~DFS_iterator(){}
        void Make_White()
        {
            for(typename std::map<int,std::shared_ptr<Vertex>,std::less<int> >::iterator it = _graph._vertices_list.begin(); it != _graph._vertices_list.end(); ++it)
            {
                it->second->Set_colour(WHITE);
            }
        }
        DFS_iterator(int id, const Graph<V,E> & graph)
        {
            _graph = graph;
            _data = _graph._vertices_list.at(id);
            _st.push(_data);
        }
        std::shared_ptr<Vertex> operator->()
        {
            return _data;
        }
        friend bool operator ==(const DFS_iterator & iter1,const DFS_iterator & iter2)
        {
            if(iter1._st.empty() && iter2._st.empty()) return true;
            if (iter1._data != iter2._data) return false;
            if (iter1._st.top() != iter2._st.top()) return false;
            return true;
        }
        bool operator !=(const  DFS_iterator & iter)
        {
            return !(*this==iter);
        }
        DFS_iterator & operator=(const DFS_iterator & iter)
        {
            _data->Set_value(iter._data->Get_value());
            _st = iter._st;
            _index = iter._index;
//			_size = iter._size;
//			_iteration = iter._iteration;
            _graph = iter._graph;
        }
//		void Set_iteration(size_t it)
//		{
//			_iteration = it;
//		}
//		void Set_size(size_t size)
//		{
//			_size = size;
//		}
        void Set_index(size_t index)
        {
            _index = index;
        }
        std::shared_ptr<Vertex> & Get_data()
        {
            return _data;
        }
        size_t Get_index()
        {
            return _index;
        }
//		size_t Get_iteration()
//		{
//			return _iteration;
//		}
//		size_t Get_size()
//		{
//			return _size;
//		}
        std::stack <std::shared_ptr<Vertex> > & Get_st()
        {
            return _st;
        }
        Vertex Get_info()
        {
            return *_data;
        }
        DFS_iterator & operator ++()
        {
            static int time = 1;
            metka:
            if(_st.empty())
                return *this;
            _data = _st.top();
            if(_data->Get_colour() == BLACK)
            {
                _st.pop();
                goto metka;
            }
            _index = _data->Get_index();
            if(!_graph._edges_list[_index].empty())
            {
                for(size_t i = 0; i < _graph._edges_list[_index].size();++i)
                {
                    if(_graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]->Get_colour() == WHITE)
                    {
                        _graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]->Set_colour(GRAY);
                        _graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]->first_time = time;
                        _graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]->second_time = time;
                        std::shared_ptr <Vertex> ptr_dat(new Vertex);
                        *(ptr_dat) = *(_graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]);
                        _data = ptr_dat;
                        time++;
                        _st.push(ptr_dat);
                        return *this;
                    }
                }
                for(size_t i = 0; i < _graph._edges_list[_index].size();++i)//for tarjan
                {
                    _data->second_time = std::min(_graph._vertices_list[_graph._edges_list[_index][i]->Get_to()]->second_time, _data->second_time);
                }
                _graph._vertices_list[_data->Get_number()]->second_time = _data->second_time;
                if(_data->first_time == _data->second_time)
                    strong_check = 1;
                _st.pop();
                _graph._vertices_list[_data->Get_number()]->Set_colour(BLACK);
                _time.push_back(_data->Get_number());//for kasarayu
                goto metka;
            }
            else
            {
                if(_data->first_time == _data->second_time)
                    strong_check = 1;
                _graph._vertices_list[_data->Get_number()]->Set_colour(BLACK);
                _time.push_back(_data->Get_number());// for kasarayu
                _st.pop();
                goto metka;
            }
            return *this;
        }
    };
    friend std::istream & operator>>(std::istream & stream, Graph<V,E> & member)
    {
        int N,M;
        stream >> N >> M;
        member._edges_list.resize(M);
        int index = 0;
        for(int i = 0; i < M; ++i)
        {
//            std::vector<std::shared_ptr<Edge> >sub_vector;
//            Vertex vert;
//            Edge edg;
            int from , to;
            E weight;
            stream >> from >> to >> weight;
            typename std::pair<typename std::map<int,std::shared_ptr<Vertex>,std::less<int> >::iterator,bool> it;
            std::shared_ptr<Vertex> elemv(new Vertex);
            std::shared_ptr<Edge> eleme(new Edge);
            elemv->Set_index(index);
            elemv->Set_number(from);
            eleme->Set_value(weight);
            eleme->Set_from(from);
            eleme->Set_to(to);
            it = member._vertices_list.insert(std::make_pair(from,std::move(elemv)));
                if (it.second == true)
                    index++;
            member._edges_list[member._vertices_list[from]->Get_index()].push_back(std::move(eleme));
            std::shared_ptr<Vertex> elemv1(new Vertex);
            elemv1->Set_index(index);
            elemv1->Set_number(to);
            typename std::pair<typename std::map<int,std::shared_ptr<Vertex>,std::less<int> >::iterator,bool> iter;
            iter = member._vertices_list.insert(std::make_pair(to,std::move(elemv1)));
                if (iter.second == true)
                    index++;
                if(member._oriented == false)
                {
                    std::shared_ptr<Edge> eleme2(new Edge);
                    eleme2->Set_value(weight);
                    eleme2->Set_from(to);
                    eleme2->Set_to(from);
                    member._edges_list[member._vertices_list[to]->Get_index()].push_back(std::move(eleme2));
                }
        }
        return stream;
    }
    friend std::ostream & operator<<(std::ostream & stream,Graph & member)
    {
        auto it = member._vertices_list.begin();
        for(auto it = member._vertices_list.begin(); it != member._vertices_list.end(); ++it)
        {
            std::cout <<std:: endl<<" vertex  with id = " <<it->second->Get_number();
            std::cout <<" edges : "<<std::endl ;
            for (size_t j = 0; j < member._edges_list[it->second->Get_index()].size(); ++j)
            {
                std::cout << member._edges_list[it->second->Get_index()][j]->Get_to() << "(weight of edge =  "<<member._edges_list[it->second->Get_index()][j]->Get_value() <<")  ";
            }
        }
        return stream;
    }
    void Transpose()
    {
        std::vector<std::vector<std::shared_ptr<Edge> > > sub_vector(_edges_list.size());
        for(size_t i = 0; i < _edges_list.size(); ++i)
        {
            for(size_t j = 0 ; j < _edges_list[i].size(); ++j)
            {
                std::shared_ptr<Edge> ptr(new Edge);
                *ptr = _edges_list[i][j]->Reverse();
                sub_vector[_vertices_list[_edges_list[i][j]->Get_to()]->Get_index()].push_back(ptr);
            }
        }
         _edges_list = sub_vector;
    }
    DFS_iterator  DFS_begin()
    {
        int index = _edges_list[0][0]->Get_from();
        DFS_iterator *iter = new DFS_iterator(index,*this);
        if(iter->_graph._vertices_list[index]->Get_colour() == WHITE)
            iter->_graph._vertices_list[index]->Set_colour(GRAY);
        iter->Set_index(index);
        return *iter;
    }
    DFS_iterator  DFS_begin(size_t id)
    {
        DFS_iterator *iter = new DFS_iterator(id,*this);
        if(iter->_graph._vertices_list[id]->Get_colour() == WHITE)
            iter->_graph._vertices_list[id]->Set_colour(GRAY);
        int index = _vertices_list[id]->Get_index();
        iter->Set_index(index);
        //iter->strong_check = false;
        return *iter;
    }
    DFS_iterator DFS_end()
    {
        DFS_iterator *iter = new DFS_iterator(*this);
        //iter->Make_White();
        return *iter;
    }
    BFS_iterator  BFS_begin()
    {
        int index = _edges_list[0][0]->Get_from();
        BFS_iterator *iter = new BFS_iterator(index,*this);
        //iter->Make_White();
        if(iter->graph._vertices_list[index]->Get_colour() == WHITE)
            iter->graph._vertices_list[index]->Set_colour(GRAY);
        iter->Set_iteration(0);
        iter->Set_size(_edges_list[0].size());
        iter->Set_index(index);
        return *iter;
    }
    BFS_iterator  BFS_begin(size_t id)
    {
        BFS_iterator *iter = new BFS_iterator(id,*this);
        int index = _vertices_list[id]->Get_index();
        if(iter->graph._vertices_list[index]->Get_colour() == WHITE)
            iter->graph._vertices_list[index]->Set_colour(GRAY);
        iter->Set_iteration(0);
        iter->Set_size(_edges_list[0].size());
        iter->Set_index(0);
        return *iter;
    }
    BFS_iterator BFS_end()
    {
        BFS_iterator *iter = new BFS_iterator(*this);
        //iter->Make_White();
        return *iter;
    }
    Graph<V,E> & operator = (const Graph<V,E> & graph)
    {
        _edges_list = graph._edges_list;
        _vertices_list = graph._vertices_list;
        return *this;
    }
     void Kosaraju()
     {
         std::set< std::set<int,std::less<int> > , Comporator> strong_comp;
         //int count = 0;
         Graph<V,E> graf;
         graf = *this;
         graf.Transpose();
         std::vector<int> time;
         for(auto it = graf._vertices_list.begin(); it != graf._vertices_list.end(); ++it)
         {
             auto it1 = graf.DFS_begin(it->second->Get_index());
             if(it->second->Get_colour() != BLACK)
             {
                 for(; it1 != graf.DFS_end(); ++it1)
                 {}
                 time.insert(time.end(),it1._time.begin(),it1._time.end());
             }
         }
         graf.Transpose();
         graf.Make_White();
         //int number = 0;
         for(int i = time.size() -1 ; i >= 0; --i)
         {
             auto it1 = graf.DFS_begin(time[i]);
             if(it1._graph._vertices_list[time[i]]->Get_colour() != BLACK)
             {
                 std::set<int> tmp;
                 for(; it1 != graf.DFS_end(); ++it1)
                 {
                     tmp.insert(it1._data->Get_number());
                 }
                 strong_comp.insert(tmp);
             }
         }
         std::cout<<"Quantity of strong components :"<<strong_comp.size()<<std::endl;
         int i = 0;
        for(auto it = strong_comp.begin(); it != strong_comp.end(); ++it)
        {
            std::cout<<std::endl<<"the "<<i<< " component : ";
            for (auto it1 = it->begin(); it1 != it->end(); ++it1)
            {
                std::cout << *(it1)<< " ";
            }
            i++;
        }
         Make_White();
     }
     void Tarjan()
     {
         std::set< std::set<int,std::less<int> >,Comporator> strong_comp;
         std::stack<std::shared_ptr<Vertex>> st;
         for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
         {
             auto it1 = DFS_begin(it->first);
             if(_vertices_list[it->first]->Get_colour() != BLACK)
             {
                 it1._data->first_time = 0;
                 it1._data->second_time = 0;
                 for(; it1 != DFS_end() ||!st.empty(); ++it1)
                 {
                     std::set<int> tmp;
                     if(it1.strong_check == 1 || it1 == DFS_end())
                     {
                         while(!st.empty())
                         {
                             if(st.top()->first_time != st.top()->second_time)
                             {
                                 _vertices_list[st.top()->Get_number()]->second_time = INT_MAX;
                                 tmp.insert(st.top()->Get_number());
                                 st.pop();
                             }
                             else
                             {
                                 _vertices_list[st.top()->Get_number()]->second_time = INT_MAX;
                                 tmp.insert(st.top()->Get_number());
                                 st.pop();
                                 break;
                             }
                         }
                         strong_comp.insert(tmp);
                         it1.strong_check = false;
                     }
                     if(it1 != DFS_end())
                        st.push(it1._data);
                 }
             }
         }
         std::cout<<"Quantity of strong components :"<<strong_comp.size()<<std::endl;
         int i = 0;
        for(auto it = strong_comp.begin(); it != strong_comp.end(); ++it)
        {
            std::cout<<std::endl<<"the "<<i<< " component : ";
            for (auto it1 = it->begin(); it1 != it->end(); ++it1)
            {
                std::cout << *(it1)<< " ";
            }
            i++;
        }
         Make_White();
     }
    void Make_White()
    {
        for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
        {
            it->second->Set_colour(WHITE);
        }
    }
    template<template<typename N, class container , class compare> class Container> void Dijkstra (int numb)
    {
        Container<std::pair<V,E>,std::vector<std::pair<V,E>>,DComporator<int,int>> queue;
        std::vector<E> d(_vertices_list.size()+1,INT_MAX);
        std::vector<bool>used (_vertices_list.size()+1,false);
        d[numb] = 0;
        queue.push(std::make_pair(numb,0));
        while(!queue.empty())
        {
            std::pair<V,E> current =queue.top();
            queue.pop();
            if(used[current.first])
                continue;
            used[current.first] = true;
            for (size_t i = 0; i < _edges_list[_vertices_list[current.first]->Get_index()].size();++i)
            {
                if(used[_edges_list[_vertices_list[current.first]->Get_index()][i]->Get_to()])
                    continue;
                if(d[_edges_list[_vertices_list[current.first]->Get_index()][i]->Get_to()] > d[current.first]
                        + _edges_list[_vertices_list[current.first]->Get_index()][i]->Get_value())
                {
                    d[_edges_list[_vertices_list[current.first]->Get_index()][i]->Get_to()] = d[current.first]
                            + _edges_list[_vertices_list[current.first]->Get_index()][i]->Get_value();
                }
                queue.push(std::make_pair(_edges_list[_vertices_list[current.first]->Get_index()][i]->Get_to(),d[current.first]
                        + _edges_list[_vertices_list[current.first]->Get_index()][i]->Get_value()));
            }

        }
        for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
        {
            std::cout<<"distance to "<<it->first<<" vertex is ";
            if(d[it->first] == INT_MAX)
                std::cout<<" -1"<<std::endl;
            else
                std::cout<<d[it->first]<<std::endl;
        }
    }
    E Kruskal()
    {
        std::vector<std::pair<E,std::pair<int,int>>> edges_order;
        int a;
        std::pair<int,int> tmp;
        DSU dsu(_vertices_list.size());
        E result = 0;
        for(size_t i = 0; i < _edges_list.size(); ++i)
        {
            if(!_edges_list[i].empty())
                 a = _edges_list[i][0]->Get_from();
            for(size_t j = 0 ; j < _edges_list[i].size(); ++j)
            {
                tmp = std::make_pair(a,_edges_list[i][j]->Get_to());
                edges_order.push_back(std::make_pair(_edges_list[i][j]->Get_value(),tmp));
            }
        }
        std::sort(edges_order.begin(),edges_order.end());
        for(size_t i = 0; i < _vertices_list.size(); ++i)
        {
            dsu.make_set(i);
        }
        for(size_t i = 0; i < edges_order.size(); ++i)
        {
            int v1 = edges_order[i].second.first,
            v2 = edges_order[i].second.second,
            weight = edges_order[i].first;
                if (dsu.find_set(v1) != dsu.find_set(v2)) {
                    result += weight;
                    dsu.union_sets(v1,v2);
                }
        }
        return result;
    }

    void Bellman_Ford(int numb)
    {
        std::unordered_map<V,E> distance;
        std::unordered_map<V,E> parent;
        for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
            distance[it->first] = INT_MAX;
        distance[numb] = 0;
       // V cycle_vert;
        bool change;
        for(size_t i = 0 ; i < _vertices_list.size();i++)
        {
            change = false;
            for(size_t j = 0; j < _edges_list.size(); ++j)
            {
                for(size_t k = 0; k < _edges_list[j].size(); ++k)
                {
                    if( distance[_edges_list[j][k]->Get_from()] < INT_MAX)
                    {
                        if(distance[_edges_list[j][k]->Get_to()] > distance[_edges_list[j][k]->Get_from()] + _edges_list[j][k]->Get_value())
                        {
                            distance[_edges_list[j][k]->Get_to()] = std::max(-INT_MAX,distance[_edges_list[j][k]->Get_from()] + _edges_list[j][k]->Get_value());
                            parent[_edges_list[j][k]->Get_to()] = _edges_list[j][k]->Get_from();
                           // cycle_vert = _edges_list[j][k]->Get_to();
                            change = true;
                        }
                    }
                }
            }
            if(change == false)
                break;
        }
        if(change == true)
        {
           std::cout<<"-1";
           return;
        }
        for(auto it = distance.begin(); it != distance.end(); ++it)
            if(it->second == INT_MAX)
                it->second = -1;
        std::cout<<_vertices_list.size()<<std::endl;
        for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
        {
            std::cout<<"distance to "<<it->first<<" vertex is "<<distance[it->first]<<std::endl;
        }
    }
    void Floyd_Warshall()
    {
        std::vector<std::vector<E> > distance (_vertices_list.size() + 1,std::vector<E>(_vertices_list.size()+1, INT_MAX));// matrix of edges with appropriate indexes;
        for(auto it = _vertices_list.begin(); it != _vertices_list.end(); ++it)
        {
            int index = it->second->Get_index();
            for(size_t i = 0; i < _edges_list[index].size() ; ++i)
                distance[it->first][_edges_list[index][i]->Get_to()] = _edges_list[index][i]->Get_value();
        }
        for (size_t i = 0; i  < distance.size() ; ++i)
            distance[i][i] = 0;
        for (size_t k = 1; k < distance.size(); ++k)
            for (size_t i = 1; i < distance.size(); ++i)
                for (size_t j = 1; j < distance.size(); ++j)
                    if (distance[i][k] < INT_MAX && distance[k][j] < INT_MAX)
                        distance[i][j] = std::min(distance[i][j], distance[i][k] + distance[k][j]);
        for (size_t i = 1; i < distance.size(); ++i)
        {
                for (size_t j = 1; j < distance.size(); ++j)
                {
                    if(distance[i][j] == INT_MAX)
                        std::cout<<"-1 ";
                    else
                        std:: cout << distance[i][j]<<" ";
                }
                std::cout<<std::endl;
        }
    }
    void Set_oriented(bool x)
    {
        _oriented = x;
    }

private:
    std::vector <std::vector<std::shared_ptr<Edge> > > _edges_list;
    std::map <int,std::shared_ptr<Vertex>,std::less<int> > _vertices_list;
    bool _oriented;
};
int main()
{
    Graph<int,int> x;
    int n;
    std::cin>>n;
    switch (n) {
    case 0:
        std::cin >> x;
        x.Tarjan();
        break;
    case 1:
        std::cin >> x;
        x.Kosaraju();
        break;
    case 2:
        std::cin >> x;
        x.Dijkstra<std::priority_queue>(1);
        break;
    case 3:
        std::cin >> x;
        x.Bellman_Ford(1);
        break;
    case 4:
        std::cin >> x;
        x.Floyd_Warshall();
        break;
    case 5:
        try
        {
              PlayBoard current;
              std::cin>>current;
                  int inv = 0;
                  for (int i = 0; i < 16; ++i)
                      if (current[i])
                          for (int j = 0; j < i; ++j)
                              if (current[j] > current[i])
                                  ++inv;
                  for (int i = 0; i < 16; ++i)
                      if (current[i] == 0)
                          inv += 1 + i / 4;
                  if (inv % 2 == 1)
                      std::cout<<-1;
                  else
                  {
                      PlayBoard goal;
                      goal.Reset();

                      std::cout<<"Current Board:"<<std::endl;
                      std::cout<<current<<std::endl;

                      AStarSearch s;
                      s.SetGoal(goal);
                      const Vertex * i = s.Search(s.AddVertex(current));

                      std::vector<const PlayBoard*> te;
                      for(;i->parent != 0;i = i->parent)
                      {
                        te.push_back(i->token);
                      }
                      std::cout<<std::endl;
                      std::cout<<te.size()<<std::endl;
                      for(std::vector<const PlayBoard*>::reverse_iterator i = te.rbegin();i != te.rend();i++)
                      {
                        std::cout<<(**i).index_0<<std::endl;
                      }
                  }
         }
         catch(std::exception & e)
         {
              std::cerr<<"Exception: "<<e.what()<<std::endl;
         }
    case 6:
        std::cin >> x;
        x.Dijkstra<BinomialHeap>(1);
        break;
    case 7:
        std::cin >> x;
        x.Set_oriented(false);
        std::cout<<x.Kruskal();
    default:
        break;
    }
//    Graph<int,int> x;
//    std::cin >> x;
//    x.Set_oriented(false);
//    std::cout<<x.Kruskal();
    //x.Dijkstra<BinomialHeap>(1);
    /*x.Tarjan();
    x.Kosaraju();*/
    //for (auto i: y)
    //	std::cout<<i;
    /*auto it = x.DFS_begin();
    ++it;
        std::cout << x;*/
    return 0;
}
