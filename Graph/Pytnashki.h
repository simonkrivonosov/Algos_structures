#ifndef PYTNASHKI_H
#define PYTNASHKI_H
typedef int TokenMovePosition;
typedef std::vector<TokenMovePosition> TokenFree;

class PlayBoard:public std::vector<int>
{
public:
 int index_0;
 PlayBoard()
 {
    reserve(16);
 }

 const_iterator Find(int k) const
 {
      const_iterator i = begin();
      for(;i != end();i++)
      {
         if(*i == k)
             return i;
      }
      return i;
 }
 void Reset()
 {
      index_0 = 15;
      assign(16,0);
      for(size_t i = 0;i < size() - 1;i++)
      {
        at(i) = i + 1;
      }
      at(size() - 1) = 0;
 }

 int Heur_calc(const PlayBoard & goal) const
 {
  /*int err = 0;
  for(int i = 0;i < 16;i++)
  {
        err += at(i) != goal.at(i);
  } *///another evristica when we count quantity of error positions

   //manhattan distance
  int err = 0;
  for(int i = 0;i < 16;i++)
  {
       int goalpos = std::distance(goal.begin(),goal.Find(at(i)));
       int cols = std::abs(goalpos % 4 - i % 4);
       int rows = std::abs(goalpos / 4 - i / 4);
       err += cols + rows;
  }
  return err;
 }
  bool Goal(const PlayBoard & goal) const
 {
    if(goal.size() != 16 || size() != 16)
        throw Exception("Wrong board");
    for(int i = 0; i < 16; ++i)
        if(at(i) != goal.at(i))
            return false;
    return true;
 }
// void Random()
// {
//  clear();
//  int tokens[]={2,11,14,5, 9,7,16,4, 13,12,1,3, 8,10,15,6};assign(&tokens[0],&tokens[sizeof(tokens)/sizeof(int)]);
//  //int tokens[]={1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};assign(&tokens[0],&tokens[sizeof(tokens)/sizeof(int)]);
//  //int tokens[]={1,2,3,4, 5,6,7,8, 9,10,11,16, 13,14,15,12};assign(&tokens[0],&tokens[sizeof(tokens)/sizeof(int)]);
//  //int tokens[]={1,2,3,4, 5,6,7,8, 9,10,16,11, 13,14,15,12};assign(&tokens[0],&tokens[sizeof(tokens)/sizeof(int)]);
// }
 PlayBoard Step(TokenMovePosition to) const
 {
      PlayBoard tmp;
      tmp.assign(begin(),end());
      iterator iter = std::find(tmp.begin(),tmp.end(),0);
      if(iter == tmp.end())
            throw Exception("0 not found");
      std::swap(tmp.at(to),*iter);
      tmp.index_0 = to;
      return tmp;
 }

 TokenFree GetTokenFree() const
 {
      const_iterator i = std::find(begin(),end(),0);
      if(i == end())
            throw Exception("0 not found");
      TokenFree tf;
      tf.reserve(4);
      size_t pos = std::distance(begin(),i);
      // move up
      if(pos > 3)
        tf.push_back(pos - 4);
      // move down
      if(pos < 12)
        tf.push_back(pos + 4);
      // move left
      if(pos % 4 != 0)
        tf.push_back(pos - 1);
      // move right
      if((pos + 1) % 4 != 0)
        tf.push_back(pos + 1);
      return tf;
 }
 bool operator < (const PlayBoard & t) const
 {
      if(t.size() != 16 || size() != 16)
          throw Exception("Wrong board");
      for(size_t i = 0;i < t.size();i++)
      {
          if(at(i) > t.at(i))
              return false;
          if(at(i) < t.at(i))
              return true;
      }
      return false;
}
};
std::ostream & operator << (std::ostream &stream, const PlayBoard & t)
{
    for(size_t i = 0;i < t.size();i++)
    {
        if(i % 4 == 0)
            stream << std::endl;
        stream.width(3);
        if(t.at(i) == 0)
            stream << " ";
        else
            stream << t.at(i);
     }
     return stream;
}
std::istream & operator >> (std::istream &stream,PlayBoard & t)
{
    t.assign(16,0);
    for(size_t i = 0; i < t.size();++i)
    {
       stream >> t[i];
       if(t[i] == 0)
           t.index_0 = i;
        if(t.at(i) > 16 || t.at(i) < 0)
            throw Exception("Wrong board");
    }
    return stream;
}

struct Vertex
{
     const PlayBoard* token;
     const Vertex* parent;
     int d;
     Vertex(const PlayBoard* t,const Vertex* p,int d):token(t),parent(p),d(d){}
     Vertex(const PlayBoard* t):token(t){}

     bool operator < (const Vertex & v) const
     {
        return token < v.token;
     }
};

class Vertex_Queue:public std::multimap<int,const Vertex*>
{
     std::map<const Vertex*,std::multimap<int,const Vertex*>::iterator> m_index;
 public:
     typedef std::multimap<int,const Vertex*> F2V;
     typedef std::map<const Vertex*,std::multimap<int,const Vertex*>::iterator> V2F;

 F2V::iterator push(int f,const Vertex* i)
 {
      F2V::iterator p = F2V::insert(std::make_pair(f,i));
      m_index.insert(std::make_pair(i,p));
      return p;
 }
 const Vertex* pop()
 {
      F2V::iterator i = F2V::begin();
      const Vertex * tmp = i->second;
      F2V::erase(i);
      m_index.erase(tmp);
      return tmp;
 }

 F2V::iterator find(const Vertex* tmp)
 {
      V2F::iterator i = m_index.find(tmp);
      if(i == m_index.end())
          return F2V::end();
      else
          return i->second;
 }
};

class GraphExtended:public std::set<PlayBoard>
{
    PlayBoard m_goal;
public:
 GraphExtended(){}
 std::set<PlayBoard>::iterator AddVertex(const PlayBoard &tp)
 {
      std::set<PlayBoard>::iterator its = insert(tp).first;
      return its;
 }

 void SetGoal(const PlayBoard &tp)
 {
    m_goal = tp;
 }

 std::vector<const PlayBoard*> Childrens(const PlayBoard* t)
 {
      std::vector<const PlayBoard*> te;
      TokenFree tf = t->GetTokenFree();
      for(TokenFree::iterator i = tf.begin();i != tf.end();i++)
      {
           PlayBoard tp = t->Step(*i);
           std::set<PlayBoard>::const_iterator its = insert(tp).first;
           te.push_back(&*its);
           //te.push_back(&tp); bad because of temporary object reference
      }
      return te;
 }

 int Heuristic(const PlayBoard* i)
 {
      if(m_goal.empty())
        throw Exception("no goal");
      return i->Heur_calc(m_goal);
 }

 bool CheckGoal(const PlayBoard* i)
 {
      if(m_goal.empty())
        throw Exception("no goal");
      return i->Goal(m_goal);
 }
};

class AStarSearch:public GraphExtended,Vertex_Queue,public std::set<Vertex>
{
public:
 const Vertex* AddVertex(const PlayBoard & tp)
 {
      std::set<PlayBoard>::iterator i = GraphExtended::AddVertex(tp);
      std::set<Vertex>::iterator t = std::set<Vertex>::insert(Vertex(&*i,nullptr,0)).first;
      return &*t;
 }

 const Vertex* Search(const Vertex* s)
 {
      Vertex_Queue::push(Heuristic(s->token),s);

      if(CheckGoal(s->token))
        return s;

      while(!Vertex_Queue::empty())
      {
           const Vertex* p = Vertex_Queue::pop();
           std::vector<const PlayBoard*> te = Childrens(p->token);
           for(std::vector<const PlayBoard*>::iterator i = te.begin();i != te.end();i++)
           {
                const int w = 1; // w(p,t)

                Vertex_Queue::iterator iter;
                std::set<Vertex>::iterator t = std::set<Vertex>::find(Vertex(*i));
                if(t == std::set<Vertex>::end()) // same as Vertex_Queue::find(t)==end()
                {
                     t = std::set<Vertex>::insert(Vertex(*i,&*p,p->d + w)).first;
                     iter = Vertex_Queue::push(t->d + Heuristic(t->token),&*t);
                     if(CheckGoal(t->token))
                        return &*t;
                }
                else
                {
                     if(p->d + w < t->d)
                     {
                          int d = p->d + w;
                          Vertex_Queue::iterator iter = Vertex_Queue::find(&*t);
                          if(iter != Vertex_Queue::end())
                            Vertex_Queue::erase(iter);
                          iter = Vertex_Queue::push(d + Heuristic(t->token),&*t);
                      }
                }
           }
      }
      throw Exception("Impossible to find a solution");
 }
};

#endif // PYTNASHKI_H
