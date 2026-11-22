#include <iostream>
#include <string>
using namespace std;
class Bill{public:virtual ~Bill()=default;virtual double total()const=0;};
class Payer{public:virtual ~Payer()=default;virtual double amountCovered(const Bill&)const=0;};
class SelfPay:public Payer{public:double amountCovered(const Bill& b)const override{return b.total();}};
class Insurance:public Payer{double limit,coPay;public:Insurance(double l,double c):limit(l),coPay(c){}double amountCovered(const Bill& b)const override{double amount=b.total();if(amount>limit)amount=limit;return amount*(1-coPay/100);}};
class GovernmentScheme:public Payer{double rate;public:GovernmentScheme(double r):rate(r){}double amountCovered(const Bill& b)const override{return b.total()<rate?b.total():rate;}};
class CorporatePayer:public Payer{double percent;public:CorporatePayer(double p):percent(p){}double amountCovered(const Bill& b)const override{return b.total()*percent/100;}};
class SampleBill:public Bill{double amount;public:SampleBill(double a):amount(a){}double total()const override{return amount;}};
int main(){SampleBill b(40000);SelfPay s;Insurance i(30000,10);GovernmentScheme g(25000);CorporatePayer c(60);cout<<"Self pay cover: "<<s.amountCovered(b)<<'\n'<<"Insurance cover: "<<i.amountCovered(b)<<'\n'<<"Government cover: "<<g.amountCovered(b)<<'\n'<<"Corporate cover: "<<c.amountCovered(b)<<'\n';}
