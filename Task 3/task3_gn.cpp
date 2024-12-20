#include <bits/stdc++.h>
using namespace std;

int main(){
    string s;
    int x ;
    int a,b;//keys
    cout<<"enter the string you want to cipher:\n";
    getline(cin, s);
    cout<<"enter the keys of the cipher:\n";
    cin>>a>>b;

     if (gcd(a, 94) != 1) {
        cout << "Key 'a' must be coprime with 94.\n";
        return 1; // Exit with an error
    }

    int size = s.length();//size of the strig to cipher
    for(int i=0 ; i<size ; i++){

        x = s[i] - 33;//offset the characters to start from zero
        s[i] = ((a * x + b) % 94) + 33;//apply affine cipher 94 is length of the sequence of symbols +33 to return characters to their real values
        cout<<s[i];

    }
}