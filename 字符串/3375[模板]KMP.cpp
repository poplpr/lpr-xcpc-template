#include<cstdio>
#include<cstring>
using namespace std;

const int maxs = 1e6 + 5;
char s1[maxs],s2[maxs];
int next[maxs];

void getfail(char* P,int* f){
	int m = strlen(P);
	f[0] = 0,f[1] = 0;
	for(int i=1;i<m;i++){
		int j = f[i];
		while(j && P[i] != P[j]) j = f[j];
		f[i+1] = P[i]==P[j] ? j+1 : 0;
	}
}

void find(char* T,char* P,int* f){
	int n = strlen(T),m = strlen(P);
	getfail(P,f);
	int j=0;
	for(int i=0;i<n;i++){
		while(j && P[j]!=T[i]) j = f[j];
		if(P[j] == T[i]) j++;
		if(j == m) printf("%d\n",i-m+2);
	}
}

int main(){
	scanf("%s%s",s1,s2);
	find(s1,s2,next);
	int len = strlen(s2);
	for(int i=1;i<=len;i++) printf("%d ",next[i]);
	return 0;
}
