#include <chrono>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <variant>
using namespace std;
using Clock=chrono::steady_clock;
struct BillBase{virtual ~BillBase()=default;virtual double total()const=0;};
struct OP:BillBase{double a,b;OP(double x,double y):a(x),b(y){}double total()const override{return a+b;}};
struct IP:BillBase{double a,b;IP(double x,double y):a(x),b(y){}double total()const override{return a*3+b;}};
struct PlainBill{int type;double a,b;};
double switched(const PlainBill& x){switch(x.type){case 0:return x.a+x.b;default:return x.a*3+x.b;}}
struct VOP{double a,b;};struct VIP{double a,b;};
using AnyBill=variant<VOP,VIP>;
double visited(const AnyBill& x){return visit([](const auto& v)->double{using T=decay_t<decltype(v)>;if constexpr(is_same_v<T,VOP>)return v.a+v.b;else return v.a*3+v.b;},x);}
template<class F> double measure(const string& label,F fn){auto start=Clock::now();double sum=fn();double ms=chrono::duration<double,milli>(Clock::now()-start).count();cout<<label<<" ms="<<ms<<" checksum="<<sum<<'\n';return sum;}
bool approveCode(double x){return x>=0&&x<=1000000;}
bool approveThrow(double x){if(x<0)throw invalid_argument("negative");if(x>1000000)throw out_of_range("large");return true;}
int main(){
    OP op(100,20); IP ip(101,20);
    BillBase* ptrs[10]; PlainBill plain[10]; AnyBill vars[10];
    for(int i=0;i<10;++i){
        if(i%2==0){ptrs[i]=&op;plain[i]={0,100,20};vars[i]=VOP{100,20};}
        else{ptrs[i]=&ip;plain[i]={1,101,20};vars[i]=VIP{101,20};}
    }
    double v0=0,s0=0,w0=0;
    for(int i=0;i<10;++i){v0+=ptrs[i]->total();s0+=switched(plain[i]);w0+=visited(vars[i]);}
    cout<<"10-bill sample totals: virtual="<<v0<<" switch="<<s0<<" variant="<<w0<<'\n';
    for(int n:{1000,1000000,10000000}){
        measure("Virtual "+to_string(n),[&](){double sum=0;for(int i=0;i<n;++i)sum+=ptrs[i%10]->total();return sum;});
        measure("Switch "+to_string(n),[&](){double sum=0;for(int i=0;i<n;++i)sum+=switched(plain[i%10]);return sum;});
        measure("Variant "+to_string(n),[&](){double sum=0;for(int i=0;i<n;++i)sum+=visited(vars[i%10]);return sum;});
    }
    for(int invalidPct:{0,1,50}){
        long long bad=1000000LL*invalidPct/100;
        measure("Return-code "+to_string(invalidPct)+"%",[&](){long long accepted=0;for(int i=0;i<1000000;++i){double x=i<bad?-1:100;accepted+=approveCode(x);}return double(accepted);});
        measure("Exception "+to_string(invalidPct)+"%",[&](){long long accepted=0;for(int i=0;i<1000000;++i){double x=i<bad?-1:100;try{accepted+=approveThrow(x);}catch(const exception&){} }return double(accepted);});
    }
}
