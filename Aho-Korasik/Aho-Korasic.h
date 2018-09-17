#ifndef AHOKORASIC_H
#define AHOKORASIC_H
#include <iostream>
#include <vector>
#include <string>
#define K 256
struct Vertex{
   std::vector<int> next;
   int string_num;
   bool terminal;
   int suff_link;
   char edge_from_symb;
   int parent;
   int good_suff_link;
   std::vector<int> go;
   Vertex()
   {
       terminal = false;
       next.resize(K,-1);
       go.resize(K,-1);
       string_num = -1;
       suff_link = -1;
       good_suff_link = -1;
       parent = -1;
       edge_from_symb = 0;

   }
   Vertex(int par,char c)
   {
       terminal = false;
       next.resize(K,-1);
       go.resize(K,-1);
       string_num = -1;
       suff_link = -1;
       parent = par;
       edge_from_symb = c;
       good_suff_link = -1;
   }
};
class AhoCorasik
{
public:
    AhoCorasik()
    {
        bohr.push_back(Vertex());
    }

    void add_string(const std::string & s){
       int num = 0; //begin with root
       for (size_t i = 0; i < s.length(); i++){
          char ch = s[i]-'a'; //get alphabet's number
          if (bohr[num].next[ch] == -1){ //-1 - edge doenst exist
             bohr.push_back(Vertex(num,ch));
             bohr[num].next[ch] = bohr.size()-1;
             }
          num = bohr[num].next[ch];
       }
       bohr[num].terminal = true;
       pattern.push_back(s);
       bohr[num].string_num = pattern.size() - 1;
    }
    bool in_bhor(const std::string & s){
       int num = 0;
       for (size_t i = 0; i < s.length(); i++){
          char ch = s[i]-'a';
          if (bohr[num].next[ch] == -1){
             return false;
             }
          num = bohr[num].next[ch];
       }
       return true;
    }
    int get_suff_link(int v){
       if (bohr[v].suff_link == -1) //if didnt count before
       {
          if (v == 0 || bohr[v].parent == 0) //if v - root or v - son of root
             bohr[v].suff_link = 0;
          else
             bohr[v].suff_link = go(get_suff_link(bohr[v].parent), bohr[v].edge_from_symb);
       }
       return bohr[v].suff_link;
    }

    int go(int v, char ch){
       if (bohr[v].go[ch]== -1)
       {
          if (bohr[v].next[ch] != -1)
             bohr[v].go[ch] = bohr[v].next[ch];
          else
          {
             if (v == 0)
                bohr[v].go[ch] = 0;
             else
                bohr[v].go[ch] = go(get_suff_link(v), ch);
          }
       }
       return bohr[v].go[ch];
    }
    int get_good_suff_link(int v){
       if (bohr[v].good_suff_link == -1){
          int u = get_suff_link(v);
          if (u == 0) // v - root or v.suff_link points to root
             bohr[v].good_suff_link = 0;
          else
             bohr[v].good_suff_link = (bohr[u].terminal == true) ? u : get_good_suff_link(u);
       }
       return bohr[v].good_suff_link;
    }
    void check(int v,int i){
        for(int u = v;u != 0;u = get_good_suff_link(u)){
            if (bohr[u].terminal == true)
                std::cout<<i - pattern[bohr[u].string_num].length() + 1<<" "<<pattern[bohr[u].string_num]<<std::endl;
        }
    }
    void find_all_pos(const std::string & s){
        int u = 0;
        for(size_t i = 0;i < s.length();++i){
            u = go(u,s[i]-'a');
            check(u,i + 1);
        }
    }
private:
    std::vector<Vertex> bohr;
    std::vector<std::string> pattern;
};
#endif // AHOKORASIC_H
