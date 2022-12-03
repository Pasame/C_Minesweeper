/*adv. C programming class3
2022202072 유정휴 & 2022202073 박현웅
title : minesweeper*/

/* <<중간발표 당시 작동방식>>
10X10의 필드에 15개의 지뢰 난수를 받고, 해당 난수들을 좌표화하여 지뢰를 심음.
지뢰의 값은 9이상으로 설정(hint의 값은 8을 넘을수가 없으므로)
현재는(중간발표 기준) 난수받고 지뢰심고, 힌트를 적는방향까지는 완료하였고,
소스코드에 그것을 확인하기 위한 문장들이 몇개 있음.
*/

/*<<중간발표 이후 변화>>
1. 기존의 10X10으로 고정되던 난이도를 이지, 노말, 하드 세가지로 분류
2. 지뢰찾기 기능(선택된 좌표의 8방향(N,S,W,E, NW, NE, SW, SE)에 지뢰개수를 알려줌) 추가
3. 깃발기능 추가
4. 시도횟수 출력
*/

/* 작동방식
난이도를 입력받은뒤, 각 난이도에 따라 필드 생성(easy 7X7, normal 10X10, hard 16X16)
생성후, 난수로 지뢰위치를 받고, 그위치에 지뢰(9)를 심음.*/

/*주요 상태들의 변수값
지뢰 : 9 || 깃발 : 50 || 지뢰찾기 실패시 밟은 지뢰 : 30*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <stdlib.h> //rand함수 및 system함수 사용을 위한 라이브러리
#include <time.h> //난수를 위한 라이브러리

#define EASY_FIELD 7 // EASY(12.2%)
#define EASY_MINE 6
#define NORMAL_FIELD 10 // NORMAL(15.0%)
#define NORMAL_MINE 15
#define HARD_FIELD 16 // HARD (18.0%)
#define HARD_MINE 46

int mine_num[400]; // 난수로 받은 지뢰의 위치를 저장할 배열
int ffield[100][100], dfield[100][100], cfield[100][100]; // 지뢰찾기의 field 선언 (ffield는 초기필드 || ddfield는 작동중 가변하는 필드(추후 사용예정))
int difffield = 0, diffmine = 0, dm2 = 0; // difffield/mine : 난이도에 따른 크기 및 지뢰개수를 받을 변수

/*ffield : 지뢰, 힌트개수, 깃발 등을 저장하는 배열
dfield : 외적으로 보이는 배열(힌트개수 등을 보여줌)
cfield : 지뢰를 찾는 과정이 시행된 칸인지를 보여주는 배열. 추가적으로 깃발의 경우도 보여줌*/

void setdifficulty(int diff) { // 난이도 설정함수
    if (diff == 1) { // easy모드
        difffield = EASY_FIELD;
        diffmine = EASY_MINE;
        dm2 = 49;
    }
    else if (diff == 2) { // normal
        difffield = NORMAL_FIELD;
        diffmine = NORMAL_MINE;
        dm2 = 100;
    }
    else if (diff == 3) { // hard
        difffield = HARD_FIELD;
        diffmine = HARD_MINE;
        dm2 = 256;
    }
}

void minegacha() { // 지뢰 난수받기 함수
    /*0~99까지의 난수 15가지를 얻는 과정(중복없이)
    get한 난수는 지뢰의 위치를 정해줌*/
    int fmine_num[100]; //난수 할당을 위한 지역변수

    for (int i = 0; i < diffmine; i++) {
        /*반복문을 이용하여 중복된 수를 제거하기 위한 과정
          얻은 난수를 기획득한 난수들과 비교해보며 값이 같을시 다시 난수를 뽑음*/
        fmine_num[i] = rand() % dm2; //0~99사이의 난수를 i번째 배열에 할당
        for (int j = 0; j < i; j++) {
            if (fmine_num[i] == fmine_num[j]) { /*중복된 변수 방지를 위한 과정의 condition*/
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < diffmine; i++) { //이때 획득한 난수들을 전역변수(배열) fmine_num에 할당
        mine_num[i] = fmine_num[i];
    }
}

void makeclearfield() { // field 초기화 함수
    /* 배열의 모든값을 0으로 초기화하여 아무것도 없는 백지상태로 만듬*/

    for (int i = 0; i < difffield; i++) {
        for (int j = 0; j < difffield; j++) {
            ffield[i][j] = 0;
            dfield[i][j] = 0;
            cfield[i][j] = 0;
        }
    }
}

void sethint(int xpos, int ypos) { //지뢰 힌트 설정 함수
    /*지뢰찾기에 존재하는 힌트(8방면에 있는 지뢰의 개수)를 설정하는 함수.
      기본적으로 지뢰가 존재하는 곳의 N, S, E, W, NW, NE, SW, SE 방향 8개에 1씩 추가함.
      하지만 지뢰가 배열의 맨 끝 테두리에 있을 경우(X,Y 좌표중 1개이상이 0또는9일때)에는
      기본 배열(10x10)을 오버하는 값에는 1을 추가하지 않음.*/
    if (xpos == (difffield - 1)) { // X좌표(※세로축)가 9일때 (9아래에 있는 배열값은 존재하지 않기때문에 추가하지 않음)
        /*※현재 배열 세로축 가로축 혼동으로 xpos가 세로축이고. ypos가 가로축이므로 추후에 수정해야함*/
        if (ypos == 0) { // y좌표(가로축)이 0일때(0 옆에 있는 배열값은 존재X -> 추가X)
            ffield[xpos - 1][ypos]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos - 1][ypos + 1]++;
        }
        else if (ypos == (difffield - 1)) {//ypos==9
            ffield[xpos - 1][ypos]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos - 1][ypos - 1]++;
        }
        else if (0 < ypos < (difffield - 1)) {//ypos가 0 or 9가 아닐때
            ffield[xpos - 1][ypos]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos - 1][ypos + 1]++;
            ffield[xpos - 1][ypos - 1]++;
        }
    }
    else if (xpos == 0) {//xpos == 0
        if (ypos == 0) {//ypos == 0
            ffield[xpos + 1][ypos]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos + 1][ypos + 1]++;
        }
        else if (ypos == (difffield - 1)) { //ypos == 9
            ffield[xpos + 1][ypos]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos + 1][ypos - 1]++;
        }
        else if (0 < ypos < (difffield - 1)) { // ypos != 0 || 9
            ffield[xpos + 1][ypos]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos + 1][ypos - 1]++;
            ffield[xpos + 1][ypos + 1]++;
        }
    }
    else { //xpos != 0 || 9
        if (ypos == (difffield - 1)) { //ypos == 9
            ffield[xpos - 1][ypos - 1]++;
            ffield[xpos + 1][ypos - 1]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos - 1][ypos]++;
            ffield[xpos + 1][ypos]++;
        }
        else if (ypos == 0) { //ypos == 0
            ffield[xpos - 1][ypos + 1]++;
            ffield[xpos + 1][ypos + 1]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos - 1][ypos]++;
            ffield[xpos + 1][ypos]++;
        }
        else { // xpos와 ypos가 모두 0 or 9가 아닐때
            ffield[xpos - 1][ypos]++;
            ffield[xpos + 1][ypos]++;
            ffield[xpos][ypos + 1]++;
            ffield[xpos][ypos - 1]++;
            ffield[xpos - 1][ypos + 1]++;
            ffield[xpos - 1][ypos - 1]++;
            ffield[xpos + 1][ypos - 1]++;
            ffield[xpos + 1][ypos + 1]++;
        }
    }

}

void setminepos() { // 지뢰위치 설정함수
    /*1~2자리 10진수로 이루어진 지뢰 난수들의 x좌표, y좌표를 구하고,
    지역변수를 이용해서 해당 좌표의 지뢰 힌트또한 sethint함수를 사용하여 설정해주는 함수*/
    int xpos = 0, ypos = 0; //※밑줄 참고
    for (int i = 0; i < diffmine; i++) { // 지뢰 개수만큼 반복
        ypos = mine_num[i]; // 지뢰의 값을 ypos에 저장함.
        xpos = 0; // xpos값 반복문마다 초기화

        while (1) {
            if (ypos < difffield) { //ypos가 한자리수일떄
                break; //반복문 탈출
            }
            xpos++;//xpos의 값을 추가함
            ypos = ypos - difffield; //ypos를 난이도 필드 길이만큼 줄여감
        }
        ffield[xpos][ypos] = 9; // 해당 좌표에 지뢰(9)를 심음
        sethint(xpos, ypos); // 해당 지뢰에 대하여 힌트값을 설정
    }
}

void findmine(int x, int y) {
    if (ffield[x][y] == 0) {
        if (x == (difffield - 1)) { // X좌표(※세로축)가 (난이도 변크기 -1)일때 (해당값보다 큰 좌표의 배열값은 존재하지 않기때문에 추가하지 않음)
            if (y == 0) { // y좌표(가로축)이 0일때(0 옆에 있는 배열값은 존재X -> 추가X)
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x - 1][y + 1] = ffield[x - 1][y + 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                cfield[x - 1][y] = 1;
                cfield[x - 1][y + 1] = 1;
                cfield[x][y + 1] = 1;
            }
            else if (y == (difffield - 1)) {//ypos==(difffield-1)
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x - 1][y - 1] = ffield[x - 1][y - 1];
                dfield[x][y - 1] = ffield[x][y - 1];
                cfield[x - 1][y] = 1;
                cfield[x - 1][y - 1] = 1;
                cfield[x][y - 1] = 1;
            }
            else if (0 < y < (difffield - 1)) {//ypos가 0 or 9가 아닐때
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y - 1] = ffield[x - 1][y - 1];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x - 1][y + 1] = ffield[x - 1][y + 1];
                dfield[x][y - 1] = ffield[x][y - 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                cfield[x - 1][y - 1] = 1;
                cfield[x - 1][y] = 1;
                cfield[x - 1][y + 1] = 1;
                cfield[x][y - 1] = 1;
                cfield[x][y + 1] = 1;
            }
        }
        else if (x == 0) {//xpos == 0
            if (y == 0) {//ypos == 0
                dfield[x][y] = ffield[x][y];
                dfield[x + 1][y] = ffield[x + 1][y];
                dfield[x + 1][y + 1] = ffield[x + 1][y + 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                cfield[x + 1][y] = 1;
                cfield[x + 1][y + 1] = 1;
                cfield[x][y + 1] = 1;
            }
            else if (y == (difffield - 1)) { //ypos == 9
                dfield[x][y] = ffield[x][y];
                dfield[x + 1][y] = ffield[x + 1][y];
                dfield[x + 1][y - 1] = ffield[x + 1][y - 1];
                dfield[x][y - 1] = ffield[x][y - 1];
                cfield[x + 1][y] = 1;
                cfield[x + 1][y - 1] = 1;
                cfield[x][y - 1] = 1;
            }
            else if (0 < y < (difffield - 1)) { // ypos != 0 || 9
                dfield[x][y] = ffield[x][y];
                dfield[x + 1][y - 1] = ffield[x + 1][y - 1];
                dfield[x + 1][y] = ffield[x + 1][y];
                dfield[x + 1][y + 1] = ffield[x + 1][y + 1];
                dfield[x][y - 1] = ffield[x][y - 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                cfield[x + 1][y - 1] = 1;
                cfield[x + 1][y] = 1;
                cfield[x + 1][y + 1] = 1;
                cfield[x][y - 1] = 1;
                cfield[x][y + 1] = 1;
            }
        }
        else { //xpos != 0 || 9
            if (y == (difffield - 1)) { //ypos == 9
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y - 1] = ffield[x - 1][y - 1];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x][y - 1] = ffield[x][y - 1];
                dfield[x + 1][y - 1] = ffield[x + 1][y - 1];
                dfield[x + 1][y] = ffield[x + 1][y];
                cfield[x - 1][y - 1] = 1;
                cfield[x - 1][y] = 1;
                cfield[x][y - 1] = 1;
                cfield[x + 1][y - 1] = 1;
                cfield[x + 1][y] = 1;
            }
            else if (y == 0) { //ypos == 0
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x - 1][y + 1] = ffield[x - 1][y + 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                dfield[x + 1][y] = ffield[x + 1][y];
                dfield[x + 1][y + 1] = ffield[x + 1][y + 1];
                cfield[x - 1][y] = 1;
                cfield[x - 1][y + 1] = 1;
                cfield[x][y + 1] = 1;
                cfield[x + 1][y] = 1;
                cfield[x + 1][y + 1] = 1;
            }
            else { // xpos와 ypos가 모두 0 or 9가 아닐때
                dfield[x][y] = ffield[x][y];
                dfield[x - 1][y - 1] = ffield[x - 1][y - 1];
                dfield[x - 1][y] = ffield[x - 1][y];
                dfield[x - 1][y + 1] = ffield[x - 1][y + 1];
                dfield[x][y - 1] = ffield[x][y - 1];
                dfield[x][y + 1] = ffield[x][y + 1];
                dfield[x + 1][y - 1] = ffield[x + 1][y - 1];
                dfield[x + 1][y] = ffield[x + 1][y];
                dfield[x + 1][y + 1] = ffield[x + 1][y + 1];
                cfield[x - 1][y - 1] = 1;
                cfield[x - 1][y] = 1;
                cfield[x - 1][y + 1] = 1;
                cfield[x][y - 1] = 1;
                cfield[x][y + 1] = 1;
                cfield[x + 1][y - 1] = 1;
                cfield[x + 1][y] = 1;
                cfield[x + 1][y + 1] = 1;
            }
        }


    }
    else
        dfield[x][y] = ffield[x][y];
}

void showallmatrix() {
    for (int i = 0; i < difffield; i++) {
        /* 마지막에 최종적으로 결과를 출력하는 부분
        9이상의 값(지뢰가 있는곳)은 Asterisk로 표현하고, 0인부분(힌트 및 지뢰가 존재하지 않는 부분)은
        space를 이용하여 아무것도 표시하지 않게 함*/
        for (int j = 0; j < difffield; j++) {
            if (ffield[i][j] >= 9 && ffield[i][j] <= 30 || ffield[i][j] >= 59) { // 9(지뢰)보다 크고, 30(밟은 지뢰)보다 작을때, 혹은 깃발이 꽃힌곳이 지뢰가 있는곳일때
                printf("*");
            }
            else if (ffield[i][j] == 0 || 50 <= ffield[i][j] && ffield[i][j] < 59) { // 힌트도 없고(8방향에 지뢰가 0개) 깃발이 있으나 지뢰가 없는 곳일때
                printf(" ");
            }
            else if (49 >= ffield[i][j] && ffield[i][j] >= 35) { // 지뢰를 밟았을때
                printf("\033[31mX\033[0m");
            }
            else {
                printf("%d", ffield[i][j]); // 기타(힌트값 출력)
            }
        }
        printf("\n");
    }
}

void printdfield() {
    system("cls");
    for (int i = 0; i < difffield; i++) {
        for (int j = 0; j < difffield; j++) {
            if (cfield[i][j] == 1 && ffield[i][j] == 0) { // 이미 확인한 값일떄(cfield == 1) 그좌표가 힌트가 존재하지 않을경우
                printf("□");
            }
            else if (cfield[i][j] == 1 && ffield[i][j] != 0) // 이미 확인했으나 힌트값이 있을경우
                printf("%d", ffield[i][j]);
            else if (cfield[i][j] >= 50) // 깃발을 사용한 곳일때
                printf("※"); // 깃발표시(깃발 아이콘이 유니코드가 깨져서 당구장표시로 대체)
            else// 기타의 경우(cfield == 0으로 아직 탐색하지 않은 칸일때
                printf("■"); 
        }
        printf("\n");
    }
}

int checkend() { // 종료 확인함수 (1=true/0=false)
    int cend = 0;
    for (int i = 0; i < difffield; i++) {
        for (int j = 0; j < difffield; j++) {
            if (cfield[i][j] == 1) {
                cend++;
            }
        }
    }
    if (cend == (dm2 - diffmine)) { // 기확인된 칸이 지뢰가 없는 필드의 칸의 개수와 같을떄
        return 1;
    }
    else
        return 0;
}

int main() { // main 함수
    int x = 0, y = 0, count = 0, flag = 0, diff = 0, mode = 0;
    //[x(x좌표) / y(y좌표) /  count ==> 기록용 시도회수 / flag(깃발] / diff(난이도 입력용 함수)/mode : 2==깃발, etc==지뢰찾기
    int end;//  end ==> 종료변수 
    int YN, fcount = 0; // YN == 재시작용 변수, fcount == 지뢰위치에 설치된 깃발개수 카운트용 함수
    while (1)
    {
        YN = 0; // 변수 초기화
        system("cls");
        diff = 0; // 변수 초기화
        srand((int)time(NULL)); // rand함수를 사용하기 위한 시간 초기화
        printf("난이도를 입력해 주세요\n(EASY : 1 | NORMAL : 2 | HARD : 3)"); // 난이도 입력받기
        scanf("%d", &diff);

        setdifficulty(diff); // 난이도 설정
        flag = diffmine; // 지뢰개수 설정

        minegacha(); // 지뢰 난수 설정 함수
        makeclearfield(); // field 초기화 함수
        printf("\n");
        setminepos();//지뢰위치 설정함수

        while (1) {
            end = 0;
            mode = 1;
            printdfield();
            printf("확인할 위치의 x, y 좌표를 입력하세요.\n");
            printf("음수를 입력하면 게임이 종료됩니다.\n");
            printf("x, y좌표, 모드를 입력해 주세요 [ex) 3 4 1]       시도횟수 : %d       깃발개수 : %d\n", count, flag);
            printf("(모드 1 == 지뢰찾기, 2 == 깃발)\n깃발을 모두 사용해야 게임이 종료됩니다.");
            scanf_s("%d %d %d", &x, &y, &mode);
            if (mode == 2) { // 깃발모드일때
                if (flag <= 0) { // 깃발이 없을때
                    printf("깃발을 모두 사용하였습니다.\n깃발이 설치된곳에 지뢰탐색을 실시하면 해당 깃발이 회수됩니다.\n.");
                    system("pause");
                }
                else if (x < 0 || y < 0) { //음수의 좌표를 입력했을때
                    break; 
                }
                else if (x < difffield && y < difffield) { // 함수크기만큼의 값을 입력했을때
                    // 이미 확인한 위치인지 체크한다.
                    if (cfield[y][x] == 0) { //만약 한번도 건들이지 않은 칸일때
                        /*x와 y가 반대로 들어간 이유는 2차원배열 특성상.*/
                        if (ffield[y][x] >= 9) { // 만약 선택된 칸이 지뢰(9)일때
                            cfield[y][x] += 50; // 깃발꼽기
                            ffield[y][x] += 50; // 깃발꼽기
                            fcount++; // 깃발개수 하나 늘리기
                            flag--; // 남은깃발개수 하나 줄이기
                            if (flag == 0 && fcount == diffmine) { // 만약 남은깃발이 없고, 맞춘 깃발이 지뢰개수와 같을떄
                                printf("모든 지뢰를 찾았습니다!\n당신의 승리!\n");
                                showallmatrix(); 
                                printf("최종 시도횟수 : %d \n게임을 종료합니다.", count);
                                break;
                            }
                        }
                        else { // 지뢰가 아닌 칸일떄
                            cfield[y][x] += 50;
                            ffield[y][x] += 50;
                            flag--; // flag만 하나 줄이고, fcount는 변동없음(틀려서)
                        }
                    }
                    else if (cfield[y][x] >= 50) { // 만약 이미 깃발이 설치된 칸일때
                        printf("이미 깃발을 설치한 위치입니다.\n 깃발을 회수합니다.");
                        cfield[y][x] -= 50;
                        ffield[y][x] -= 50;
                        flag++; // 깃발개수 하나 증가
                    }
                    else { // 기타의 경우(처음확인하는 칸도 아니고 깃발이 설치된 칸도 아닐떄)
                        printf("이미 확인된 위치입니다.\n\n");
                    }
                    system("pause");
                }
                else {
                    printf("잘못된 위치를 입력했습니다.\n\n");
                }
            }
            else { // 깃발모드가 아닐때 (지뢰찾기모드 진입)
                if (x < 0 || y < 0) {
                    break;
                }
                else if (x < difffield && y < difffield) {
                    // 이미 확인한 위치인지 체크한다.
                    if (cfield[y][x] == 0) {
                        // 사용자가 폭탄을 선택한 경우!
                        if (ffield[y][x] >= 9 && cfield[y][x] == 0) {
                            printf("폭탄을 선택했습니다 - 미션 실패!!\n\n");
                            ffield[y][x] += 30; // 마지막에 선택한곳이 지뢰였음을 보여주기 위한 30증가
                            showallmatrix();
                            printf("최종 시도횟수 : %d \n게임을 종료합니다.", count);
                            break;
                        }
                        else {
                            count++;
                            cfield[y][x] = 1; // 확인했음을 확인하기 위한 1
                            findmine(y, x);
                        }
                    }
                    else if (cfield[y][x] == 50) {
                        printf("깃발이 설치된 곳입니다.\n 깃발제거는 깃발모드에서만 가능합니다.");
                        system("pause");
                    }
                    // 선택했음을 설정한다.
                    else
                        printf("이미 확인한 위치입니다.\n\n");
                    system("pause");
                }
                else {
                    printf("잘못된 위치를 입력했습니다.\n\n");
                }

            }
            end = checkend();
            if (end == 1); { // 만약 종료변수가 1일때
                printf("승리하였습니다. 게임을 종료합니다.\n");
                showallmatrix();
                printf("최종 시도횟수 : %d \n", count);
            }
        }
        printf("다시 하시겠습니까?? : (Y=1/N=2)");
        scanf("%d", &YN);
        if (YN == 1) {
            continue;
            count == 0;
        }
        else {
            printf("게임을 종료합니다.");
            break;
        }
    }

    system("pause"); // console 창에서 바로 종료되는것을 막기 위한 pause

    return 0;
}
