#include<stdio.h>
int main(){
    float yr,height,weight;
    scanf("%f%f%f",&yr,&height,&weight);
    float BMI = (int)((weight/((height/100)*(height/100)))*10+0.5)/10.0;
    printf("Age = %.0f\nHeight = %.0fcm\nWeight = %.0fkg\nBMI = %.1f\n",2020-yr+1,height,weight,BMI);
    if(((2020-yr+1)<19) || ((2020-yr+1)>36)){
        printf("Not a Draftee (age < 19 or age > 36 years old)\nNo need to perform Military Service");
    }else{
        if(17.0<=BMI && BMI<=31.0){
            printf("Physical Status of Regular Service Draftees\n");
            if(yr<=1993){
                printf("1 year of Regular Service");
            }else{
                printf("4 months of Regular Service");
            }
        }else if(BMI<16.5 || BMI>31.5){
            printf("Physical Status of Military Service Exemption\nExemption from Military Service");
        }else{
            printf("Physical Status of Substitute Service Draftees\n");
            if(yr<=1993){
                printf("Substitute Services");
            }else{
                printf("Replacement Service");
            }
        }
    }

    return 0;

}
