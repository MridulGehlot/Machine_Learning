#include<iostream>
#include<random>
#include<unordered_set>
using namespace std;
int main()
{
random_device rd;
uniform_int_distribution<int> d(1,100);
mt19937 engine(rd());
unordered_set<int> st;
for(int i=0;i<10;++i) st.insert(d(engine));
cout<<st.size()<<endl;
for(auto x:st) cout<<x<<endl;
return 0;
}