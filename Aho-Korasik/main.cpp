#include"Aho-Korasic.h"
int main()
{
    AhoCorasik x;
    x.add_string("abc");
    x.add_string("bcdc");
    x.add_string("cccb");
    x.add_string("bcdd");
    x.add_string("bbbc");
    x.find_all_pos("abcdcbcddbbbcccbbbcccbb");
//    x.add_string("ab");
//    x.add_string("bc");
//    x.find_all_pos("abc");
    return 0;
}
