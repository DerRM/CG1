#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h"
#include "Hit.h"
#include "RTObject.h"
#include "Mesh.h"
#include "Plane.h"
#include "PPM.h"

using namespace glm;

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

// global variables //////////////////////////////////////////
int _id_window, _id_screen, _id_world;
int _win_w = 256;
int _win_h = 256;
int _win_gap = 8;

vec3 eye(0, 5, 20);
vec3 up(0, 1, 0);
vec3 center(0, 0, 0);
float fovy = 60;
float zNear = 0.1;
float zFar = 100.0;

vec4 _clear_col(0.5,  0.5, 0.5, 1.0);

mat4 projection, modelview, modelview2, modelview2_inv;
GLint viewport[4];

float _view_oldx = 0;
float _view_oldy = 0;
float _view_rotx = 0;
float _view_roty = 0;
bool _view_dofovy = false;
bool _view_doshift = false;
bool _view_motion = false;

float _world_oldx = 0;
float _world_roty = -35;

Mesh mesh;
std::vector<RTObject*> raytraceObjects;

std::vector<vec3> hitPoints;
std::vector<Ray> rays;
std::vector<vec3> rayTracedImage;
GLuint rayTracedImageId = 0;

float _sample_factor = 1;
int _sample_width = 0;
int _sample_height = 0;

int _rays_disp_step = 10;
bool _auto_vis = true;

int _recursions = 0;

enum vis_mode { vis_default, vis_opengl, vis_isecs, vis_lods, vis_N };
int _vis_mode = vis_default;

char* _vis_names[vis_N] = { "raytraced image", "opengl shaded" };


/*********************************************************************/
// raytracing
/*********************************************************************/
void clear_rays()
{
    rays.clear();
    rayTracedImage.clear();
}

// Create rays for each sample of the image
void create_primary_rays(std::vector<Ray>& rays, int resx, int resy)
{
    
	// TODO!
    vec3 start = unProject(vec3(resx, resy, 0), modelview, projection, make_vec4(viewport));
    vec3 end = unProject(vec3(resx, resy, 1), modelview, projection, make_vec4(viewport));
    
    Ray ray = Ray(start, normalize(end - start));
    rays.push_back(ray);
}

bool intersectTriangle(const Ray& ray)
{
    vec3 p0 = (vec3) (modelview * vec4(0, 0, 0, 1.0));
    vec3 p1 = (vec3) (modelview * vec4(10, 0, 0, 1.0));
    vec3 p2 = (vec3) (modelview * vec4(0, 10, 0, 1.0));
    
    vec3 normal = normalize(cross(p0 - p1, p1 - p2));
    float t = - dot(ray.o - p0, normal) / dot(ray.d, normal);
    vec3 x = ray.o + t * ray.d;
    
    if (dot(cross(p1 - p0, x - p0), normal) < 0.0)
    {
        return false;
    }
    
    if (dot(cross(p2 - p1, x - p1), normal) < 0.0)
    {
        return false;
    }
    
    if (dot(cross(p0 - p2, x - p2), normal) < 0.0)
    {
        return false;
    }
    
    return true;
}

vec3 light_pos(-5.0f, 5.0f, 5.0f);

vec3 computeLighting(Hit& hitpoint, vec3 color)
{
    vec3 light_dir = light_pos - hitpoint.hitPoint;
    vec3 norm_light_dir = (vec3) normalize(modelview * vec4(light_dir, 0.0));
    
    vec3 half_vec = normalize(light_dir + eye);
    
	float diffuseTerm = dot(hitpoint.normal, norm_light_dir);
    
    if (diffuseTerm < 0.0f)
    {
        diffuseTerm = 0.0f;
    }
    
    if (diffuseTerm > 1.0f)
    {
        diffuseTerm = 1.0f;
    }
	
	float half_dot_normal = dot(half_vec, hitpoint.normal);
    
	vec4 light_spec = vec4(0.0, 0.0, 0.0, 0.0);
    
	if (half_dot_normal > 0.0)
	{
		light_spec = diffuseTerm * vec4(0.6, 0.6, 0.6, 1.0) * vec4(0.7, 0.7, 0.7, 1.0) * pow(half_dot_normal, 128.0f);
	}
    
	vec4 lightingColor = vec4(color * (1.0f / 5.0f), 1.0) * vec4(0.3, 0.3, 0.3, 1.0) + diffuseTerm * vec4(color * (3.0f / 4.0f), 1.0) * vec4(0.7, 0.7, 0.7, 1.0) + light_spec;
    
    //cout << lightingColor.x << " " << lightingColor.y << " " << lightingColor.z <<  "\n";
    
    hitpoint.color = (vec3) lightingColor;
    
    return (vec3) lightingColor;
}

bool castShadow(Hit& hit)
{
    vec3 lightdir = light_pos - hit.hitPoint;
    vec3 normal_lightdir = normalize(lightdir);
    
    Ray ray(hit.hitPoint, normal_lightdir, 0.5f);

    
    Hit dummy;
    std::vector<RTObject*>::iterator it;
    for (it = raytraceObjects.begin(); it != raytraceObjects.end(); ++it) {
        if ((*it)->hit(ray, dummy))
        {
            return true;
        }
    }
    
    return false;
}

bool castReflection(Hit& hit, Hit& newHit)
{
    vec3 dir = eye - hit.hitPoint;
    float angle = dot(dir, hit.normal);
    vec3 reflectDir = 2.0f * angle * hit.normal - dir;

    Ray reflectRay(hit.hitPoint, reflectDir, 0.01f);
//    reflectRay.o = (vec3) (inverse(modelview) * vec4(reflectRay.o, 1.0));
//    reflectRay.d = (vec3) (inverse(modelview) * vec4(reflectRay.d, 0.0));
    
    std::vector<RTObject*>::iterator it;
    for (it = raytraceObjects.begin(); it != raytraceObjects.end(); ++it) {
        if ((*it)->hit(reflectRay, newHit))
        {
            return true;
        }
    }
    
    return false;
}

bool hitTest(Ray& ray, Hit& hit)
{
    Hit temp;
    double tempDist = DBL_MAX;    
    bool result = false;
    
    std::vector<RTObject*>::iterator it;
    for (it = raytraceObjects.begin(); it != raytraceObjects.end(); ++it) {
        if ((*it)->hit(ray, temp))
        {
            result = true;
            
            double dist = glm::distance(ray.o, temp.hitPoint);
            if (tempDist > dist)
            {
                hit = temp;
                tempDist = dist;
            }
        }
    }
    
    return result;
}

// Ray trace the scene
void ray_trace()
{
	// Number of samples in x and y direction given the sampling factor
    unsigned int w = _sample_factor * _win_w;
    unsigned int h = _sample_factor * _win_h;

    _sample_width = w;
    _sample_height = h;

    std::cout << "raycast: w=" << w << " h=" << h << std::endl;


    for (float j = 0; j < _win_h; j += (1 / _sample_factor))
    {
        for (float i = 0; i < _win_w; i += (1 / _sample_factor))
        {
            create_primary_rays(rays, i, j);
        }
    }

	rayTracedImage.clear();
    rayTracedImage.resize(w*h, vec3(0, 0, 0));
    
	// TODO : write the samples with the correct color (i.e raytrace)
    for(size_t j = 0; j < h; j++)
    {
        for(size_t i = 0; i < w; i++)
        {
            Hit hit;
            Ray ray = rays[i + j * w];
            Ray newRay;
            newRay.o = (vec3) (inverse(modelview) * vec4(ray.o, 1.0));
            newRay.d = (vec3) (inverse(modelview) * vec4(ray.d, 0.0));
            
            if (hitTest(newRay, hit))
            {
                hitPoints.push_back(hit.hitPoint);
                rayTracedImage[i + j * w] = computeLighting(hit, hit.color);
                
                if (castShadow(hit))
                {
                    rayTracedImage[i + j * w] -= vec3(0.5f, 0.5f, 0.5f);
                }
                
                Hit newHit;
                if (castReflection(hit, newHit))
                {
                    rayTracedImage[i + j * w] += computeLighting(newHit, newHit.color);
                }
            }
		}
	}

	// Create an openGL texture if it doesn't exist allready
	if(!rayTracedImageId)
	{
		glGenTextures(1, &rayTracedImageId);
		glBindTexture(GL_TEXTURE_2D, rayTracedImageId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	// Feed with raytraced image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, &rayTracedImage[0]);
}

// Draw the rays shot on the scene
void draw_rays()
{
    for (int i = 0; i < rays.size(); i++) {
        glBegin(GL_LINES);
        {
            glVertex3fv(&rays[i].o[0]);
            vec3 dir = (rays[i].o + rays[i].d * 3.0f);
            glVertex3fv(&dir[0]);
        }
        glEnd();
        
    }
}

// drawing utilities ///////////////////////////////////////
int get_vis_mode()
{
    bool can_show_image = (!_view_motion && !rayTracedImage.empty());
    return !can_show_image && _vis_mode == vis_default ? vis_opengl : _vis_mode;
}

void draw_camera()
{
    const vec3 c[4] =
    {
        vec3( 1,  1, -2),
        vec3( 1, -1, -2),
        vec3(-1, -1, -2),
        vec3(-1,  1, -2)
    };

    glBegin(GL_QUADS);
    glColor3f(0.2,0.2,0.2);
    for (int i = 0; i < 4; i++)
    {
        glVertex3fv(&c[i][0]);
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1,1,0);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(0, 0, 0);
        glVertex3fv(&c[i][0]);
    }
    glEnd();
}

void full_screen_quad(){
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();

	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS); 
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-1, -1, -1); 

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3i(1, -1, -1); 

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3i(1, 1, -1); 

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3i(-1, 1, -1);

	glEnd(); 

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void draw_raytraced_image()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rayTracedImageId);
	glDisable(GL_LIGHTING);
	full_screen_quad();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void draw_string(int x, int y, const std::string& str)
{
    void* font = GLUT_BITMAP_HELVETICA_12;
    const int height = 15;
    const int tab = 18;

    vec3 colors[4] = { vec3(0, 0, 0), vec3(0.0, 1, 0.5), vec3(0.9, 0.3, 0.1), vec3(1, 1, 1) };

    glColor3fv(&colors[0][0]);

    glRasterPos2i(x, y);
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '\n')
        {
            y += height;
            glRasterPos2i(x, y);

            continue;
        }

        if (str[i] == '\t')
        {
            int p[4];
            glGetIntegerv(GL_CURRENT_RASTER_POSITION, p);

            int cx = ((p[0] + tab - 1) / tab) * tab;
            glRasterPos2i(cx, y);
        }

        if (str[i] == '$')
        {
            ++i;

            int k = (int) str[i] - '0';
            if (k < 0 || k >= 4) k = 0;
            glColor3fv(&colors[k][0]);

            // update raster color, by setting current raster position
            int p[4];
            glGetIntegerv(GL_CURRENT_RASTER_POSITION, p);
            glRasterPos2i(p[0], y);

            continue;
        }

        glutBitmapCharacter(font, str[i]);
    }
}

// window drawing ///////////////////////////////////////
void main_reshape(int width,  int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    clear_rays();

    _win_w = (width - 3*_win_gap) / 2.0;
    _win_h = height - 2*_win_gap - 120;
    
    glutSetWindow(_id_screen);
    glutPositionWindow(_win_gap, _win_gap);
    glutReshapeWindow(_win_w, _win_h);

    glutSetWindow(_id_world);
    glutPositionWindow(_win_gap + _win_w + _win_gap, _win_gap);
    glutReshapeWindow(_win_w, _win_h);
}

void main_display()
{
    glClearColor(0.7, 0.7, 0.7, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    vec4 lightDirection(0,0,1,0);

    // material parameters
	GLfloat mat_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_specular[] = {0.4f, 0.4f, 0.4f, 1};
	GLfloat mat_shininess[] = { 128 };
	// light parameters
	GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2 };
	GLfloat light_position[] = { -5.0, 5.0, 5.0, 0.0 };
    
	// Set fixed pipeline opengl material states
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	// Set fixed pipeline opengl light states
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightDirection[0]);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(vec4(0,0,0,1)));
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


    std::ostringstream text;
    text << "mouse \t: rotate scene, mouse+shift: change field of view\n";
    text << "space \t: raytrace scene (also triggered when user changes view or settings)\n";
    text << "tab   \t: change visualization (current: $1" << _vis_names[get_vis_mode()] << "$0)\n";

    std::ostringstream text2;
	text2 << "key s,S\t: +/- sampling factor $2" << _sample_factor << "$0\n";
    text2 << "key i\t: render image to file\n";

    draw_string(_win_gap, _win_gap + _win_h + _win_gap + 12, text.str());
    draw_string(2*_win_gap+_win_w, _win_gap + _win_h + _win_gap + 12, text2.str());

    glutSwapBuffers();
}

void draw_help_stuff()
{
    GLfloat bb[] = {0.0, 0.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bb);
    mesh.renderBoundingBox();
    
    glBegin(GL_POINTS);
    {
        GLfloat point[] = {1.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, point);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, point);

        for (int i = 0; i < hitPoints.size(); i++)
        {
            glVertex3fv(&hitPoints[i][0]);
        }
    }
    glEnd();
}

void draw_scene_openGL()
{    
	glPushMatrix();
    glMultMatrixf(&modelview[0][0]);
    glEnable(GL_LIGHTING);

	// TODO :
	// Draw the preview scene here
//	glBegin(GL_TRIANGLES);
//	glVertex3f(0,0,0);
//	glVertex3f(10,0,0);
//	glVertex3f(0,10,0);
//	glEnd();
    
    draw_help_stuff();
    
    GLfloat model[] = {0.5, 0.5, 0.5, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, model);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, model);
    mesh.renderFlat();
    
    glPopMatrix();
}

void world_reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (float) width / height, 1.0, 256.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, -5.0, -30.0);
    glRotatef(_world_roty, 0.0, 1.0, 0.0);

    glClearColor(_clear_col[0], _clear_col[1], _clear_col[2], _clear_col[3]);
    glEnable(GL_DEPTH_TEST);
}

void world_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glMultMatrixf(&modelview2_inv[0][0]);    

    glDisable(GL_LIGHTING);
    draw_camera();

    glMultMatrixf(&modelview[0][0]);

    draw_rays();

	draw_scene_openGL();

	glutSwapBuffers();
}

void screen_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (float) width/height, zNear, zFar);
    glGetFloatv(GL_PROJECTION_MATRIX, &projection[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, &modelview2[0][0]);
	modelview2_inv = glm::inverse(modelview2);
    glRotatef(_view_rotx, 1, 0, 0);
    glRotatef(_view_roty, 0, 1, 0);
    glGetFloatv(GL_MODELVIEW_MATRIX, &modelview[0][0]);
 
    glClearColor(_clear_col[0], _clear_col[1], _clear_col[2], _clear_col[3]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

}

void screen_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int vis = get_vis_mode();
	// Diplay the ray-traced image
    if (vis == vis_default)
    {
        draw_raytraced_image();
    }
    else
    {
		draw_scene_openGL();
    }

    glutSwapBuffers();
}

void redisplay_all()
{
    glutSetWindow(_id_window);
    glutPostRedisplay();

    glutSetWindow(_id_world);
    world_reshape(_win_w, _win_h);
    glutPostRedisplay();

    glutSetWindow(_id_screen);
    screen_reshape(_win_w, _win_h);
    glutPostRedisplay();
}

// gui interaction /////////////////////////////////////
void screen_mouse(int button, int state, int x, int y)
{
    rays.clear();
    hitPoints.clear();
    
    if (button == GLUT_LEFT && state == GLUT_DOWN)
    {
        _view_motion = true;

        _view_oldx = x;
        _view_oldy = y;
        
        _view_dofovy = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;     
        _view_doshift = (glutGetModifiers() & GLUT_ACTIVE_ALT) != 0;
    }

    if (state == GLUT_UP)
    {
        _view_motion = false;
		ray_trace();
    }

    redisplay_all();
}

void screen_motion(int x, int y)
{
    if (_view_motion)
    {
        if (_view_dofovy)
        {
            fovy += 1.0 * (y - _view_oldy);    
        }
        else if (_view_doshift)
        {
            center[0] += -0.3 * (x - _view_oldx);
            center[1] += 0.3 * (y - _view_oldy);
        }
        else
        {
            _view_roty += -1.0 * (x - _view_oldx);
            _view_rotx += 1.0 * (y - _view_oldy);
        }

        _view_oldx = x;
        _view_oldy = y;
    }

    redisplay_all();
}

void screen_passive_motion(int x, int y)
{
    if (x < 0 || y < 0 || x >= _win_w || y >= _win_h)
    {
        return;
    }
}

void screen_menu(int value)
{
    redisplay_all();
}

void world_mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT && state == GLUT_DOWN)
    {
        _world_oldx = x;
    }
}

void world_motion(int x, int y)
{
    _world_roty += -1.0 * (x - _world_oldx);
    _world_oldx = x;

    redisplay_all();
}

void world_menu(int value)
{
    redisplay_all();
}

void main_keyboard(unsigned char key, int x, int y)
{
	if((int)key == 9)
	{
		_view_motion = !_view_motion;
	}
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'r':
            ray_trace();
            break;
        case 'i':
        {
            PPM ppm(_sample_height, _sample_width);
            ppm.saveImage(rayTracedImage);
            break;
        }
		case 's':
        {
            _sample_factor *= 2;
            std::cout << "sampling factor: " << _sample_factor << std::endl;
            break;
        }
        case 'S':
        {
            _sample_factor *= 0.5;
            std::cout << "sampling factor: " << _sample_factor << std::endl;
            break;
        }
    }

    redisplay_all();
}

void idle()
{
}

int main(int argc, char** argv)
{
	// Init OpenGL stuffs
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024, 600);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    mesh.loadOff("meshes/teapot.off");
    mesh.setColor(vec3(1.0, 0.0, 0.0));

    Plane plane(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0));
    plane.setColor(vec3(0.0, 1.0, 0.0));
    
    raytraceObjects.push_back(&mesh);
    raytraceObjects.push_back(&plane);
    
	// Create main window
    _id_window = glutCreateWindow("cg1 ex5 ws11/12 - raytracing");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
    
    glutIdleFunc(idle);

	// Create world window
    _id_world = glutCreateSubWindow(_id_window, _win_gap, _win_gap, _win_w, _win_h);
    glutReshapeFunc(world_reshape);
    glutDisplayFunc(world_display);
    glutMouseFunc(world_mouse);
    glutMotionFunc(world_motion);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(world_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
	// Create view window
    _id_screen = glutCreateSubWindow(_id_window, _win_gap + _win_w + _win_gap, _win_gap, _win_w, _win_h);
    glutReshapeFunc(screen_reshape);
    glutDisplayFunc(screen_display);
    glutMouseFunc(screen_mouse);
    glutMotionFunc(screen_motion);
    glutPassiveMotionFunc(screen_passive_motion);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(screen_menu);
    redisplay_all();

    glutMainLoop();
    
    return 0;
}

