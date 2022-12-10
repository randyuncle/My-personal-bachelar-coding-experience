#include<stdio.h>

int main(){
	int n,counter = 1;
	scanf("%d",&n);
	long int metrix[n][n];
	for(int i=0 ; i<n ; i++ ){
		for(int j=0 ; j<n ; j++ ){
			scanf("%ld",&metrix[i][j]);
		}
	}
	for(int i=0 ; i<n && counter ; i++ ){
		for(int j=0 ; j<n && counter ; j++ ){
			if(metrix[i][j] != metrix[j][i])
				counter--;
		}
	}
	if(counter)
		printf("Yes\n");
	else
		printf("No\n");

	return 0;
}