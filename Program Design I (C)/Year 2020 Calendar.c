#include<stdio.h>

int main(){
    int n;
    scanf("%d",&n);
    printf("Sun Mon Tue Wed Thu Fri Sat \n");
    switch (n) {
        case 1:
            printf("            ");
            for(int i=1;i<=31;i++){
                if (i == 5 || ((i - 5) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ",i);
            }
            break;
        case 2:
            printf("                        ");
            for(int i=1;i<=29;i++){
                if (i == 2 || ((i - 2) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 3:
            for(int i=1;i<=31;i++){
                if (i == 8 || (((i - 8) % 7 == 0)) && (i - 8 > 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }break;
        case 4:
            printf("            ");
            for(int i=1;i<=30;i++){
                if (i == 5 || ((i - 5) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 5:
            printf("                    ");
            for(int i=1;i<=31;i++){
                if (i == 3 || ((i - 3) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 6:
            printf("    ");
            for(int i=1;i<=30;i++){
                if (i == 7 || ((i - 7) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 7:
            printf("            ");
            for(int i=1;i<=31;i++){
                if (i == 5 || ((i - 5) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 8:
            printf("                        ");
            for(int i=1;i<=31;i++){
                if (i == 2 || ((i - 2) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 9:
            printf("        ");
            for(int i=1;i<=30;i++){
                if (i == 6 || ((i - 6) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 10:
            printf("                ");
            for(int i=1;i<=31;i++){
                if (i == 4 || ((i - 4) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 11:
            for(int i=1;i<=30;i++){
                if (i == 8 || (((i - 8) % 7 == 0)) && (i - 8 > 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;
        case 12:
            printf("        ");
            for(int i=1;i<=31;i++){
                if (i == 6 || ((i - 6) % 7 == 0)) {
                    printf("\n");
                }
                printf("%3d ", i);
            }
            break;

    }
    return 0;
}
