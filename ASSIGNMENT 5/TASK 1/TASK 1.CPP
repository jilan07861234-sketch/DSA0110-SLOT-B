#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;
class Bill{protected:string patient;public:Bill(string p):patient(p){}virtual ~Bill()=default;virtual double total()const=0;virtual string type()const=0;void print()const{cout<<type()<<" "<<patient<<": "<<total()<<'\n';}};
class OutPatient:public Bill{double consultation,tests;public:OutPatient(string p,double c,double t):Bill(p),consultation(c),tests(t){}double total()const override{return consultation+tests;}string type()const override{return "OP";}};
class InPatient:public Bill{int days;double perDay,surgery;public:InPatient(string p,int d,double r,double s):Bill(p),days(d),perDay(r),surgery(s){}double total()const override{return days*perDay+surgery;}string type()const override{return "IP";}};
class DayCare:public Bill{double package;public:DayCare(string p,double v):Bill(p),package(v){}double total()const override{return package;}string type()const override{return "DayCare";}};
class Emergency:public Bill{double base,hours;public:Emergency(string p,double b,double h):Bill(p),base(b),hours(h){}double total()const override{return base+hours*500;}string type()const override{return "Emergency";}};
class Payer{public:virtual ~Payer()=default;virtual double amountCovered(const Bill& b)const=0;};
class SelfPay:public Payer{public:double amountCovered(const Bill& b)const override{return b.total();}};
class Insurance:public Payer{double limit,copay;public:Insurance(double l,double c):limit(l),copay(c){}double amountCovered(const Bill& b)const override{return b.total()>limit?limit*(1-copay/100):b.total()*(1-copay/100);}};
class GovernmentScheme:public Payer{double packageRate;public:GovernmentScheme(double r):packageRate(r){}double amountCovered(const Bill& b)const override{return b.total()<packageRate?b.total():packageRate;}};
int main(){vector<unique_ptr<Bill>> bills;bills.push_back(make_unique<OutPatient>("Ravi",500,1200));bills.push_back(make_unique<InPatient>("Meena",3,2500,40000));bills.push_back(make_unique<DayCare>("Arun",18000));bills.push_back(make_unique<Emergency>("Sara",2000,3));double sum=0;for(auto& b:bills){b->print();sum+=b->total();}cout<<"Total "<<sum<<'\n';Insurance insurer(50000,20);GovernmentScheme scheme(15000);cout<<"Insurance on Meena: "<<insurer.amountCovered(*bills[1])<<'\n';cout<<"Scheme on DayCare: "<<scheme.amountCovered(*bills[2])<<'\n';}
