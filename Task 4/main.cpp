#include <iostream>

using namespace std;

int modInverse(int a, int m) {
    for (int x = 1; x < m; ++x) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // No modular inverse exists
}

int main() {
    string s;
    cout<<"enter the string you want to decipher:\n";
    getline(cin,s);
    int size = s.length();
    int a ; // Key a
    int b ; // Key b
    cout<<"enter the keys a then b:\n";
    cin>>a>>b;
    int m = 95; // Size of the character set (ASCII 32 to 126)
    int a_inv = modInverse(a, m);

    if (a_inv == -1) {
        cerr << "Error: Modular inverse of Key A does not exist." << endl;
        return 1;
    }

    for (int i = 0; i < size; i++) {
        int y = s[i] - 32; // Map characters starting from ASCII 32 (space)
        int x = (a_inv * (y - b)) % m; // Apply the decryption formula

        // Ensure the result is in the valid range
        if (x < 0) {
            x += m;
        }
        s[i] = x + 32; // Convert back to ASCII
    }

    cout << "Deciphered Message: " << s << endl;
    return 0;
}
