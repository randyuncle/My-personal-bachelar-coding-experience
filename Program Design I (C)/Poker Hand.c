#include <stdbool.h>
#include <stdio.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS 5
#define PEOPLE 4

/* external variables */
int num_in_rank[PEOPLE][NUM_RANKS] = {0};
int num_in_suit[PEOPLE][NUM_SUITS] = {0};
int ranks[PEOPLE] = {0};
int temp_rank[NUM_CARDS] = {0};
int temp_suit[NUM_CARDS] = {0};
int all_rank[PEOPLE][NUM_CARDS] = {0};
int all_suit[PEOPLE][NUM_CARDS] = {0};

bool straight, flush, four, three;
int pairs;
int number = 0;
int read_people = 0;

void read_cards(void);
void analyze_hand(void);
void ranking(void);

int main(void)
{
    char g[PEOPLE] = {"\0"};
    for (;number < PEOPLE;) {
        read_cards();
        analyze_hand();
        ranking();
        read_people++;
    }
    int  j;
    for(int i=0 ; i<PEOPLE ; i++){
        j=0;
        for(int k=0 ; k<PEOPLE ; k++){
            if(ranks[i] > ranks[k])
                j++;
            else if(ranks[i] == ranks[k]){
                //straight && flush //flush //straight //single
                if (ranks[i] == 0 || ranks[i] == 3 || ranks[i] == 4 || ranks[i] == 8){
                    if(all_rank[i][4] < all_rank[k][4])
                        j++;
                    else if(all_rank[i][4] == all_rank[k][4] && all_suit[i][4] < all_suit[k][4])
                        j++;
                }
                //four
                else if (ranks[i] == 1){
                    if(all_rank[i][2] < all_rank[k][2])
                        j++;
                }
                //full house //three
                else if (ranks[i] == 2){
                    if(all_rank[i][2] < all_rank[k][2]) //middle of the cards
                        j++;
                }
                //pairs == 2
                else if (ranks[i] == 6){
                    int l,r,cou = 0,s_l,s_r;

                    for(l=0 ; l<NUM_RANKS ; l++){
                        if(num_in_rank[i][l] == 2){
                            if(cou) {
                                for (int temp = 0, c = 0; temp < NUM_CARDS; temp++) {
                                    if(all_rank[i][temp] == l){
                                        if(c) {
                                            s_l = all_suit[i][temp];
                                            break;
                                        }else
                                            c++;
                                    }
                                }
                            }else
                                cou++;
                        }
                    }
                    for(r=0 ,cou=0; r<NUM_RANKS ; r++){
                        if(num_in_rank[k][r] == 2){
                            if(cou) {
                                for (int temp = 0, c = 0; temp < NUM_CARDS; temp++) {
                                    if(all_rank[k][temp] == r){
                                        if(c) {
                                            s_r = all_suit[k][temp];
                                            break;
                                        }else
                                            c++;
                                    }

                                }

                            }else
                                cou++;
                        }
                    }

                    if(l < r)
                        j++;
                    else if(l == r && s_l < s_r)
                        j++;
                }
                //pairs == 1
                else if (ranks[i] == 7){
                    int l,r,s_l,s_r;

                    for(l=0 ; l<NUM_RANKS ; l++){
                        if(num_in_rank[i][l] == 2) {
                            for (int temp = 0; temp < NUM_CARDS; temp++) {
                                if (all_rank[i][temp] == l) {
                                    s_l = all_suit[i][temp];
                                    break;
                                }
                            }
                        }
                    }
                    for(r=0 ; r<NUM_RANKS ; r++){
                        if(num_in_rank[k][r] == 2) {
                            for (int temp = 0; temp < NUM_CARDS; temp++) {
                                if (all_rank[k][temp] == r) {
                                    s_r = all_suit[k][temp];
                                    break;
                                }
                            }
                        }
                    }
                    if(l < r)
                        j++;
                    else if(l == r && s_l < s_r)
                        j++;
                }
            }
        }

        if(i == 0)
            g[j] = 'A';
        else if(i == 1)
            g[j] = 'B';
        else if(i == 2)
            g[j] = 'C';
        else
            g[j] = 'D';
    }

    for(int i=0 ; i<PEOPLE ; i++){
        printf("%c ",g[i]);
    }


}

void read_cards(void)
{
    char rank_ch, suit_ch;
    int rank, suit;
    int cards_read = 0;
    while (cards_read < NUM_CARDS) {
        rank_ch = getchar();
        if(rank_ch == ' ' || rank_ch == '\n'){
            ;
        }else{
            switch (rank_ch) {
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    rank = rank_ch - '0' - 2;
                    break;
                case 't':
                case 'T':
                    rank = 8;
                    break;
                case 'j':
                case 'J':
                    rank = 9;
                    break;
                case 'q':
                case 'Q':
                    rank = 10;
                    break;
                case 'k':
                case 'K':
                    rank = 11;
                    break;
                case 'a':
                case 'A':
                    rank = 12;
                    break;
            }
            suit_ch = getchar();
            switch (suit_ch) {
                case 'c': case 'C':
                    suit = 0;
                    break;
                case 'd': case 'D':
                    suit = 1;
                    break;
                case 'h': case 'H':
                    suit = 2;
                    break;
                case 's': case 'S':
                    suit = 3;
                    break;
            }
            num_in_rank[read_people][rank]++;
            num_in_suit[read_people][suit]++;
            temp_rank[cards_read] = rank;
            temp_suit[cards_read] = suit;
            cards_read++;
        }

    } /* while (cards_read < NUM_CARDS) */
    for(int i=0 ; i<cards_read ; i++){
        int counter = 0;
        for(int j=0 ; j<cards_read ; j++){
            if(temp_rank[i] > temp_rank[j])
                counter++;
            else if(temp_rank[i] == temp_rank[j] && temp_suit[i] > temp_suit[j])
                counter++;
        }
        all_rank[read_people][counter] = temp_rank[i];
        all_suit[read_people][counter] = temp_suit[i];
    }

} /* void read_cards(void) */

void analyze_hand(void)
{
    int num_consec = 0;
    int rank, suit;
    straight = false;
    flush = false;
    four = false;
    three = false;
    pairs = 0;
    /* check for flush */
    for (suit = 0; suit < NUM_SUITS; suit++)
        if (num_in_suit[read_people][suit] == NUM_CARDS)
            flush = true;
    /* check for straight */
    rank = 0;
    while (num_in_rank[read_people][rank] == 0)
        rank++; /* find 1st nonzero */
    for (; rank < NUM_RANKS && num_in_rank[read_people][rank] > 0; rank++)
        num_consec++;
    if (num_consec == NUM_CARDS) {
        straight = true;
        return;
    }
    /* check for 4-of-a-kind, 3-of-a-kind, and pairs */
    for (rank = 0; rank < NUM_RANKS; rank++) {
        if (num_in_rank[read_people][rank] == 4)
            four = true;
        if (num_in_rank[read_people][rank] == 3)
            three = true;
        if (num_in_rank[read_people][rank] == 2)
            pairs++;
    }
}

void ranking(void)
{
    if (straight && flush)
        ranks[number] = 0;
    else if (four)
        ranks[number] = 1;
    else if (three &&pairs == 1)
        ranks[number] = 2;
    else if (flush)
        ranks[number] = 3;
    else if (straight)
        ranks[number] = 4;
    else if (three)
        ranks[number] = 5;
    else if (pairs == 2)
        ranks[number] = 6;
    else if (pairs == 1)
        ranks[number] = 7;
    else
        ranks[number] = 8;

    number++;
}


