#include <cgRender.h>
#include <sstream>
#include <fstream>
#include <iostream>

const GLfloat thing = 1.0f;

struct point *points;

int **polygons;

void init() 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  cout << "init" << endl;

  
  glShadeModel (GL_SMOOTH);
  
  // Enable lighting
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, &thing);//LightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  &thing);//LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  &thing);//LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, &thing);//LightSpecular);
  
  // Set material parameters
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  &thing);//MaterialSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &thing);//MaterialShininess);
  
  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);
  
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cout << "display" << endl;

  /*
  for (all polygons)
    glBegin(GL_POLYGON);
    for (all vertices of polygon)
      // Define texture coordinates of vertex
      glTexCoord2f(...);
      // Define normal of vertex
      glNormal3f(...);
      // Define coordinates of vertex
      glVertex3f(...);
    }
    glEnd();
  }
  glFlush ();
  //  or, if double buffering is used,
  //  glutSwapBuffers();
  */
}

void reshape (int w, int h)
{
  cout << "reshape" << endl;

  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  /*
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, aspect, near, far);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
  */
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
  }
}


void load_points(const char *filename)
{
    ifstream in;
    in.open(filename);
    string s;
    int no_of_points;

    while (in >> s) {
        if (s == "POINTS") {
            in >> s;
            no_of_points = atoi(s.c_str());
            in >> s;
            cout << no_of_points << endl;
            break;
        }
    }
   
    points = new point[no_of_points];
    int i = 0;
    while (in >> s) {
        if (s == "POLYGONS")
            break;
        points[i].x = atof(s.c_str());
        in >> s;
        points[i].y = atof(s.c_str());
        in >> s;
        points[i].z = atof(s.c_str());
        i++;
        cout << i << endl;
    }
}

void load_polygons(const char *filename){
    ifstream in;
    in.open(filename);
    string s;

    while (in >> s) {
        if (s == "POLYGONS")
            break;
    }
    
    in >> s;
    int no_of_polygons = atoi(s.c_str());
    polygons = new int*[no_of_polygons];
    in >> s;
    
    int i = 0;
    while (in >> s) {
        int size = atoi(s.c_str()); 
        polygons[i] = new int[size];
        for (int j = 0; j < size; j++) {
            in >> s;
            cout << s << endl;
            polygons[i][j] = atoi(s.c_str());
        }
    }
}

int main(int argc, char** argv)
{
  char *filename = argv[1];
  cout << filename << endl;
  load_points(filename);
  load_polygons(filename);
  
  // Initialize graphics window
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
  //  Or, can use double buffering
  //  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

  glutInitWindowSize (256, 256); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);

  // Initialize OpenGL
  init();

  // Initialize callback functions
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);



  // Start rendering 
  glutMainLoop();

}

