#include<iostream>
using namespace std;
struct m {
	int r;
	int c;
	float **a;
};
int main(){
	m m1,m2;
	cout<<"enter the row of m1:";
	cin>>m1.r;
	cout<<"enter the column of m1:";
	cin>>m1.c;
	cout<<"enter the row of m2:";
	cin>>m2.r;
	cout<<"enter the column of m2:";
	cin>>m2.c;
	if(m1.c!=m2.r){
		cout<<"these two matrices can not multiply!";
		return 0;
	}
	
	cout<<"enter m1:"<<endl;
	m1.a=new float*[m1.r];
	for(int i=0;i<m1.r;i++){
		m1.a[i]=new float[m1.c];
	}
	for(int i=0;i<m1.r;i++){
		for(int j=0;j<m1.c;j++){
			cin>>m1.a[i][j];
			if(cin.fail()){
				cout<<"Please enter a right matrix!";
				return 0;
			}
		}
	}
	
	cout<<"enter m2:"<<endl;
	m2.a=new float*[m2.r];
	for(int i=0;i<m2.r;i++){
		m2.a[i]=new float[m2.c];
	}
	for(int i=0;i<m2.r;i++){
		for(int j=0;j<m2.c;j++){
			cin>>m2.a[i][j];
			if(cin.fail()){
				cout<<"Please enter a right matrix!";
				return 0;
			}
		}
	}
	
	m m3;
	m3.r=m1.r;
	m3.c=m2.c;
	m3.a=new float*[m3.r];
	for(int i=0;i<m3.r;i++){
		m3.a[i]=new float[m3.c];
	}
	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			m3.a[i][j] = 0;
		}
	}
	cout<<"The product is:"<<endl;
	for (int i = 0; i < m3.r; i++) {
		for (int k = 0; k < m1.c; k++) {
			float s = m1.a[i][k];
			for (int j = 0; j < m3.c; j++) {
				m3.a[i][j] += s * m2.a[k][j];
			}
		}
	}
	for(int i=0;i<m3.r;i++){
		for(int j=0;j<m3.c;j++){
			cout<<m3.a[i][j]<<" ";
		}
		cout<<endl;
	}
	return 0;
} 
