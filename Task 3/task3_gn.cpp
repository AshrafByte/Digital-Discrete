#include <bits/stdc++.h>
using namespace std;

int main(){
    string plaintext;
    int x ;
    int a,b;//keys
    cout<<"enter the string you want to cipher:\n";
    getline(cin, plaintext);
    cout<<"enter the keys of the cipher:\n";
    cin>>a>>b;

     if (gcd(a, 95) != 1) {
        cout << "Key 'a' must be coprime with 95.\n";
        return 1; // Exit with an error
    }

    string encrypted = plaintext;
    for(auto & character : encrypted ){

        x = character - 32;//offset the characters to start from zero
        character = ((a * x + b) % 95) + 32;//apply affine cipher 95 is length of the sequence of symbols +32 to return characters to their real values
    }

    cout<<"the encrypted string is:"<<encrypted;
}