#include <iostream>
#include <iomanip>
#include "loader.h"
#include "ai.h"
#include "mh.h"
#include "pI.h"

using namespace std;

int max_el(float* a, int n){
    int ans = 0;
    for(int i = 1; i < n; i++)
        if(a[ans] < a[i]) ans = i;
    return ans;
}

void generate_ans(float *a, int ans, short len){
    for(int i = 0; i < len; ++i)
        a[i] = 0;
    a[ans] = 10.0;
}

int train_ai(ai &ir, ai_dt &pc, int ans_sz, int cases, int j, int save_divider){
    int part_cor_ans = 0, cur_cor, *wrongs = new int [ans_sz];
    for(int i = 0; i < ans_sz; ++i)
        wrongs[i] = 0;
    float *a = new float[ans_sz], *cr_ans = new float[ans_sz], mid_mistake, cur;
    Loader loader(1, pc.len);
    for(int i = 0; i < pc.len; i += cases){
        mid_mistake = 0;
        cur_cor = 0;
        for(int k = 0; k < cases; k++){
            a = ir.calc_ans(pc.dt[i + k].bt);
            generate_ans(cr_ans, pc.dt[i + k].ans, ans_sz);
            ir.backpropogation_set(cr_ans);
            cur = ir.mistake(cr_ans);
            if(max_el(a, ans_sz) == pc.dt[i + k].ans)
                cur_cor++;
            else
                wrongs[pc.dt[i + k].ans]++,
                wrongs[max_el(a, ans_sz)]++;
            mid_mistake = mid_mistake + cur;
        }
        mid_mistake /= cases;
        loader.update(i + cases);
        ir.backpropagation_save(cases * save_divider);
        part_cor_ans += cur_cor;
    }
    cout << '\n' << j + 1 << ")  correct anses: " << part_cor_ans << " / " << pc.len << "\t so: " << fixed << setprecision(1) << (double) part_cor_ans * 100 / pc.len << "%\n"; 
    cout << "wrong for i(0..11): ";
    for(int i = 0; i < ans_sz; i++)
        cout << i << ") " << wrongs[i]*100/(pc.len - part_cor_ans) << "%, ",
        wrongs[i] = 0;
    cout << endl;
    ir.save_ai();
    return part_cor_ans;
}
