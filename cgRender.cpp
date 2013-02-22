#include <cgRender.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

const GLfloat thing = 1.0f;

int no_of_points;
vector< vector<float> > points;
vector< vector<float> > texture_points;
vector<vector <float> > vertex_normals;

int no_of_polygons;
vector< vector <int> > polygons;

int width;
int height;
unsigned char* texture;

float rotate = 0.0f;

float av_point[3];

void averagePoint() {
    av_point[0] = 0; av_point[1] = 0; av_point[2] = 0;
    for(int i = 0; i < no_of_points; i++) {
        av_point[0] += points[i][0];
        av_point[1] += points[i][1];
        av_point[2] += points[i][2];
    }
    av_point[0] /= no_of_points;
    av_point[1] /= no_of_points;
    av_point[2] /= no_of_points;
}

void normalise(float v[3]) {
    GLfloat length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= length;
    v[1] /= length;
    v[2] /= length;
}

/*
 *  * Performs cross product of vectors u and v and stores
 *   * result in result
 *    * Normalises result.
 *     */
void crossProduct(float u[3], float v[3], float result[3]) {
    result[0] = u[1] * v[2] - u[2] * v[1];
    result[1] = u[2] * v[0] - u[0] * v[2];
    result[2] = u[0] * v[1] - u[1] * v[0];
}

/*
 *  * Calculates normal for plane
 *   */
void calculate_normal(vector<int> polygon, float normal[3]) {
    GLfloat u[3], v[3];
    for (int i = 0; i < polygon.size(); i++) {
        u[i] = points[polygon[0]][i] - points[polygon[1]][i];
        v[i] = points[polygon[2]][i] - points[polygon[1]][i];
    }
    crossProduct(u, v, normal);
    normalise(normal);
}

void calculate_vertex_normals()
{
    vertex_normals = vector< vector<float> >(no_of_points);
    vector< vector<int> > in_faces(no_of_polygons);
    vector< vector<float> > face_normals(no_of_polygons);
    // Face normals
    for (int i = 0; i < no_of_polygons; i++) {
        vector<float> normal(3);
        calculate_normal(polygons[i], &normal[0]);
        for (int j = 0; j < polygons[i].size(); j++) {
            in_faces[polygons[i][j]].push_back(i);
        }
        face_normals[i] = normal;
    }

    // Vertex normals
    for (int i = 0; i < no_of_points; i++) {
        vector<int> &faces = in_faces[i];
        int faces_count = 0;
        vector<float> normal(3);
        for (vector<int>::iterator it = faces.begin(); it != faces.end(); ++it){
            normal[0] += face_normals[*it][0];
            normal[1] += face_normals[*it][1];
            normal[2] += face_normals[*it][2];
            faces_count++;
        }
        normalise(&normal[0]);

        vertex_normals[i] = normal;
    }
}
void init() 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  cout << "init" << endl;

  
  glShadeModel (GL_SMOOTH);
  
  // Enable lighting
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);

  GLfloat light_pos[] = {-3.0, 0, 1.0, 0.1};
 //-0.25, -0.2, 0.8,


  GLfloat light_ka[] = {0.8, 0.8, 0.8, 1.0};
  GLfloat light_kd[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_ks[] = {1.0, 1.0, 1.0, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ka);
  //glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_kd);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, light_ks);

  //Initialise and set material parameters
  GLfloat material_ka[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat material_kd[] = {0.43, 0.47, 0.54, 1.0};
  GLfloat material_ks[] = {0.33, 0.33, 0.33, 1.0};
  GLfloat material_ke[] = {0.1, 0.1, 0.1, 0.1};
  GLfloat material_se[] = {3.0};

  glMaterialfv(GL_FRONT, GL_AMBIENT,  material_ka);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,  material_kd);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  material_ks);
  glMaterialfv(GL_FRONT, GL_EMISSION,  material_ke);
  glMaterialfv(GL_FRONT, GL_SHININESS, material_se);

  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);

}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cout << "display" << endl;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, texture);
  for (int i = 0; i < no_of_polygons; i++) {
    glBegin(GL_POLYGON);
    for (int j = 0; j < polygons[i].size(); j++) {
      int vertex_index = polygons[i][j];
      // Define texture coordinates of vertex
      vector<float> tex_point = texture_points[vertex_index];
      glTexCoord2f(tex_point[0], tex_point[1]);
      
      // Define normal
      vector<float> normal = vertex_normals[vertex_index];
      glNormal3f(normal[0], normal[1], normal[2]);
      
      // Define coordinates of vertex
      vector<float> vertex = points[vertex_index];
      glVertex3f(vertex[0], vertex[1], vertex[2]);
    }
    glEnd();
  }
  //glFlush ();
  //  or, if double buffering is used,
    glutSwapBuffers();
 
}

void reshape (int w, int h)
{
  cout << "reshape" << endl;

  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(10, 1, 0, 10);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-0.3, -0.2, 1.0,
            av_point[0], av_point[1], av_point[2], 
            0, 1.0, 0.0);
  
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27: // ESC
    //exit(0);
     glTranslatef(0.0f,0.0f,-5.0f);
     glRotatef(rotate,0.0f,0.0f,1.0f);
     rotate += 0.05f;
    break;
  }
}


void load_points(const char *filename)
{
    ifstream in;
    in.open(filename);
    string s;

    while (in >> s) {
        if (s == "POINTS") {
            in >> s;
            no_of_points = atoi(s.c_str());
            in >> s;
            break;
        }
    }
   
    points =  vector< vector<float> >(no_of_points);
    int i = 0;
    while (in >> s) {
        if (s == "POLYGONS")
            break;
        points[i] = vector<float>(3);
        points[i][0] = atof(s.c_str());
        in >> s;
        points[i][1] = atof(s.c_str());
        in >> s;
        points[i][2] = atof(s.c_str());
        i++;
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
    no_of_polygons = atoi(s.c_str());
    polygons = vector< vector<int> >(no_of_polygons);
    in >> s;
    
    int i = 0;
    while (in >> s) {
        if (s == "POINT_DATA")
            break;

        int size = atoi(s.c_str());
        polygons[i] = vector<int>(size);
        for (int j = 0; j < size; j++) {
            in >> s;
            polygons[i][j] = atoi(s.c_str());
        }
        i++;
    }
}

void load_textures (const char *filename) {
    ifstream in;
    in.open(filename);
    string s;

    while (in >> s) {
        if (s == "TEXTURE_COORDINATES")
            break;
    }

    in >> s;
    in >> s;
    in >> s;
    
    texture_points = vector< vector<float> >(no_of_points);
    int i = 0;
    while (in >> s) {
        texture_points[i] = vector<float>(2);
        texture_points[i][0] = atof(s.c_str());
        in >> s;
        texture_points[i][1] = atof(s.c_str());
        i++;
    }
}

void init_texture(char* filename) {

    FILE* fp;
    char line[70];
    fp = fopen(filename, "rb");
   
    //Strip first line
    fgets(line, 70, fp);
   
    fgets(line, 70, fp);
    sscanf(line, "%d %d", &width, &height);
    fgets(line, 70, fp);

    texture = new unsigned char[width * height * 3];
    fread(texture, sizeof (unsigned char), width * height * 3, fp);
    fclose(fp);
}

int main(int argc, char** argv)
{
  load_points("../data/face.vtk");
  load_polygons("../data/face.vtk");
  load_textures("../data/face.vtk");
  init_texture("../data/face.ppm");
  averagePoint();
  calculate_vertex_normals();
  cout << "x: "<< av_point[0] << " y:" << av_point[1] << " z:" << av_point[2] << endl;
  
  // Initialize graphics window
  glutInit(&argc, argv);
  //glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
  //  Or, can use double buffering
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

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


