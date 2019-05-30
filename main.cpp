#include "SearchTree.h"

int main()
{
    SearchTree<int> st;

    st.insert(5);
    st.insert(1);
    st.insert(2);
    st.insert(3);
    st.insert(2);
    st.insert(6);

    // st.PrintInorder();

    SearchTree<int> st1(st);
    // st1.PrintPostorder(); 

    SearchTree<int> st2;
    st2 = st1;

    // st2.clear();
    st2.insert(-1000);
    //st2.PrintPreorder();
    //st2.remove(-1000);
    st2.PrintPreorder();
    // st2.print();

    return 0;
}
