#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using pii = std::pair <int, int>;

template<typename Node_CItr,
    typename Node_Itr,
    typename Cmp_Fn,
    typename Alloc>
struct my_node_update{
    typedef int metadata_type;

    int query(int rank){
        auto it = node_begin();
        while (rank){
            auto l = it.get_l_child();
            auto r = it.get_r_child();
            int lsz = l == node_end() ? 0 : l.get_metadata();
            if (rank <= lsz){
                it = l;
                continue;
            }
            rank -= lsz;
            if (rank == 1){
                return (*it) -> first;
            }
            -- rank;
            it = r;
        }
        return -1;
    }

    int rank(pii p){
        int ans = 0;
        auto it = node_begin();
        while (it != node_end()){
            auto l = it.get_l_child();
            auto r = it.get_r_child();
            if (Cmp_Fn()(p, **it)){
                it = l;
            }
            else{
                ans += (l == node_end() ? 0 : l.get_metadata()) + 1;
                it = r;
            }
        }
        return ans;
    }

    void operator()(Node_Itr it, Node_CItr end_it){
        auto l = it.get_l_child();
        auto r = it.get_r_child();
        int left = 0, right = 0;
        if (l != end_it) left = l.get_metadata();
        if (r != end_it) right = r.get_metadata();
        const_cast<int &>(it.get_metadata()) = left + right + 1;
    }

    virtual Node_CItr
    node_begin() const = 0;

    virtual Node_CItr
    node_end() const = 0;
};

typedef tree<
    pii,
    null_type,
    std::less<pii>,
    rb_tree_tag,
    my_node_update>
    ordered_set;

int main(){
    ordered_set set;
    int n;
    scanf("%d", &n);
    while (n --){
        int op;
        scanf("%d", &op);
        if (op == 1){
            int x;
            scanf("%d", &x);
            set.insert({x, n});
        }
        else if (op == 2){
            int x;
            scanf("%d", &x);
            auto u = set.lower_bound({x, INT_MIN});
            set.erase(u);
        }
        else if (op == 3){
            int x;
            scanf("%d", &x);
            int ans = set.rank({x, INT_MIN}) + 1;
            printf("%d\n", ans);
        }
        else if (op == 4){
            int x;
            scanf("%d", &x);
            int ans = set.query(x);
            printf("%d\n", ans);
        }
        else if (op == 5){
            int x;
            scanf("%d", &x);
            auto u = set.lower_bound({x, INT_MIN});
            u = std::prev(u);
            printf("%d\n", u -> first);
        }
        else if (op == 6){
            int x;
            scanf("%d", &x);
            auto u = set.lower_bound({x, INT_MAX});
            printf("%d\n", u -> first);
        }
    }
}

void other(){
    typedef tree<
    int,
    null_type,
    less<int>,
    rb_tree_tag,
    tree_order_statistics_node_update>
    ordered_set;

    ordered_set X;
    X.insert(1);
    X.insert(2);
    X.insert(4);
    X.insert(8);
    X.insert(16);

    cout<<*X.find_by_order(1)<<endl; // 2
    cout<<*X.find_by_order(2)<<endl; // 4
    cout<<*X.find_by_order(4)<<endl; // 16

    cout<<X.order_of_key(-5)<<endl;  // 0
    cout<<X.order_of_key(1)<<endl;   // 0
    cout<<X.order_of_key(3)<<endl;   // 2
    cout<<X.order_of_key(4)<<endl;   // 2
    cout<<X.order_of_key(400)<<endl; // 5
}
