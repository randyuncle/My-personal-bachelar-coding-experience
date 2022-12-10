#include<stdio.h>
#include<math.h>

int main(){
    int mode;
    scanf("%d",&mode);
    double l,w,r,h;
    switch(mode){
        case 1:
            scanf("%lf",&l);
            printf("%.2lf",l*l*l);
            break;
        case 2:
            scanf("%lf %lf %lf",&l,&w,&h);
            printf("%.2lf",l*w*h);
            break;
        case 3:
            scanf("%lf %lf",&r,&h);
            printf("%.2lf",3.14*r*r*h);
            break;
        case 4:
            scanf("%lf",&l);
            printf("%.2lf",(sqrt(2)/12)*l*l*l);
            break;

    }

    return 0;
}
