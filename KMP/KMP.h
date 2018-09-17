#ifndef KMP_H
#define KMP_H
#include<vector>
#include<string>
std::vector<size_t> prefixFunction (const std::string & s) {
    size_t len = s.length();
    std::vector<size_t> pi (len);
    for (size_t i = 1; i < len; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            ++j;
        pi[i] = j;
    }
    return std::move(pi);
}
std::vector<size_t> KMP(const std::string & s1 ,const std::string & s2){
   size_t s1_len = s1.size();
   size_t s2_len = s2.size();
   std::vector<size_t> result;
   std::vector<size_t> pi = prefixFunction(s1 + "#" + s2);
   for (size_t i = 0 ;i < s2_len; ++i)
      if (pi[s1_len + i + 1] == s1_len)
         result.push_back(i + 1 - s1_len);
   return std::move(result);
}
#endif // KMP_H
