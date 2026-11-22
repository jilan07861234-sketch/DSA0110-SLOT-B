#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
class BillingError:public runtime_error{public:using runtime_error::runtime_error;};
class ClaimError:public BillingError{public:using BillingError::BillingError;};
class Guard{string name;public:Guard(string n):name(n){cout<<"open "<<name<<'\n';}~Guard(){cout<<"close "<<name<<'\n';}};
void processClaim(double amount){Guard g("claim");if(amount<0)throw invalid_argument("negative amount");if(amount>50000)throw ClaimError("above cover");cout<<"claim accepted\n";}
int main(){for(double x:{10000.0,-5.0,60000.0}){try{try{processClaim(x);}catch(const ClaimError& e){cout<<"audit: "<<e.what()<<'\n';throw;}}catch(const exception& e){cout<<"reported: "<<e.what()<<'\n';}}}
