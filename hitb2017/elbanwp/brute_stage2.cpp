// brute force the decryption key for stage2
//
// we know that the sum of all tweaks is 15, so there are only ~150M different
// possible keys.
#include <bits/stdc++.h>
using namespace std;

unsigned char key[] = {
    0xdf, 0xf1, 0xa2, 0x5b, 0xfa, 0x4c, 0x26, 0x74,
    0xe5, 0xdf, 0x1a, 0xb6, 0x5e, 0x62, 0x85, 0x4d,
};

unsigned char input[128] = {
    0x68, 0x5d, 0xf9, 0xd0, 0xc4, 0x10, 0x39, 0xd6,
    0xa6, 0xcf, 0xac, 0x96, 0xc0, 0xc5, 0xe8, 0x45,
    0xf2, 0xcc, 0x3a, 0xea, 0x8f, 0xe4, 0x01, 0x19,
    0xb4, 0xec, 0x5f, 0x04, 0xc4, 0x00, 0x0b, 0xb8,
    0x65, 0xba, 0x0d, 0x5e, 0x7a, 0xba, 0x77, 0x6b,
    0xd9, 0xc7, 0x10, 0xb0, 0xbd, 0x8b, 0x45, 0x8e,
    0x04, 0xce, 0x45, 0x16, 0xc8, 0x04, 0xe0, 0x6d,
    0x8d, 0x24, 0x7a, 0x0b, 0xc0, 0x94, 0xc8, 0xbe,
    0x1b, 0x08, 0x52, 0x68, 0x47, 0xdc, 0xa9, 0xe1,
    0x2f, 0xe5, 0x60, 0x26, 0xbf, 0xbc, 0xbf, 0x88,
    0xaf, 0xb6, 0x64, 0xfe, 0x9c, 0x60, 0x7f, 0x29,
    0x84, 0x06, 0x21, 0xd3, 0xae, 0x99, 0xa0, 0x45,
    0x08, 0xf7, 0xc6, 0xbe, 0x0a, 0xbd, 0x24, 0x58,
    0xe5, 0x69, 0xa1, 0xd7, 0x04, 0x47, 0x6e, 0x42,
    0x8b, 0x29, 0xa0, 0xb8, 0xe7, 0xd6, 0x35, 0x77,
    0x0f, 0x32, 0x65, 0x6f, 0x0e, 0xbf, 0x6e, 0xeb,
};
unsigned char encrypted[128] = {
};

void encrypt_block(uint32_t* state) {
    uint32_t* k = (uint32_t*)key;
    uint32_t x = 0x5ba322e0;
    for (int round = 0; round < 0x20; ++round) {
        uint32_t a = ((state[0] << 4) + k[2]) ^ (state[0] + x) ^ ((state[0] >> 5) + k[3]);
        state[1] -= a;
        uint32_t b = ((state[1] << 4) + k[0]) ^ (state[1] + x) ^ ((state[1] >> 5) + k[1]);
        state[0] -= b;
        x -= 0xbadd1917;
    }
}

void encrypt(size_t size) {
    memcpy(encrypted, input, size);
    for (int i = 0; i < size; i += 8)
        encrypt_block((uint32_t*)(encrypted + i));
}

bool check() {
    encrypt(32);
    for (int i = 0; i < 32; ++i) {
        if (encrypted[i] < 20 || encrypted[i] > 127)
            return 0;
    }
    return 1;
}

size_t total = 0;
void dfs(int i, int rest) {
    if (i == 16) {
        if (check()) {
            cout << "key found: ";
            int s= 0;
            for (int j = 0; j < 16; ++j) cout << (int)key[j] << " ", s += key[j];
            cout << endl;
            assert(s +0xf73b == 0xffff);

            encrypt(128);
            cout << "encrypted: " << encrypted << endl;
        }
        total++;
        if (total % 10000000 == 0)
            cout << total << " / "  << total << endl;
        return;
    }
    for (int j = (i==15)?rest:0; j <= rest; ++j) {
        key[i]-=j;
        dfs(i+1, rest-j);
        key[i]+=j;
    }
}

int main() {
    //key[0] -= 15;
    //encrypt();
    //for (int i = 0; i < 8; ++i)
        //cout << hex << (int)encrypted[i] << " ";
    //cout << endl;
    //return 0;
    dfs(0, 15);
    cout << total << endl;
}
