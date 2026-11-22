#include <iostream>
#include <stdexcept>
#include <string>
#include <new>
#include <limits>
using namespace std;
class ClaimError:public runtime_error{public:using runtime_error::runtime_error;};
double approve(double billed,double cover){
    if(billed<0||cover<0)throw invalid_argument("negative amount");
    if(billed>cover)throw ClaimError("exceeds cover");
    return billed;
}
double settle(double billed,double cover){
    try{return approve(billed,cover);}
    catch(const ClaimError& e){cout<<"audit: "<<e.what()<<'\n';throw;}
}
void validate(double amount){if(amount<0)throw invalid_argument("negative");if(amount>1e6)throw out_of_range("too large");cout<<"ok ";}
void process(double amount){try{validate(amount);}catch(const out_of_range& e){cout<<"[log "<<e.what()<<"] ";throw;}}
void checkPackage(const string& code){if(code!="DAYCARE-01")throw runtime_error("unknown package code");}
void checkPatient(bool found){if(!found)throw runtime_error("patient record missing");}
int main(){
    double cases[][2]={{40000,50000},{60000,50000},{-1,50000}};
    for(auto& c:cases)try{double paid=settle(c[0],c[1]);cout<<"approved "<<paid<<'\n';}
    catch(const ClaimError& e){cout<<"claim rejected: "<<e.what()<<'\n';}
    catch(const exception& e){cout<<"error: "<<e.what()<<'\n';}
    for(double a:{100.0,-5.0,2e6})try{process(a);cout<<"done ";}
    catch(const exception& e){cout<<"caught "<<e.what()<<' ';}
    cout<<'\n';
    try { size_t huge=numeric_limits<size_t>::max(); char* p=new char[huge]; delete[] p; }
    catch(const bad_alloc& e){cout<<"allocation failure caught: "<<e.what()<<'\n';}
    try { size_t parsed=static_cast<size_t>(stod("not-a-number")); cout<<parsed<<'\n'; }
    catch(const invalid_argument& e){cout<<"parse error caught: "<<e.what()<<'\n';}
    try { checkPackage("UNKNOWN"); } catch(const exception& e){cout<<"package error caught: "<<e.what()<<'\n';}
    try { checkPatient(false); } catch(const exception& e){cout<<"patient error caught: "<<e.what()<<'\n';}
    try { throw 7; } catch(...) { cout<<"unknown error caught at top level\n"; }
}
