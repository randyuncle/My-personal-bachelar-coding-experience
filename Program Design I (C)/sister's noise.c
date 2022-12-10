#include<stdio.h>

int main(){
	int sisters[10],temp,max = 0;
	for(int i=0 ; i<10 ; i++ ){
		scanf("%d",&sisters[i]);
	}
	for(int i=0 ; i<10 ; i++ ){
		for(int j=(i+1) ; j<10 ; j++ ){
			temp = (sisters[i]+sisters[j])*(j-i);
			if(temp>max){
				max = temp;
			}
		}
	}
	printf("%d",max);
	
	return 0;
	
}