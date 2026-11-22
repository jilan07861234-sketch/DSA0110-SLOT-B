#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
class Bill{protected:string patient;public:Bill(string p):patient(p){}virtual ~Bill()=default;virtual double total()const=0;virtual string type()const=0;};
class OutPatient:public Bill{double a,b;public:OutPatient(string p,double x,double y):Bill(p),a(x),b(y){}double total()const override{return a+b;}string type()const override{return "OP";}};
class InPatient:public Bill{double a,b;public:InPatient(string p,double x,double y):Bill(p),a(x),b(y){}double total()const override{return a*3+b;}string type()const override{return "IP";}};
class MaternityBill:public Bill{double package;public:MaternityBill(string p,double x):Bill(p),package(x){}double total()const override{return package;}string type()const override{return "Maternity";}};
class Payer{public:virtual ~Payer()=default;virtual double amountCovered(const Bill&)const=0;};
class Insurance:public Payer{double limit;public:Insurance(double l):limit(l){}double amountCovered(const Bill& b)const override{return b.total()<limit?b.total():limit;}};
class EmployeePayer:public Payer{double share;public:EmployeePayer(double s):share(s){}double amountCovered(const Bill& b)const override{return b.total()*share/100;}};
int main(){vector<unique_ptr<Bill>> bills;bills.push_back(make_unique<OutPatient>("Ravi",500,1200));bills.push_back(make_unique<InPatient>("Meena",2500,40000));bills.push_back(make_unique<MaternityBill>("Nila",65000));Insurance ins(50000);EmployeePayer emp(70);for(auto& b:bills)cout<<b->type()<<" "<<b->total()<<" insurance="<<ins.amountCovered(*b)<<" employee="<<emp.amountCovered(*b)<<'\n';cout<<"All objects released safely\n";}
