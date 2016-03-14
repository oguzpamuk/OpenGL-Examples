
/*
	*Author: Oguzcan Pamuk
	*Number: 111044053
	*Class : main.cpp
*/

#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <iostream>
#include <gl\glut.h>



void render(void);
/*Program menulerini ekrana basar*/
void printMenu(void);
/*Mouse sol tusuna basinca koordinatlari verir ve algoritmayi baslatir*/
void leftClickMouse(int button, int state, int x, int y);
/*Direct Draw Algorithm ile dogru cizimi yapan fonksiyon*/
void DDAAlgorithm(void);
/*Bresenham Algoritmasý ile dogru cizimi yapan fonksiyon*/
void bresenhamAlgorithm(void);
/*Glut icin ilk ayarlar*/
void initGlut();
void createGrid(void);
void benchmark(void);
/*cout ve cin icin*/
using namespace std;
/*koordinatlar*/
int X1, Y1, X2, Y2;
/*iki defa mouse uzerinde nokta alabilmek icin*/
int counter = 0;
/*menu tercihi*/
int choose = 0;

/*board genislik ve yukseklik*/
/*200*4*/
#define WIDTH 800
#define HEIGHT 800
/*sayi yuvarlamak icin*/
#define ROUND_VALUE 0.5
/*DDA icin macro fonksiyonlar*/
#define FLOOR(X) floor(X + ROUND_VALUE)
#define STEPS(XORT,YORT) (abs(XORT)>abs(YORT)) ? (abs(XORT)) : (abs(YORT))
#define INTERVAL_BEGIN 20
#define INTERVAL_END 100
#define INTERVAL_GRID 4

int main(int argc, char** argv){
	//random sayilar icin
	srand(time(NULL));
	/*GLUT library ayarlari*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	/*Boardu olusturuyor*/
	glutCreateWindow("HW1");
	/* Boardun ayarlarini yapiyor */
	initGlut();
	/*menuyu basar ve bir deger bekler*/
	printMenu();
	/*boardu temizler*/
	glutDisplayFunc(render);	
	/*fonksiyonlar burada cagiriliyor*/
	
	if (choose != 3){
		glutMouseFunc(leftClickMouse);
	} else{
		glutDisplayFunc(benchmark);
	}
		
	/*surekli calisabilir kilmak icin*/
	glutMainLoop();
	//return 0;
}


void initGlut()
{
	/*Renk ayarlari,beyaz board*/
	glClearColor(1.0, 1.0, 1.0, 0);
	glColor3f(0.0, 0.0, 0.0);
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

void putPixel(int x, int y){
    glColor3f(0.3, 0.2, 0.0); // activate the pixel by setting the point color to white  
    glBegin(GL_POINTS);
	glVertex2i(x, y); // set the point  
    glEnd();
    glFlush(); // process all openGL routines as quickly as possible  
 }

/*Bresenham Algoritmasý ile dogru cizimi yapan fonksiyon*/
void bresenhamAlgorithm(void)
{
	int dx = abs(X2 - X1);
	int dy = abs(Y2 - Y1);
	int x, y;
	if (dx >= dy){
		int d = 2 * dy - dx;
		int ds = 2 * dy;
		int dt = 2 * (dy - dx);
		if (X1 < X2){
			x = X1;
			y = Y1;
		}else{
			x = X2;
			y = Y2;
			X2 = X1;
			Y2 = Y1;
		}
		putPixel(x, y);
		while (x < X2){
			if (d < 0){
				d += ds;
			}else{
				if (y < Y2){
					y++;
					d += dt;
				}else{
					y--;
					d += dt;
				}
			}
			x++;
			putPixel(x, y);

		}
	}else{
		int d = 2 * dx - dy;
		int ds = 2 * dx;
		int dt = 2 * (dx - dy);
		if (Y1 < Y2){
			x = X1;
			y = Y1;
		}else{
			x = X2;
			y = Y2;
			Y2 = Y1;
			X2 = X1;
		}

		putPixel(x, y);

		while (y < Y2){

			if (d < 0) {
				d += ds;
			}else{
				if (x > X2){
					x--;
					d += dt;
				}else{
					x++;
					d += dt;
				}
			}

			y++;
			putPixel(x, y);
		}
	}
}

/*Mouse sol tusuna basinca koordinatlari verir ve algoritmayi baslatir*/
void leftClickMouse(int button, int state, int x, int y)
{
	//sol tusa basilip basilmadigini kontrol eder
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN){
			
			/*counter 0 ise ilk degerleri alir , x1,y2*/
			if (counter == 0){
				cout << "X1-->" << x << ", Y1-->" << y << endl;
				X1 = x;
				Y1 = HEIGHT - y;
				counter++;
			}
			else{
				/*counter 0dan farkli ise ikinci noktanin degerlerini alir , x2,y2*/
				cout << "X2-->" << x << ", Y2-->" << y << endl;
				X2 = x;
				Y2 = HEIGHT - y;
				counter = 0;	
				/*Menuden hangi method secildiyse burada calistirilir*/
				if (choose == 1)
					DDAAlgorithm();
				else if (choose == 2)
					bresenhamAlgorithm();					
			}

		}
	}
}

/*Ekrani surekli calisir kilmak icin kullaniyorum*/
void render(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//cout << "girdi";
	glBegin(GL_LINES);
	createGrid();
	glEnd();
	glFlush();
}


/*Direct Draw Algorithm ile dogru cizimi yapan fonksiyon*/
void DDAAlgorithm(void){

	/*x'ler ve y'ler arasi uzaklik bu degiskenlerde tutulur*/
	/*counter ise kac asamada dogruyu cizecegini belirtir*/
	double xOrt, yOrt, counter;
	/*Noktalarin ne kadar araliklarla ilerleyecegi belirlenir*/
	/*x ve y degiskeni global olarak tanimlanan koordinatlari bozmamak icin kullanilir*/
	float xPlus, yPlus, x, y;
	/*dongu counteri*/
	int i = 0;
	
	xOrt = (X2 - X1);
	yOrt = (Y2 - Y1);

	/*global koordinatlari bozmamak icin kullaniyorum*/
	x = X1;
	y = Y1;

	/*x ve y koordinatlarini hangi aralýklarla arttiracagimi buluyorum*/
	counter = STEPS(xOrt, yOrt);
	xPlus = xOrt / (float)counter;
	yPlus = yOrt / (float)counter;

	//cout << xPlus << " - " << yPlus << " - " << counter << endl;

	glClear(GL_COLOR_BUFFER_BIT);

	/*noktalar belirlenen araliklarla cizilmeya baslanir*/
	glBegin(GL_LINES);
	createGrid();
	glEnd();
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	
	for (i = 0; i < counter; i++){
		x += xPlus;
		y += yPlus;
		glVertex2d(FLOOR(x), FLOOR(y));
		//cout << x << "  " << FLOOR(x) << endl;
	}

	glEnd();
	glFlush();
}

void createGrid(void){

	
	for (int i = 0; i < 800; i+=4){
		glVertex2i(i, 0);
		glVertex2i(i, HEIGHT);
		
		glVertex2i(0, i);
		glVertex2i(WIDTH, i);
	}

}

/*menu 3*/
void benchmark(void){

	//cizilecek toplam cizgiyi random atiyorum
	int interval = rand() % INTERVAL_BEGIN + INTERVAL_END; // 100 - 120 arasinda
	cout << "Total line benchmark --> " << interval << endl;

	/*koordinatlari array de tutuyorum*/
	int** coordinateArr = new int*[interval];
	for (int i = 0; i < interval; ++i)
		coordinateArr[i] = new int[4];

	
	for (int i = 0; i < interval; i++){
		for (int j = 0; j < 4; j++){
			coordinateArr[i][j] =(rand() % 500 + 1);
		}
	}

	clock_t startTime = clock();
	cout << "DDA Algorithm" << endl;
	for (int i = 0; i < interval; i++){
		X1 = coordinateArr[i][0];
		Y1 = coordinateArr[i][1];
		X2 = coordinateArr[i][2];
		Y2 = coordinateArr[i][3];
		cout << X1 << " , " << Y1 << " , " << X2 << " , " << Y2 << endl;
		DDAAlgorithm();
	}
	clock_t endTime = clock();

	clock_t clockTicksTaken = endTime - startTime;
	double timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
	cout << "DDA Algortihm Time --> " << timeInSeconds<<endl;
	
	clock_t startTime1 = clock();
	cout << "Bresenham Algorithm" << endl;
	for (int i = 0; i < interval; i++){
		X1 = coordinateArr[i][0];
		Y1 = coordinateArr[i][1];
		X2 = coordinateArr[i][2];
		Y2 = coordinateArr[i][3];
		cout << X1 << " , " << Y1 << " , " << X2 << " , " << Y2 << endl;
		bresenhamAlgorithm();
	}
	clock_t endTime1 = clock();

	clock_t clockTicksTaken1 = endTime1 - startTime1;
	double timeInSeconds1 = clockTicksTaken1 / (double)CLOCKS_PER_SEC;
	cout << "Bresenham Algortihm Time --> " << timeInSeconds1 << endl;


	if (timeInSeconds1 > timeInSeconds)
		cout << "DDA Algortihm daha hizli" << endl;
	else
		cout << "Bresenham Algortihm daha hizli" << endl;


	for (int i = 0; i < interval; ++i)
		delete[] coordinateArr[i];
}

/*Program menulerini ekrana basar*/
void printMenu(void){
	/*Ekrani temizle*/
	system("CLS");

	/*dogru secim yapilana kadar dongu doner*/
	while (choose==0){

		cout << "----------------------------\n";
		cout << "Choose from the menu\n";
		cout << "----------------------------\n";
		cout << "1-Use DDA Algorithm\n";
		cout << "2-Use Bresenham Algorithm\n";
		cout << "3-Start Benchmark\n";
		cout << "4-Exit\n";
		cout << "----------------------------\n";
		cout << "Enter menu number:";

		cin >> choose;
		switch (choose){
			case 1:
				cout << "You have chosen DDA Algorithm\n";

				break;
			case 2:
				cout << "You have chosen Bresenham Algorithm\n";
				break;
			case 3:
				cout << "You have chosen Benchmark\n";
				break;
			case 4:
				exit(0);
				break;

			default:
				cout << "There is no such menu,please try again\n";
				choose = 0;
			
		}
	}
}