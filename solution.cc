#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

const int N = (int)1e5 + 10;

int b, l, d;
int s[N];
vector< vector<int> > b2l;
vector<int> order;

bool cmp(int x, int y) {
    return s[x] > s[y];
}

struct library {
    multiset<pair<int, int> > books;
    vector<int> scanned;
    int n, t, m;
    bool used;
    
    library() {
        used = false;
    }
    
    void read(int lib_id) {
        cin >> n >> t >> m;
        for (int i = 0, id; i < n; i++) {
            cin >> id;
            b2l[id].push_back(lib_id);
            books.insert(make_pair(-s[id], id));
        }
    }
};

library lib[N];

int cost(int x, int j) {
    // return the sum of non-scanned books if we sign-up library *x* at time *j*
    library *cur = &lib[x];
    int ret = 0;
    auto it = cur -> books.begin();
    for (int days = j + cur -> t; days < d; days++) {
        int scanned_today = 0;
        while (it != cur -> books.end() && scanned_today < cur -> m) {
            ret += -it -> first;
            scanned_today++;
            it++;
        }
        if (it == cur -> books.end()) break;
    }
    return ret;
}

void delete_book(int id) {
    // delete the book id from libraries
    // b2l[id] will give you vector of libs that contains book id
    for (auto i : b2l[id]) {
        auto it = lib[i].books.find(make_pair(-s[id], id));
        lib[i].books.erase(it);
    }   
}

void delete_scanned(int x, int j) {
    // delete scanned books from library *x*
    order.push_back(x);
    library *cur = &lib[x];
    cur -> used = true;
    auto it = cur -> books.begin();
    for (int days = j + cur -> t; days < d; days++) {
        int scanned_today = 0;
        while (it != cur -> books.end() && scanned_today < cur -> m) {
            cur -> scanned.push_back(it -> second);
            scanned_today++;
            it++;
        }
        if (it == cur -> books.end()) break;
    }

    for (auto it2 : cur -> scanned) {
        delete_book(it2);
    }
}

int solve(int cur_time) {
    // find *i* with maximum cost(i, cur_time)
    // delete library *i* and all scanned books
    // and then call solve(cur_time+t[i])
    int max_lib = -1;
    for (int i = 0; i < l; i++) {
        if (lib[i].used || cur_time + lib[i].t >= d) 
            continue;
        
        // cost/signing time is our main strategy
        //in order to get more scores with this code, you need play around this part
        // we tried cost^a/signing^b for several a and b constants
        if (max_lib == -1 || (double)(cost(max_lib, cur_time)) / (double)(lib[max_lib].t) < (double)(cost(i, cur_time)) / (double)(lib[i].t))
            max_lib = i;
    }
    
    if (max_lib == -1) {
        return 0;    
    }
    
    int cur = cost(max_lib, cur_time);
    
    delete_scanned(max_lib, cur_time);
    
    return cur + solve(cur_time + lib[max_lib].t);
}


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> b >> l >> d;
    b2l.resize(b);
    for (int i = 0; i < b; i++) {
        cin >> s[i];
    }
    for (int i = 0; i < l; i++) {
        lib[i].read(i);
    }
    cout << solve(0) << '\n';
    cout << (int)order.size() << '\n';
    for (int i = 0; i < (int)order.size(); i++) {
        library *cur = &lib[order[i]];
        cout << order[i] << " " << (int)(cur -> scanned.size()) << '\n';
        for (int j = 0; j < (int)(cur -> scanned.size()); j++) {
            cout << cur -> scanned[j] << " ";
        }
        cout << '\n';
    }
    return 0;
}
