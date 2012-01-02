#include "game.h"
#pragma comment(lib, "winmm.lib")
using namespace std;

class Ghost
{
 public:
	bool life;
	double i;
	double j;
	char go;

	Ghost(int ghq, double ghi, double ghj){
		life = true;
		i = ghi;
		j = ghj;
	}

	Ghost(){};
};

double angle = 45.0;

double eyeX = 32;
double eyeY = 14;
double eyeZ = 40;

double atX = 15;
double atY = 14;
double atZ = 0;

Ghost Ghosts [4];

double pm = 0;
int pmi = 23;
int pmj = 14;
char pml = ' ';
char lastpml = 'l';
int pmQ=1;
int pmrot = 0;
int pmSwpAng = 240;
int pmStrAng = 60;
bool pmLive = true;
int pmLife = 3;

double bigFoodSize = 0.25;

bool ride = false;

int game [31][28];
int level = 1;

int n = 0;

int tmpTime = 0 ;

bool pmEating = false;

int score = 0 ;

double ghostSpeed= 0.1;

bool gameOver = false;

void otherWall(){
	glPushMatrix();
	glRotatef(90,1,0,0);
	gluCylinder(gluNewQuadric(),.25,.25,1,20,20);
	glPopMatrix();
}

void wall(){
	/*static GLuint eboxTexture = LoadTexture("ie.ppm", 275, 183, false);
	GLUquadricObj* esphere = gluNewQuadric();  
	gluQuadricTexture(esphere, true);
	gluQuadricNormals(esphere, GLU_SMOOTH);
	glEnable(GL_TEXTURE_2D);  
	glBindTexture(GL_TEXTURE_2D, eboxTexture);
	glEnable(GL_CULL_FACE);
	
	gluCylinder(gluNewQuadric(),.5,0,1,20,20);

	gluDeleteQuadric(esphere);

	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_CULL_FACE);*/
	
	
	gluCylinder(gluNewQuadric(),.5,0,1,20,20);
}

void food(){
	gluSphere(gluNewQuadric(),.2,20,20);
}

void bigFood(){
	gluSphere(gluNewQuadric(),.4,20,20);
}

void ghost(int n){
	glPushMatrix();

	glTranslatef(Ghosts[n].i,Ghosts[n].j,0);

	switch(n) {
		case 1 : glColor3f(1,0,0); break;
		case 2 : glColor3f(1,0,1); break;
		case 3 : glColor3f(0,1,1); break;
		case 4 : glColor3f(1,1,0); break;
	}

	if(pmEating)
		glColor3f(1,1,1);

	gluCylinder(gluNewQuadric(),0.5,0.35,0.5,20,20);
	glTranslated(0.0,0,0.5);
	gluCylinder(gluNewQuadric(),0.35,0,0.5,20,20);

	glColor3f(1,.5,0);
	glTranslated(0.0,0,-0.8);
	gluCylinder(gluNewQuadric(),0.01,0.12,0.5,20,20);

	glColor3f(1,.25,0);
	glTranslated(0.2,0,0);
	gluCylinder(gluNewQuadric(),0.01,0.12,0.5,20,20);

	glColor3f(.5,1,0);
	glTranslated(-0.4,0,0);
	gluCylinder(gluNewQuadric(),0.01,0.12,0.5,20,20);

	glPopMatrix();
}

void pacMan(){
	glColor3f(1,0,0);
	glRotatef(pmrot,0,0,1);
	gluPartialDisk(gluNewQuadric(),0,.5,20,20,pmStrAng,pmSwpAng);
	glTranslatef(0,-.1,0);
	glColor3f(1,1,1);
	gluDisk(gluNewQuadric(),.05,.15,20,20);
	glColor3f(0,0,0);
	gluDisk(gluNewQuadric(),.0,.05,20,20);
}

void drawWalls(){
	for(int i=0;i<31;i++){
		glPushMatrix();
		glTranslatef(i,0,0);
		for(int j=0;j<28;j++){			
			glTranslatef(0,1,0);
			if(game[i][j] == -1){
				glColor3f(.40,.26,.13);
				wall();
			}
			if(game[i][j] == -2){
				glPushMatrix();
				glTranslatef(0,.5,.25);
				glColor3f(0,1,0);
				otherWall();
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
}

void drawFood(){
	for(int i=0;i<31;i++){
		glPushMatrix();
		glTranslatef(i,0,0);
		for(int j=0;j<28;j++){			
			glTranslatef(0,1,0);
			if(game[i][j] == 5){
				glColor3f(1,1,0);
				food();
			}
			if(game[i][j] == 1){
				glColor3f(0,1,0);

				if(bigFoodSize >= 1.75)
					bigFoodSize = 0.25;

				if(bigFoodSize>1)
					glScalef(2-bigFoodSize,2-bigFoodSize,2-bigFoodSize);
				else
					glScalef(bigFoodSize,bigFoodSize,bigFoodSize);

				bigFood();

				if(bigFoodSize>1)
					glScalef(1/(2-bigFoodSize),1/(2-bigFoodSize),1/(2-bigFoodSize));
				else
					glScalef(1/bigFoodSize,1/bigFoodSize,1/bigFoodSize);
				bigFoodSize += 0.02;
			}
		}
		glPopMatrix();
	}
}

void newLife(){
	if(pmLife < 1 && !gameOver){
		stringstream s;
		s <<"GAME OVER \n You Final score is ";
		s << score;
		string x = s.str();
		MessageBoxA(NULL, x.c_str(), "Sorry!!", MB_OK);
		gameOver = true;
	}else {
		pmLife--;
		pmi = 23;
		pmj = 14;
		pmLive = true;
	}
}

void drawPacMan(){	
	for(int i=0;i<4;i++){
		if(abs((Ghosts[i].i)-pmi)<=.5 && abs((Ghosts[i].j-1)-pmj)<=.5){
			if(pmEating){
				Ghosts[i].life = false;
				Ghosts[i].i = 13;
				Ghosts[i].j = 14;
				score+=200;
				PlaySound(L"pacman_eatghost", 0, SND_ASYNC);
			} else {
				pmLive = false;
				PlaySound(L"pacman_death", 0, SND_ASYNC);
				newLife();
			}
		}
	}
	
	glPushMatrix();


	switch (pml) {
		case ' ': 
			glTranslatef(pmi,pmj+1,0);
			pmSwpAng = 240;
			pmStrAng = 60;
			break;
		case 'r':
			glTranslatef(pmi,pmj+pm+1,0); 
			if(pmj<27)
				if( game[pmi][pmj+1] >-1){
					if(pm == .75){
						pm = 0;		
						game[pmi][pmj] = 0;
						if(game[pmi][pmj+1]%4==1){
							if(pmSwpAng>=360) {
								pmStrAng = 60;
								pmSwpAng = 240;
							} else {
								pmStrAng -= 30;
								pmSwpAng += 60;
							}
							if(game[pmi][pmj+1]==1){
								pmEating = true;
								score += 40;
								tmpTime = 0;
								ghostSpeed =0.05;
							}
							score+=10;
							n--;
							PlaySound(L"pacman_eatfruit", 0, SND_ASYNC);
						} else { 
							pmStrAng = 60;
							pmSwpAng = 240;
						}
						game[pmi][pmj+1] = 3;
						pmj++;
					} else {
						pm+=.25;
					}
				} else {
					pml = ' ';
				}
			else{
				game[pmi][pmj] = 0;
				game[pmi][0] = 3;
				pmj = 0;
			}
			break;
		case 'l':
			glTranslatef(pmi,pmj-pm+1,0); 
			if(pmj!=0)
				if( game[pmi][pmj-1] >-1){
					if(pm == .75){
						pm = 0;
						game[pmi][pmj] = 0;
						if(game[pmi][pmj-1]%4==1){
							if(pmSwpAng>=360) {
								pmStrAng = 60;
								pmSwpAng = 240;
							} else {
								pmStrAng -=30;
								pmSwpAng += 60;
							}
							if(game[pmi][pmj-1]==1){
								pmEating = true;
								score+=40;
								tmpTime = 0;
								ghostSpeed =0.05;
							}
							score+=10;
							n--;
							PlaySound(L"pacman_eatfruit", 0, SND_ASYNC);
						} else { 
							pmStrAng = 60;
							pmSwpAng = 240;
						}
						game[pmi][pmj-1] = 3;
						pmj--;
					} else {
						pm+=.25;
					}
				} else {
					pml = ' ';
				}
			else{
				game[pmi][pmj] = 0;
				game[pmi][27] = 3;
				pmj = 27;
			}
			break;
		case 'u':
			glTranslatef(pmi-pm,pmj+1,0); 
			if(game[pmi-1][pmj] >-1){
				if(pm == .75){
					pm = 0;
					game[pmi][pmj] = 0;
					if(game[pmi-1][pmj]%4==1){		
						if(pmSwpAng>=360) {
							pmStrAng = 60;
							pmSwpAng = 240;
						} else {
							pmStrAng -=30;
							pmSwpAng += 60;
						}
						if(game[pmi-1][pmj]==1){
							pmEating = true;
							score += 40;
							tmpTime = 0;
							ghostSpeed =0.05;
						}
						score+=10;
						n--;
						PlaySound(L"pacman_eatfruit", 0, SND_ASYNC);
					} else { 
						pmStrAng = 60;
						pmSwpAng = 240;
					}
					game[pmi-1][pmj] = 3;
					pmi--;
				} else {
					pm+=.25;
				}
			} else {
				pml = ' ';
			}
			break;
		case 'd':
			glTranslatef(pmi+pm,pmj+1,0); 
			if(game[pmi+1][pmj] >-1){
				if(pm == .75){
					pm = 0;
					game[pmi][pmj] = 0;
					if(game[pmi+1][pmj]%4==1){
						if(pmSwpAng>=360) {
							pmStrAng = 60;
							pmSwpAng = 240;
						} else {
							pmStrAng -=30;
							pmSwpAng += 60;
						}
						if(game[pmi+1][pmj]==1){
							pmEating = true;
							score+=40;
							ghostSpeed =0.05;
							tmpTime = 0;
						}
						score+=10;
						n--;
						PlaySound(L"pacman_eatfruit", 0, SND_ASYNC);
					} else { 
						pmStrAng = 60;
						pmSwpAng = 240;
					}
					game[pmi+1][pmj] = 3;
					pmi++;
				} else {
					pm+=.25;
				}
			} else {
				pml = ' ';
			}
			break;
	}
	pacMan();
	glPopMatrix();
}

void nextMoveGhost(int nn){
	if(Ghosts[nn].j < 1.2 ){
		Ghosts[nn].j = 27.5;
		return;
	}

	if(Ghosts[nn].j > 27.8 ){
		Ghosts[nn].j = 1.5;
		return;
	}

	if(int(Ghosts[nn].i*10)%10!=0||int(Ghosts[nn].j*10)%10!=0){
		if(Ghosts[nn].go == 'l'){
			Ghosts[nn].j -= ghostSpeed;
		}
		if(Ghosts[nn].go == 'r'){
			Ghosts[nn].j += ghostSpeed;
		}
		if(Ghosts[nn].go == 'u'){
			Ghosts[nn].i -= ghostSpeed;
		}
		if(Ghosts[nn].go == 'd'){
			Ghosts[nn].i += ghostSpeed;
		}
		return;
	}

	srand (time(NULL));
	string str = "";
	int c = 0;

	if(game[(int)Ghosts[nn].i][(int)Ghosts[nn].j-2] != -1 && Ghosts[nn].go!='r'){
		c++;
		str += "l";
	}
	if(game[(int)Ghosts[nn].i][(int)Ghosts[nn].j] != -1 && Ghosts[nn].go!='l'){
		c++;
		str += "r";
	}
	if(game[(int)Ghosts[nn].i-1][(int)Ghosts[nn].j-1] != -1 && Ghosts[nn].go!='d'){
		c++;
		str += "u";
	}
	if(game[(int)Ghosts[nn].i+1][(int)Ghosts[nn].j-1] != -1 && Ghosts[nn].go!='u'){
		c++;
		str += "d";
	}
	
	if(c!=0){
		c = rand() % c;
		Ghosts[nn].go = str.at(c);
	}else{
		if(Ghosts[nn].go == 'l'){
			Ghosts[nn].go = 'r';
		}
		if(Ghosts[nn].go == 'r'){
			Ghosts[nn].go = 'l';
		}
		if(Ghosts[nn].go == 'u'){
			Ghosts[nn].go = 'd';
		}
		if(Ghosts[nn].go == 'd'){
			Ghosts[nn].go = 'u';
		}
	}

	if(Ghosts[nn].go == 'l'){
		Ghosts[nn].j -= ghostSpeed;
	}
	if(Ghosts[nn].go == 'r'){
		Ghosts[nn].j += ghostSpeed;
	}
	if(Ghosts[nn].go == 'u'){
		Ghosts[nn].i -= ghostSpeed;
	}
	if(Ghosts[nn].go == 'd'){
		Ghosts[nn].i += ghostSpeed;
	}
}

void drawGhosts(){	
	for(int i=0;i<4;i++) {
		nextMoveGhost(i);
		ghost(i);
	}	
}

void ridepm(){
	if(pml == 'r'){
		eyeX = pmi;
		eyeY = pmj+pm-5;
		eyeZ = 2;
		atX = eyeX;
		atY = eyeY+5;
		atZ = 0;
	}
	if(pml == 'l'){
		eyeX = pmi;
		eyeY = pmj-pm+6;
		eyeZ = 2;
		atX = eyeX;
		atY = eyeY-5;
		atZ = 0;
	}
}

void timerFunction(int arg)
{
	if(pmEating){
		if(tmpTime >= 4000){
			pmEating = false;
			tmpTime = 0 ;
			ghostSpeed =0.1;
		}else{
			tmpTime+=25;
		}
	} else {
		tmpTime = 0 ;
	}
	glutPostRedisplay();
}

void drawBG(){
	glPushMatrix();

	glColor3f(1.0f,1.0f,1.0f);

	static GLuint eboxTexture = LoadTexture("green.ppm", 1280, 800, false);
	GLUquadricObj* esphere = gluNewQuadric();  
	gluQuadricTexture(esphere, true);
	gluQuadricNormals(esphere, GLU_SMOOTH);
	glEnable(GL_TEXTURE_2D);  
	glBindTexture(GL_TEXTURE_2D, eboxTexture);
	glEnable(GL_CULL_FACE);
	
	//glTranslatef(15.5,14,0);
	//gluSphere(esphere,15,100,100);

	glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0);	glVertex3f( 2.0f, 2.0f, 5.0f);
      glTexCoord2f(1.0,0.0);	glVertex3f( 32.0f, 2.0f, 5.0f);
      glTexCoord2f(1.0,1.0);	glVertex3f( 32.0f, 27.0f, 5.0f);
      glTexCoord2f(0.0,1.0);	glVertex3f( 2.0f, 27.0f, 5.0f);
    glEnd();

	gluDeleteQuadric(esphere);

	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_CULL_FACE);

	glPopMatrix();
}

void displayScore(){
	glColor3f(1,1,1);
	std::string s;
	std::string u = "Current Score :: ";
	std::stringstream out;
	out << score;
	s = out.str(); 

	int len = u.size();
	glRasterPos2f( 0,-7);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, u.at(i));
    }
	len = s.size();
	glRasterPos2f( 1,-5);
    for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s.at(i));
    }
}

void displayExit(){
	glColor3f(1,1,1);
	std::string u = "Exit";

	int len = u.size();
	glRasterPos2f( 10,-7);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, u.at(i));
    }

}

void displayLifes(){
	glPushMatrix();
	glTranslatef(31,-7,0);
	for(int i=0; i<pmLife ;i++){
		glTranslatef(0,1,0);
		glColor3f(1,0,0);
		gluPartialDisk(gluNewQuadric(),0,.5,20,20,60,240);
		glColor3f(1,1,1);
		gluDisk(gluNewQuadric(),.05,.15,20,20);
		glColor3f(0,0,0);
		gluDisk(gluNewQuadric(),.0,.05,20,20);
	}
	glPopMatrix();
}

void drawDDPacMan(){
	for(int j=240;j<360;j+=10){
		for(int i=60;i<360;i+=10){
			glColor3f(1,0,0);
			gluPartialDisk(gluNewQuadric(),0,5,20,20,i,j);
			glFlush();
			glColor3f(1,1,1);
			gluPartialDisk(gluNewQuadric(),0,5,20,20,i,j);
			glFlush();
			glColor3f(0,0,0);
			gluPartialDisk(gluNewQuadric(),0,5,20,20,i,j);
			glFlush();
		}
	}
	newLife();
}

void keyb(unsigned char c, int a, int b){		
	switch (c) {
		case 27 : exit(0); break;

		case 'x' : atX++; break;
		case 'X' : atX--; break;

		case 'y' : atY++; break;
		case 'Y' : atY--; break;

		case 'z' : atZ++; break;
		case 'Z' : atZ--; break;

		case '+' : eyeZ--; atZ--; break;
		case '-' : eyeZ++; atZ++;break;

	}
	
}

void processSpecialKeys(int key, int x, int y){
	if(key == GLUT_KEY_END)
		exit(0);
	if(key == GLUT_KEY_RIGHT){
		if(pml == 'l') pm = 0;
		if(game[pmi][pmj+1] >-1){
			pml = 'r';
			pmrot = 0;
		}
	}
	if(key == GLUT_KEY_LEFT){
		if(pml == 'r') pm = 0;
		if(game[pmi][pmj-1] >-1){
			pml = 'l';
			pmrot = 180;
		}
	}
	if(key == GLUT_KEY_UP){
		if(pml == 'd') pm = 0;
		if(game[pmi-1][pmj] >-1){
			pmrot = 90;
			pml	= 'u';
		}
	}
	if(key == GLUT_KEY_DOWN){
		if(pml == 'u') pm = 0;
		if(game[pmi+1][pmj] >-1){
			pmrot = -90;
			pml = 'd';
		}
	}
	
}

void mouse(int but, int str, int x, int y){
	if(but == GLUT_LEFT_BUTTON && str == GLUT_UP){
		if(x>=163&&x<193&&y>=287&&y<301){
			exit(0);
		}
	}

}

void read(){
	// 0 --> nothing
	// -2 --> other wall
	// -1 --> wall
	// 5 --> food
	// 1 --> big food
	// 2 --> ghost
	// 3 --> pacMan
	string fileName;
	switch (level){
	case 1 :
		 fileName = "pacman1.txt";
		 break;
	case 2 :
		 fileName = "pacman2.txt";
		 break;
	}
	
	
	ifstream traffic (fileName);
	if(traffic.is_open()){
		cout << "START READING" << endl;
		string str;
		for (int i=0; traffic.good() && i<31; i++)
		{
			traffic >> str;
			for(int j = 0 ; j<str.length() ; j++){
				switch (str.at(j)){
					case 'x' : 
						game[i][j] = -1;
						game[i][27-j] = -1;
						break;
					case '.' :
						game[i][j] = 5;
						game[i][27-j] = 5;
						break;
					case 'o' :
						game[i][j] = 1;
						game[i][27-j] = 1;
						break;
					case '-' :
						game[i][j] = -2;
						game[i][27-j] = -2;
						break;
				}				
			}
		}

		game[11][13] = 2;
		game[13][11] = 2;
		game[13][13] = 2;
		game[13][15] = 2;

		Ghosts[0] = Ghost(1,11,14);
		Ghosts[1] = Ghost(2,13,12);
		Ghosts[2] = Ghost(3,13,14);
		Ghosts[3] = Ghost(4,13,16);

		game[pmi][pmj] = 3;

		cout << "DONE READING" << endl;
		traffic.close();

		for(int i=0;i<31;i++){
			for(int j=0;j<28;j++){
				switch (game[i][j]){
					case -2 : cout << "-" ; break; 
					case -1 : cout << "#" ; break;
					case 0 : cout << " " ; break;
					case 1 : cout << "o" ; n++; break;
					case 2 : cout << "@" ; break;
					case 3 : cout << "*" ; break;
					case 5 : cout << "." ; n++; break;  // counting number of food !!!
				}
			}
			cout << endl;
		}
	} else {
		cout << "can't open the file" << endl;
	}

	cout << "n = " << n << endl;

}

void displaySL(){
	if(level>2){
		stringstream s;
		s <<"Thank you for playing Pac-Man 3D \n you finish with score :: ";
		s << score;
		string x = s.str();
		MessageBoxA(NULL, x.c_str(), "Congratulations!", MB_OK);
		return;
	}
	level++;
	stringstream s;
	s <<"Going to Next Level";
	s << level;
	string x = s.str();
	MessageBoxA(NULL, x.c_str(), "Congratulations!", MB_OK);	
	read();
}

void Display(){
	glMatrixMode(GL_PROJECTION); // ...
	glLoadIdentity();
	gluPerspective(angle,1.5,0,40);
	glMatrixMode(GL_MODELVIEW); // ...
	glLoadIdentity();
	if(ride) 
		ridepm();
	else
		lastpml = ' ';

	gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, 0, 0, 1);

	GLfloat LightAmbient[] = { 1.0, .0, .0, 0 };    
	GLfloat LightDiffuse[] = { 1, 0.1, 0.1, 0 };  
	GLfloat LightPosition[] = { pmi, pmj+1, -2, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);   
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);   
	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition); 
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 75.f);
	GLfloat spot_direction[] = { .0, .0, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	GLfloat LightAmbient7[] = { 0, 01, 0.0, 1.0 }; 
	GLfloat LightDiffuse7[] = { 0.1, 0.1, .5, 1.0 };
	GLfloat LightPosition7[] = { 1.5, 1.5, 1.5, 0.0 };
	glLightfv(GL_LIGHT7, GL_AMBIENT, LightAmbient7);   
	glLightfv(GL_LIGHT7, GL_DIFFUSE, LightDiffuse7);   
	glLightfv(GL_LIGHT7, GL_POSITION,LightPosition7);  

	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, .05);

	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);	

	
	if(n!=0){

		drawBG();

		glEnable(GL_LIGHTING);

		glEnable(GL_LIGHT0);
		drawWalls();		
		glDisable(GL_LIGHT0);

		glEnable(GL_LIGHT7);
		drawGhosts();
		glDisable(GL_LIGHT7);

		glDisable(GL_LIGHTING);

		drawGhosts();
		drawFood();		
		if(!gameOver)
			if(pmLive)
				drawPacMan();
			else
				drawDDPacMan();

	}else{
		displaySL();
	}

	displayLifes();

	displayScore();

	displayExit();

	glFlush();

	glutTimerFunc(25,timerFunction,0);
}

void main(int argc,char** argr)
{
	glutInit(&argc,argr);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	 glClearDepth(1.0); 
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	glutCreateWindow("PacMan");
	glutFullScreen();
	
	PlaySound(L"pacman_beginning", 0, SND_ASYNC);

	read();
	
	glutKeyboardFunc(keyb);
	glutSpecialFunc( processSpecialKeys );
	glutMouseFunc(mouse);

	glClearColor(.0f,.0f,.0f,.0f);	
	glutDisplayFunc(Display);
	
	glutMainLoop();
}
