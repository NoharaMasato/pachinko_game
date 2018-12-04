//
//  main.cpp
//  main
//
//  Created by 野原将人 on 2018/10/31.
//  Copyright © 2018年 野原将人. All rights reserved.
//

#include <iostream>
#include<sstream>
#include <stdlib.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using namespace std;
#define PI 3.1415926535897932384626433832795

/* 物体の色 */
static GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
static GLfloat black[] = { 0, 0, 0, 1.0 };
static GLfloat brown[] = { 0.5, 0.0, 0.0, 1.0 };
static GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };
static GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };
static GLfloat ground[][4] = {
    { 0.6, 0.6, 0.6, 1.0 },
    { 0.3, 0.3, 0.3, 1.0 }
};

const double dt(0.01),g(-9.8);
double ex = 0, ey = -1.0, ez = -10.0 ,rx = 0,ry =0; //視点の位置と向きr(x方向回転とy方向回転)
int sum_point(0), sum_num(0); //合計得点と投げた回数
int cource(1); //コースの番号
int situation(0); //０はまだ発射してない。１は発射して飛んでいる状態、２は的に当たった後
int key_seq(0);
double px(0),py(0.5),pz(5),vx(0),vy(5),vz(0),v(0); //ボールの位置と速度

//単純な図形を描写する関数のプロトタイプ宣言
void DrawLine(GLfloat width, GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2);
void draw_circle(GLdouble x, GLdouble y, GLdouble z, GLdouble r,int point);
void draw_circle(GLdouble x, GLdouble y, GLdouble z, GLdouble r);
void draw_sphere(double x,double y,double z,double r);
void DrawString(double x, double y, string s);
void draw_square(double x,double y,double z,double r);
void draw_pachinko(GLdouble x, GLdouble y, GLdouble z);
void draw_tree(double x,double z);
//まとクラス
class mato{
protected:
    double cx, cy, cz,cr; //まとの中心座標
    double cpoint; //まとの点数
public:
    mato(double x,double y,double z, double r, int point);
    int hantei();
    void mato_draw();
};

//まとクラスのコンストラクタ
mato::mato(double x,double y,double z, double r, int point){
    cx = x;
    cy = y;
    cz = z;
    cr = r;
    cpoint = point;
}

void mato::mato_draw(){
    draw_circle(cx,cy,cz,cr,cpoint);
}

//このメンバ関数を常に呼び出し続ける
int mato::hantei(){
    if (situation == 1){
        if((cx-px)*(cx-px)+(cy-py)*(cy-py) < cr*cr && cz - 1 <= pz && pz <= cz){//当たったらという条件分岐
            cout << cpoint << "点の的に当たりました"<< endl;
            situation = 2;
            vz = 0;
            vx = 0;
            return cpoint; //判定が成功したらそのまとの得点を返す
        }
    }
    return 0;
}

//動くまとクラス（まとクラスを継承している）
class move_mato : public mato {
    double mato_vx,mato_vy; //まとの動く速度をた定義
public:
    void move();
    move_mato(double x,double y,double z, double r, int point, double mvx,double mvy);
};

//move_matoクラスのコンストラクタ。まとクラスのコンストラクタに引数を渡している
move_mato::move_mato(double x,double y,double z, double r, int point, double mvx,double mvy) : mato(x,y,z,r,point){
    mato_vx = mvx;
    mato_vy = mvy;
};

void move_mato::move(){ //ここで基底クラスのメンバにアクセスする必要が出てくる。そのためprotectedにしないといけない。何かいい方法はないのか。
    cx += mato_vx*dt;
    cy += mato_vy*dt;
    if (cx > 3) {
        cx = 3;
        mato_vx = -mato_vx;
    } else if (cx < -3){
        cx = -3;
        mato_vx = -mato_vx;
    }
    if (cy > 3) {
        cy = 3;
        mato_vy = -mato_vy;
    } else if (cy < 0){
        cy = 0;
        mato_vy = -mato_vy;
    }
}

//まとクラスのインスタンス作成
mato matos1[] = {mato(1,1,-3,0.7,100),mato(-1,1,-2,0.5,50),mato(0,3,0,1,200),mato(6,3,0,1,100)};//まとの定義
mato matos2[] = {mato(1,2,-5,0.7,100),mato(-1,1,-8,0.5,50),mato(0,3,-6,1,200),mato(6,3,0,1,100)};
mato matos3[] = {mato(2,2,-1,0.7,100),mato(0,1,-4,0.5,50),mato(1,3,0,1,200),mato(1,3,0,1,100)};
move_mato move_matos1[] = {move_mato(1,1,-1,0.5,80,2,1),move_mato(-1,1,-3,0.5,100,-1,1)};
move_mato move_matos2[] = {move_mato(1,1,-1,0.5,80,1,1),move_mato(-1,1,-1,0.5,100,-1,1)};
move_mato move_matos3[] = {move_mato(1,1,-1,0.5,80,1,1),move_mato(-1,1,-1,0.5,100,-1,1)};
int num_mato1 = sizeof matos1 / sizeof matos1[0];//まとの数
int num_mato2 = sizeof matos2 / sizeof matos2[0];
int num_mato3 = sizeof matos3 / sizeof matos3[0];
int num_move_mato1 = sizeof move_matos1 / sizeof move_matos1[0];//動くまとの数
int num_move_mato2 = sizeof move_matos2 / sizeof move_matos2[0];
int num_move_mato3 = sizeof move_matos3/ sizeof move_matos3[0];

/* 光源を設定する関数 */
void init_liting(){
    static GLfloat lit_amb[4]={0.2, 0.2, 0.2, 1.0};    /* 環境光の強さ */
    static GLfloat lit_dif[4]={0.9, 0.9, 0.9, 1.0};    /* 拡散光の強さ 値を0.7から0.9に変えたら一気に明るい雰囲気になった*/
    static GLfloat lit_spc[4]={1.0, 1.0, 1.0, 0.0};    /* 鏡面反射光の強さ */
    static GLfloat lightpos[] = { 3.0, 4.0, 5.0, 1.0 }; /* 光源の位置 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, lit_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_spc);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

//地面の描写
void scene(void)
{
    int i, j;
    glBegin(GL_QUADS);
    glNormal3d(0.0, 1.0, 0.0);
    for (j = -10; j < 10; ++j) {
        for (i = -10; i < 10; ++i) {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
            glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
            glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
            glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
            glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
        }
    }
    glEnd();
}

//ポイントを描写する（視野変換をする前に描写すると回転しない）
void not_move_scene(){
    glPushMatrix();
    glLoadIdentity();
    if (cource < 4) {
        init_liting();
        glRotated(0, 0.0, 1.0, 0.0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
        stringstream score;
        score <<"score:" << sum_point;
        glTranslated(0, 0, -9);
        DrawString(1.5,2, score.str()); //合計得点を描写している
        for (int i(0); i < 5 - sum_num; i++) { //残りの打つことができる玉の描写
            draw_circle(1.35 + i * 0.1,1.7,1,0.05);
        }
    } else {
        stringstream end;
        end <<"END";
        cout << "end";
        DrawString(0,0, end.str());
    }
    glPopMatrix();
}

void draw_main() {
    double x = 5 * sin(-rx / 60.0 + PI);
    double z = 5 * cos(-rx / 60.0 + PI);
    glPushMatrix(); //これは視点に対して回転する
    glTranslated(x, 0, z + 5);
    draw_sphere(px,py,pz,0.2); //初期位置 x = 0 y = 0.5,z = 5
    
    glPushMatrix();
    draw_pachinko(0, 0.5, 5);
    glPopMatrix();
    
    glPopMatrix();
}
//ここが描写を行う上でのmainとなる関数
void display(void)
{
//    cout << "draw" << endl; //キーボードを押しつずけても約１２倍描写の方が早い
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 画面クリア
    
    not_move_scene();//これは画面に対して動かない
    // モデルビュー変換行列の初期化
    glPushMatrix();
    glLoadIdentity();
    //下の二行で視野変換を行なっている（図形を全体的に動かしている。一方各図形で下の２行と同じ操作を行なっているのはモデリング変換）
    glRotated(rx, 0.0, 1.0, 0.0);
    glRotated(ry, -1.0, 0.0, 0.0);
    glTranslated(ex, ey, ez);
    
    if (cource < 4){
        scene();
        draw_main(); //ballとパチンコの描写
        if (cource == 1){
            for(int i(0); i < num_mato1; i++){
                matos1[i].mato_draw();
            }
            
            for(int i(0); i < num_move_mato1; i++){
                move_matos1[i].mato_draw();
            }
        } else if (cource == 2){
            for(int i(0); i < num_mato2; i++){
                matos2[i].mato_draw();
            }
            for(int i(0); i < num_move_mato2; i++){
                move_matos2[i].mato_draw();
            }
        } else if (cource == 3) {
            for(int i(0); i < num_mato3; i++){
                matos3[i].mato_draw();
            }
            for(int i(0); i < num_move_mato3; i++){
                move_matos3[i].mato_draw();
            }
        }
    }
    draw_tree(0,-5);//木の描写 ここに置くと光の影響を受けない（明るくなる）
    draw_tree(1.5,-5);
    draw_tree(-1.5,-5);
    draw_tree(-3,-5);
    draw_tree(5,-7);
    draw_tree(-5,-7);
    draw_tree(3,-7);
    draw_tree(-3,-3);
    draw_tree(0,-3);
    draw_tree(8,-3);
    draw_tree(9,-3);
    glFlush();
    glPopMatrix();
}

void resize(int w, int h){
    /* ウィンドウ全体をビューポートにする */
    glViewport(0, 0, w, h);
    
    /* 透視変換行列の指定 */
    glMatrixMode(GL_PROJECTION);
    
    /* 透視変換行列の初期化 */
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);//第一引数は前方に見える角度
    
    /* モデルビュー変換行列の指定 */
    glMatrixMode(GL_MODELVIEW);
}

/*玉の位置を変える関数*/
void ball_move() {
    v = 15 * key_seq / 10; //ここによってqを押した時間による速度の調整
    vx = v*sin(1.5*rx/180.0*PI);
    vz = -v*cos(1.5*rx/180.0*PI);
    vy += g*dt;
    px += vx*dt;
    py += vy*dt;
    if(py < 0){
        py = 0;
        vy = -0.8 * vy;
    }
    pz += vz*dt;
}

void mato_hantei(int num,int move_num, mato matos[],move_mato move_matos[]){
    for(int i(0); i < num; i++){
        sum_point += matos[i].hantei();
    }
    for(int i(0); i < move_num; i++){
        sum_point += move_matos[i].hantei();
    }
}

//move_matoを動かす関数
void mato_move(int move_num,move_mato move_matos[]){
    for(int i(0); i < move_num; i++){
        move_matos[i].move();
    }
}

/*スタートするとこの関数が呼ばれ続ける、描写な前に必要な操作（判定と移動）はここの中で行う*/
void idle(void){
    if (situation != 0){
        ball_move();
    }
    if (situation == 1){
        if (cource == 1){
            mato_hantei(num_mato1,num_move_mato1,matos1,move_matos1);
        } else if (cource == 2) {
            mato_hantei(num_mato2,num_move_mato2,matos2,move_matos2);
        } else if (cource == 3) {
            mato_hantei(num_mato3,num_move_mato3,matos3,move_matos3);
        }
    }
    if (cource == 1){
        mato_move(num_move_mato1,move_matos1);
    } else if (cource == 2) {
        mato_move(num_move_mato2,move_matos2);
    } else if (cource == 3) {
        mato_move(num_move_mato3,move_matos3);
    }
    glutPostRedisplay();
}
void keyboardup(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
            if (situation  == 0){ //発射される場合
                vy = 5;
                vy += ry * 0.5;
                if (cource < 4) {
                    if (sum_num <= 4){ //これで５回まで発射することができる
                        sum_num += 1;
                        situation = 1;//これによって状況が飛んでいる状況に変わる。
                    }else {
                        sum_num = 0;
                        cource += 1;
                    }
                }
            }
            cout << key_seq << endl;
            break;
        case ',':
            ex--;
            break;
        case 's':
            ey++;
            break;
    }
}

void keyboard(unsigned char key, int x, int y) //x,yはキーボードの位置
{
//    cout << "key" << endl;
    switch (key) {
        case '.':
            ex++;
            break;
        case ',':
            ex--;
            break;
        case 's':
            ey++;
            break;
        case 'd':
            ey--;
            break;
        case 'f':
            ez++;
            break;
        case 'g':
            ez--;
            break;
        case 'u':
            if (rx > -30) { //左右方向に30度ずつう動くようにした
                rx--;
            }
            break;
        case 'i':
            if (rx < 30) {
                rx++;
            }
            break;
        case 'j':
            ry--;
            break;
        case '8':
            ry++;
            break;
        case 'r':
            if (cource < 4) {
                px = 0;
                py = 0.5;
                pz = 5;
                vx = 0;
                vy = 5;
                vz = 0;
                situation = 0;
                key_seq = 0;
            }
            break;
        case '\033':
            exit(0);
            break;
        case 'q':
            key_seq += 1;
            break;
    }
    glutPostRedisplay();
}

void init(void)
{
    /* 初期設定 */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutIdleFunc(idle);
}


int main(int argc, char *argv[])
{
    /*windowの初期化、位置、大きさ決め*/
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 600);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("ball");//windowのタイトルを決める
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    init();
    glutMainLoop();
    return 0;
}


/*図形描写ライブラリ*/
void DrawLine(GLfloat width, GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2){
    glPushMatrix();
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex3d(x1,y1,z1);
    glVertex3d(x2,y2,z2);
    glEnd();
    glLineWidth(1.0);
    glPopMatrix();
}

//まとを描写
void draw_circle(GLdouble x, GLdouble y, GLdouble z, GLdouble r, int point){
    glPushMatrix();
    glTranslated(x,y,z);
    if (x > 4){ //これによって左右x軸方向に離れすぎれているものは円の法線方向を発射点に向くようにずらしている（必要かどうか？）
        glRotated(-20, 0.0, 1.0, 0.0);
    } else if (x < -4) {
        glRotated(20, 0.0, 1.0, 0.0);
    }
    int q(0); //まとをしましまにするために必要な変数
    for (double j(r); j > 0; j-=0.1){
        GLdouble x1, y1;
        q++;
        if (q%2 == 0){//ここのif文でわ色を分けている。それをしながら円の大きさを変えていっている
            glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        } else {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
        }
        glTranslated(0,0,0.01); //微妙に前側にずらすことによって、順に色をつけている
        glBegin(GL_POLYGON);
        glNormal3d(0.0, 0.0, 1.0);
        for (int i = 0; i < 100; i++) {
            float rate = (double)i / 100; // 座標を計算
            x1 = j * cos(2 * PI * rate);
            y1 = j * sin(2 * PI * rate);
            glVertex3f(x1, y1, 0); // 頂点座標を指定
        }
        glEnd();
    }
    glPopMatrix();
    
    stringstream cpoint;
    cpoint << point;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glPushMatrix();
    glTranslated(0,0,z + 1); //x,y方向は下の行で調節しているがz方向だけはここで調節している。まとから1だけ内側にずらして得点を表示させている
    DrawString(x,y, cpoint.str());
    glPopMatrix();
}

//draw_circleのオーバーロード（あと何球打てるかを表示させている）
void draw_circle(GLdouble x, GLdouble y, GLdouble z, GLdouble r){
    glPushMatrix();
    glTranslated(x,y,z);
    //    これは塗りつぶしたまと。こっちの方が圧倒的に実行スピードが早い
    GLdouble x1, y1;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    glBegin(GL_POLYGON);
    glNormal3d(0.0, 0.0, 1.0);
    for (int i = 0; i < 100; i++) {
        // 座標を計算
        float rate = (double)i / 100;
        x1 = r * cos(2 * M_PI * rate);
        y1 = r * sin(2 * M_PI * rate);
        glVertex3f(x1, y1, 0); // 頂点座標を指定
    }
    glEnd();
    glPopMatrix();
}

void draw_sphere(double x,double y,double z,double r){
    glPushMatrix();
    glTranslated(x, y, z);//平行移動値の設定
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glutSolidSphere(r, 50, 50);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
    glPopMatrix();
}

void DrawString(double x, double y, std::string s){
    glRasterPos2f(x, y);
    for(unsigned int i=0; i<s.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s.at(i));
    }
}

void draw_square(double x,double y,double z,double r){
    glPushMatrix();
    //    glTranslated(x, y, z);
    glBegin(GL_POLYGON);
    glVertex3d(x, y, z);
    glVertex3d(x + r, y, z);
    glVertex3d(x + r, y + r, z);
    glVertex3d(x, y + r, z);
    glEnd();
    glPopMatrix();
}

void draw_pachinko(GLdouble x, GLdouble y, GLdouble z){
    glPushMatrix();
    glTranslated(x, y, z);
    glRotatef(-rx*1.5, 0.0, 1.0, 0.0);//パチンコそのものの回転(要検討　このままだと、大きな円を書いて回る)
    GLdouble x1, y1;
    for (int i = 90; i < 270; i+=5) {
        x1 = sin(PI*i / 180.0)/3;
        y1 = cos(PI*i / 180.0)/3;
        draw_sphere(x1, y1, 0, 0.1);
    }
    for (double y = -0.45; y > -1.0; y-=0.05) {
        draw_sphere(0, y, 0, 0.1);
    }
    glPopMatrix();
}

void draw_tree(double x,double z){
    glPushMatrix();
    glTranslated(x, 0, z);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
    DrawLine(15, 0.0, -0.5, 0, 0.0, 1.5, 0);    // 線の描画(x軸)
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    for(int i=0 ; i<=360; i++){
        DrawLine(2, 0,2.0, 0.0, 0.5*sin(PI*i/180.0),0.5,0.5*cos(PI*i/180.0)); // 線の描画(x軸)
    }
    glPopMatrix();
}
