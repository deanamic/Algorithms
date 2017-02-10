#include <bits/stdc++.h>
using namespace std;

/*
 *  Sparse table is an algorithm which uses a table to store values of the answer
 *  to the query in intervals of length 2^i for 0 <= i <= MAXLOG
 *  It is specially useful for RMQ (O(nlogn) preprocess and O(1) queries)
 *  or LCA. This code will use the RMQ algorithm to process the range minimum query
 *  ! Sparse table doesnt allow for modifications
 */

struct SparseTable {
    vector<vector<int> > ST;
    vector<int > P;
    int N;
    int MAXLOG = 0;
    void build(int n, const vector<int>& v){
        N = n;
        while ((1 << MAXLOG) <= N) ++MAXLOG;
        ST = vector<vector<int> > (N, vector<int> (MAXLOG));
        P = vector<int> (N+1);

        int LOG = 0;
        for(int i = 1; i < N + 1; ++i){
            P[i] = ((1<<LOG) > i ?  LOG-1 : ++LOG-1);
        }
        // Dynamic Sparse table building
        for (int j = 0; j < MAXLOG; ++j) {
            for (int i = 0; i < N; ++i) {
                if (i + (1 << j) - 1 < N)
                    ST[i][j] = (j ? min(ST[i][j - 1], ST[i + (1 << (j - 1))][j - 1]) : v[i]);
            }
        }
        // ST[i][j] is the minimum of a interval of length 2^j
        // intervals are from [i, i+2^j)
        // We will build the segments by length
        // building a segment is easy, to have ST[i][j]
        // we simply take segments [i , i + 2^j-1) and [i + 2^j-1, i + 2^j)
    }

// query range [l,r]
    int query(int l, int r){
        int LOG = P[r-l+1];

        // we need an interval range such the we can cover [l,r] with
        // [l , l + 2^LOG) and [r - 2^LOG + 1, r+1)
        // hence we need the maximum LOG, such that 2^LOG is smaller than
        // the range or [l,r]
        // Which has been preprocessed before
        return min(ST[l][LOG], ST[r - (1 << LOG) + 1][LOG]);

    }
};

int main(){
    int n;

    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    SparseTable S;
    S.build(n, v);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        cout << S.query(x, y) << endl;
    }

}
