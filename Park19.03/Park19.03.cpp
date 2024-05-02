#include <iostream>
#include <vector>
#include <list>

using namespace std;

template <typename T> void printcontainer(const T &cont){
    typename T::const_iterator itdefault = cont.begin();
    for (itdefault; itdefault != cont.end(); itdefault++){
        cout << *itdefault << ' ';
    }
    cout << endl;
    typename T::const_reverse_iterator itreverse = cont.rbegin();
    for (itreverse; itreverse != cont.rend(); itreverse++){
        cout << *itreverse << ' ';
    }
    cout << endl;

} 

template <typename A> void changecontainer(vector<A> &cont1, const list<A> &cont2){
    typename vector <A>:: iterator it1 = cont1.begin();
    typename list <A>:: const_iterator it2 = cont2.begin();
    int i = 0;
    int j = 0;
    int l = cont2.size();
    cout << l << endl;
    A mas[l];
    int s = 0;
    for (it2; it2 != cont2.end(); it2++){
        if (i%2 == 0){
            mas[s] = *it2;
            cout << "s = " << s << " Mas[i] =" << mas[s] << ' ';
            s++;
        }
        i++;
    }
    cout << endl << "i = " << i << endl;
    int size = 0;
    size = cont1.size();
    if (s<size){
        size = s;
    }
    cout << '\n' << size << '\n';
    int temp = 0;
    //cout << "mas[1] = " << mas[1] << endl;
    it1+=2;
    for (it1; it1 != cont1.end(); it1++){
        if (temp <= size){
            cout << "mas[temp] = " << mas[temp] << ' ';
            it1 = cont1.insert(it1, mas[temp]);
            temp++;
            it1+=2;
        }
    }
    cout << endl;
}

template <typename A> void deleteeachthird(A &cont1){
    typename A:: iterator it1 = cont1.begin();;
    it1+=2;
    for (it1; it1 != cont1.end(); it1+=2){
        it1 = cont1.erase(it1);
    }
}





int main(){
    list <int> mylist = {1,2,3,4,5};
    vector <int> myvector = {6,7,8,9,10,11};
    //list <int>::iterator itl1 = mylist.begin();
    //vector <int>::iterator itv1 = myvector.begin();
    printcontainer(mylist);
    printcontainer(myvector);
    changecontainer(myvector, mylist);
    printcontainer(mylist);
    printcontainer(myvector);
    deleteeachthird(myvector);
    printcontainer(myvector);
  //  for (itl1; itl1 != mylist.end(); itl1++){
   //     cout << *itl1 << ' ';
  //  }
   // cout << endl;
   // for (itv1; itv1 != myvector.end(); itv1++){
    //    cout << *itv1 << ' ';
    //}
    //cout << endl;

}
