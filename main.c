#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define S_WIDTH 600
#define S_HEIGHT 400

unsigned char textureData[S_WIDTH][S_HEIGHT][3];
int grid[S_WIDTH][S_HEIGHT];

unsigned int bin_color(int value){
	if(value)
		return 255;
	else
		return 0;
}

void renderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	int i,j,k,l;

	int next_grid[S_WIDTH][S_HEIGHT];
	for(i = 0; i < S_WIDTH; i++){
		for(j = 0; j < S_HEIGHT; j++){
			int count= 0;
			for(k = -1; k<1;k++){
				for(l = -1; l<1; l++){
					count += grid[(i+k+S_WIDTH)%S_WIDTH][(j+l+S_HEIGHT)%S_HEIGHT];
				}
			}
			count -= grid[i][j];

			int now_state = grid[i][j];

			if(!now_state && count == 3){
				next_grid[i][j] = 1;
			}else if(now_state && (count < 2 || count > 3)){
				next_grid[i][j] = 0;
			}else{
				next_grid[i][j] = now_state;
			}
		}
	}
	
	for(i = 0; i < S_WIDTH; i++){
		for(j = 0; j < S_HEIGHT; j++){
			grid[i][j] = next_grid[i][j];
		}
	}

	// Texture update
	for(i = 0; i < S_WIDTH; i++){
		for(j = 0; j < S_HEIGHT; j++){
			textureData[i][j][0] = textureData[i][j][1] = textureData[i][j][2] = bin_color(grid[i][j]);
		}
	}
		
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, S_WIDTH, S_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (void*) textureData);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
		glTexCoord2f(1.0, 0.0); glVertex2f( 1.0, -1.0);
		glTexCoord2f(1.0, 1.0); glVertex2f( 1.0,  1.0);
		glTexCoord2f(0.0, 1.0); glVertex2f(-1.0,  1.0);
	glEnd();

	glutSwapBuffers();
}

void framebuffer_size_callback(int w, int h){
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);

}

void setup(){
	glClearColor(0.2, 0.2, 0.4, 1.0);

	srand(time(NULL));

	int i,j;

	for(i = 0; i < S_WIDTH; i++){
		for(j = 0; j < S_HEIGHT; j++){
			grid[i][j] = rand()%2;
			textureData[i][j][0] = textureData[i][j][1] = textureData[i][j][2] = bin_color(grid[i][j]);
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, S_WIDTH, S_HEIGHT, GL_FALSE, GL_RGB, GL_UNSIGNED_BYTE, (void*)textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	 glEnable(GL_TEXTURE_2D);

}

void timerFunction(int val){
	glutPostRedisplay();

	glutTimerFunc(33, timerFunction, 1);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(S_WIDTH, S_HEIGHT);
    glutCreateWindow("The Game Of Life");
    glutDisplayFunc(renderScene);
    glutReshapeFunc(framebuffer_size_callback);
	glutTimerFunc(10, timerFunction, 1);
	setup();

	glutMainLoop();

	return 0;
}
