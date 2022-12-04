# C_Minesweeper
고급C프로그래밍(담당교수 최승호) 3분반 2022202072 유정휴 & 2022202073 박현웅 프로젝트 최종 <br>
~~윈도우 11 일부 버전에 한하여 콘솔창에서 숫자, 영문, * 기호가 전각처리되는 경우가 있어 전각버전과 반각버전 두가지를 업로드 합니다.~~

## 1. 프로젝트 개요
지뢰찾기는 전세계적으로 유명한 게임이며, Microsoft의 Windows에 깔려있는 지뢰찾기가 대표적으로 그 예시이다.<br>
고급C프로그래밍 프로젝트의 주제를 지뢰찾기로 정하고, C프로그래밍으로 직접 만들어보고자 하였음.  
프로젝트 개발 목적은 C언어를 활용해서 지뢰찾기를 얼마나 완벽하게 구현할 수 있는지, 그리고 나아가 기존 지뢰찾기와 차별점을 가진 프로그램을 개발하는 것.  

## 2.코드 설명
### 2-1. 작동방법
1. 프로그램 실행
2. 난이도 선택(1=EASY, 2=NORMAL, 3=HARD)
3. 난이도에 따라 MATRIX FIELD 생성<BR>**(EASY == 7X7, 6MINES | NORMAL == 10X10, 15MINES | HARD == 16X16, 46 MINES)**
4. 좌표 및 모드 입력(X, Y, MODE)
5. 모드에 따라 행동 실행<br>
5-1. (모드1) 지뢰찾기 실행<br>
5-2. (모드2) 깃발설치 실행<br>
6.반복

### 2-2. 사용 라이브러리
* <stdio.h>
* <stdlib.h>
  - for 'rand' & 'system' func.
* <time.h>
  - for 'rand' func.
  
### 2-3. 변수 및 매크로 상수 설명
* 전역변수 : 
```c
int mine_num[400]; // 난수로 받은 지뢰의 위치를 저장할 배열
int ffield[100][100], dfield[100][100], cfield[100][100]; // 지뢰찾기의 field 선언 (ffield는 초기필드 || ddfield는 작동중 가변하는 필드(추후 사용예정))
int difffield = 0, diffmine = 0, dm2 = 0; // difffield/mine : 난이도에 따른 크기 및 지뢰개수를 받을 변수
```

* 매크로 상수
```c
#define EASY_FIELD 7 // EASY(12.2%)
#define EASY_MINE 6
#define NORMAL_FIELD 10 // NORMAL(15.0%)
#define NORMAL_MINE 15
#define HARD_FIELD 16 // HARD (18.0%)
#define HARD_MINE 46
```

* main :
```c
int x = 0, y = 0, count = 0, flag = 0, diff = 0, mode = 0; //[x(x좌표) / y(y좌표) /  count ==> 기록용 시도회수 / flag(깃발] / diff(난이도 입력용 함수)/mode : 2==깃발, etc==지뢰찾기
int end;//  end ==> 종료변수 
int YN, fcount = 0; // YN == 재시작용 변수, fcount == 지뢰위치에 설치된 깃발개수 카운트용 함수
```

#### 2-3-1. 필드 변수 내부의 상수값 의미
* ffield && dfield
  - 0 == clear(no hint)
  - 1~8 == hint count
  - 9 == mine
  - 30 == last touch (for print RED X sign when game overed)
  - 50 == flag
* cfield
  - 0 == unsearched
  - 1 == searched
  - 50 == flag
  
### 2-4. 함수 설명
* setdifficulty
  - 입력받은 숫자에 따라 난이도를 설정함

* minegacha
  - rand함수를 이용하여 난수를 뽑음

* makeclearfield
  - 배열 초기화

* sethint
  - 지뢰의 위치의 8방향(↗↙↖↘→←↑↓)에 힌트를 1씩 늘림

* setminepos
  - 난수로 받은 지뢰들의 좌표를 2차원 좌표화 한뒤 필드에 심음(9)

* findmine
  - 좌표를 입력받고, 모드1일때 실행되는 함수
  - 입력받은 좌표의 배열값을 확인함.
    + 만약 0이면 8방향의 칸들 또한 확인시켜줌
    + 만약 1~8이면 해당좌표만 확인시켜줌
    + 만약 9(지뢰)이면 +30(마지막에 X좌표 출력을 위해) 후 게임종료

* showallmatrix
  - 마지막에 최종적으로 처음에 받았던 지뢰 필드를 출력
  
* printdfield
  - dfield(현재 진행상황)을 출력함
  
* checkend
  - 지뢰를 다 찾으면 종료함.

### 3. DEMO Video
  **↓ Click image below**<br>
  
  [![Video Label](http://img.youtube.com/vi/QVZY5gtc_Xg/0.jpg)](https://youtu.be/QVZY5gtc_Xg)
