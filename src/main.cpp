/*
 * Program 3 base code - includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>
#include <chrono>

#include "Bezier.h"
#include "Camera.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Model.h"
#include "Program.h"
#include "Player.h"
#include "Shape.h"
#include "Spline.h"
#include "Texture.h"
#include "WindowManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>
#define PI 3.1415927

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime = glfwGetTime();
	double difference = actualtime - lasttime;
	lasttime = actualtime;
	return difference;
}

Camera mycam;
float deltaTime;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	//// Our shader program - use this one for Blinn-Phong
	//std::shared_ptr<Program> prog;

	////Our shader program for textures
	//std::shared_ptr<Program> texProg;

	// our shader programs
	std::shared_ptr<Program> prog, texProg, skyProg;

	//our geometry
	shared_ptr<Shape> sphere;
	//shared_ptr<Shape> dog;

	shared_ptr<Model> wolf;
	shared_ptr<Model> palm;
	shared_ptr<Model> chest;
	shared_ptr<Model> ship;
	//shared_ptr<Model> skeleton_pirate;
	shared_ptr<Player> skeleton_pirate;
	shared_ptr<Shape> mesh;

	///shared_ptr<Shape> theBunny;

	//global data for ground plane - direct load constant defined CPU data to GPU (not obj)
	GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;
	int g_GiboLen;
	//ground VAO
	GLuint GroundVertexArrayID;

	//the image to use as a texture (ground)
	shared_ptr<Texture> texture0, textureWolf, texture1, textureDaySky, textureNightSky;

	//global data (larger program should be encapsulated)
	vec3 gMin;
	float gRot = 0;
	float gCamH = 0;
	//animation data
	float lightTrans = 0;
	//float gTrans = -3;
	float gTrans = 40;
	float hierX = 60;
	float hierY = 1.5;
	float hierZ = 77.75;
	float sTheta = 0;
	float skyTheta = 0;
	float eTheta = 0;
	float hTheta = 0;
	float fTheta = 0;
	float materialType = 0;
	float numMaterials = 3;

	// Mouse Input
	double lastMouseX = 0.0;
	double lastMouseY = 0.0;
	double mouseX = 0.0;
	double mouseY = 0.0;
	double mouseOffsetX = 0.0;
	double mouseOffsetY = 0.0;
	bool firstMouse = true; // calcs first mouse movement

	Spline splinepath[2];
	bool goCamera = false;

	float cocoStartTime = glfwGetTime();
	float cocoStartWaitTime;
	bool cocoOnGround = false;
	float cocoY = 1.6;
	const float grav_accel = 9.8;
	const float cocoStartY = 1.6;
	const float cocoGroundTime = 2;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		////update global camera rotate
		//if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		//	gRot -= 0.2;
		//}
		//if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		//	gRot += 0.2;
		//}
		////update camera height
		//if (key == GLFW_KEY_S && action == GLFW_PRESS){
		//	gCamH  += 0.25;
		//}
		//if (key == GLFW_KEY_F && action == GLFW_PRESS){
		//	gCamH  -= 0.25;
		//}

		// TODO call player move here
		//skeleton_pirate->move(key, action);
		
		// moves camera w/ wasd
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			skeleton_pirate->w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
			skeleton_pirate->w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			skeleton_pirate->s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
			skeleton_pirate->s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			skeleton_pirate->a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
			skeleton_pirate->a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			skeleton_pirate->d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
			skeleton_pirate->d = 0;
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
			skeleton_pirate->shift = 1;
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
			skeleton_pirate->shift = 0;
		}

		// moving camera w/ arrow keys
		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			mycam.w = 1;
		}
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
			mycam.w = 0;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			mycam.s = 1;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
			mycam.s = 0;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			mycam.a = 1;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
			mycam.a = 0;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			mycam.d = 1;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
			mycam.d = 0;
		}


		if (key == GLFW_KEY_Q && action == GLFW_PRESS){
			lightTrans += 0.25;
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS){
			lightTrans -= 0.25;
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}

		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			cout << "Cam pos: X:" << mycam.pos.x << " Y: " << mycam.pos.y << " Z: " << mycam.pos.z << endl;
		}

		// bezier curve time
		if (key == GLFW_KEY_G && action == GLFW_RELEASE) {
			goCamera = !goCamera;
			cout << "goCamera = " << goCamera << endl;
		}

		// update material
		if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			materialType = fmod(materialType+1, numMaterials);
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		//double posX, posY;

		//if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			//glfwGetCursorPos(window, &posX, &posY);
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// cout << "Pos X " << mouseX << " Pos Y " << mouseY << endl;

		//if (firstMouse) {
		//	lastMouseX = mouseX;
		//	lastMouseY = mouseY;
		//	firstMouse = false;
		//}

		//// calc offset
		//mouseOffsetX = mouseX - lastMouseX;
		//mouseOffsetY = lastMouseY - mouseY; // oppo b/c inverted

		//lastMouseX = mouseX;
		//lastMouseY = mouseY;

		/*cout << "DT: " << deltaTime << "\n"
			<< "Mouse offsetX: " << mouseOffsetX << " Mouse offsetY: " << mouseOffsetY << endl; */
		//}

		//if (action == GLFW_PRESS)
		//{
		//	 glfwGetCursorPos(window, &posX, &posY);
		//	 cout << "Pos X " << posX <<  " Pos Y " << posY << endl;


		//}
	}

	void cursorCallBack(GLFWwindow* window)
	{
		//double posX, posY;

		int mouse1State = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		int mouse2State = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

		//glfwGetCursorPos(window, &posX, &posY);
		glfwGetCursorPos(window, &mouseX, &mouseY);
		//cout << "Pos X " << mouseX << " Pos Y " << mouseY << endl;

		if (firstMouse) {
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			firstMouse = false;
		}

		// calc offset
		mouseOffsetX = mouseX - lastMouseX;
		mouseOffsetY = lastMouseY - mouseY; // oppo b/c inverted

		lastMouseX = mouseX;
		lastMouseY = mouseY;

		//cout << "DT: " << deltaTime << "\n"
		//	<< "Mouse offsetX: " << mouseOffsetX << " Mouse offsetY: " << mouseOffsetY << endl;
		
		// calc angle around player 
		if (mouse1State == GLFW_PRESS) {
			/*float angleChange = mouseOffsetX * 0.3f;
			mycam.setAngleAroundPlayer(mycam.getAngleAroundPlayer() - angleChange);
			cout << "Angle Around Player: " << mycam.getAngleAroundPlayer() << endl;*/
			mycam.angle = mycam.angle + mouseOffsetX * 0.3f;
		}

		// calc camera pitch
		if (mouse2State == GLFW_PRESS) {
			float pitchChange = mouseOffsetY * 0.1f;
			if ((mycam.pitch - pitchChange) < 80 && (mycam.pitch - pitchChange) > -80) {
				mycam.pitch -= pitchChange;
			}
			cout << "Camer Pitch: " << mycam.pitch << endl;
		}
		

		//if (action == GLFW_PRESS)
		//{
		//	 glfwGetCursorPos(window, &posX, &posY);
		//	 cout << "Pos X " << posX <<  " Pos Y " << posY << endl;


		//}
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY) {
		//cout << "xDel + yDel " << deltaX << " " << deltaY << endl;
		//fill in for game camera
		//float new_pitch = mycam.pitch + deltaY * 1.5;
		//if (new_pitch <= 80 && new_pitch >= -80) //only update if within bounds
		//	mycam.pitch = new_pitch;
		//mycam.yaw = mycam.yaw + deltaX * 1.5;
		float zoomLevel = deltaY * 0.5;
		mycam.setDistanceFromPlayer(mycam.getDistanceFromPlayer() - zoomLevel);
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.72f, .84f, 1.06f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);


		// Initialize the GLSL program that we will use for local shading
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("flip");
		prog->addUniform("MatAmb");
		prog->addUniform("MatDif");
		prog->addUniform("MatSpec");
		prog->addUniform("MatShine");
		prog->addUniform("lightPos");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");


		// Initialize the GLSL program that we will use for texture mapping
		texProg = make_shared<Program>();
		texProg->setVerbose(true);
		texProg->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag0.glsl");
		texProg->init();
		texProg->addUniform("P");
		texProg->addUniform("V");
		texProg->addUniform("M");
		texProg->addUniform("flip");
		texProg->addUniform("Texture0");
		//texProg->addUniform("MatShine");
		texProg->addUniform("lightPos");
		texProg->addAttribute("vertPos");
		texProg->addAttribute("vertNor");
		texProg->addAttribute("vertTex");

		skyProg = make_shared<Program>();
		skyProg->setVerbose(true);
		skyProg->setShaderNames(resourceDirectory + "/skyvertex.glsl", resourceDirectory + "/skyfrag.glsl");
		skyProg->init();
		skyProg->addUniform("P");
		skyProg->addUniform("V");
		skyProg->addUniform("M");
		skyProg->addUniform("tex");
		skyProg->addUniform("tex2");
		skyProg->addUniform("day_night_ratio");
		skyProg->addAttribute("vertPos");
		skyProg->addAttribute("vertNor");
		skyProg->addAttribute("vertTex");

		//read in a load the texture
		texture0 = make_shared<Texture>();
  		//texture0->setFilename(resourceDirectory + "/grass.jpg");
  		texture0->setFilename(resourceDirectory + "/sand.jpg");
  		texture0->init();
  		texture0->setUnit(0);
  		texture0->setWrapModes(GL_REPEAT, GL_REPEAT);

		textureWolf = make_shared<Texture>();
		//textureWolf->setFilename(resourceDirectory + "/wolf_objs/wolf_difuse_001.tga.png");
		textureWolf->setFilename(resourceDirectory + "/wolf_objs/fur_texture_13_by_fox_n_wolf_d4t05rq-fullview.jpg");
		textureWolf->init();
		textureWolf->setUnit(1);
		textureWolf->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture1 = make_shared<Texture>();
		texture1->setFilename(resourceDirectory + "/cartoonSky.png");
		texture1->init();
		texture1->setUnit(2);
		texture1->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		textureDaySky = make_shared<Texture>();
		//textureDaySky->setFilename(resourceDirectory + "/sphere-day.jpg");
		textureDaySky->setFilename(resourceDirectory + "/ocean_sky2.jpg");
		textureDaySky->init();
		textureDaySky->setUnit(3);
		textureDaySky->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		textureNightSky = make_shared<Texture>();
		//textureNightSky->setFilename(resourceDirectory + "/nightsphere.jpg");
		textureNightSky->setFilename(resourceDirectory + "/ocean_sky2.jpg");
		textureNightSky->init();
		textureNightSky->setUnit(4);
		textureNightSky->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		// init splines up and down
		//splinepath[0] = Spline(glm::vec3(-6, 0, 5), glm::vec3(-1, -5, 5), glm::vec3(1, 5, 5), glm::vec3(2, 0, 5), 5);
		//splinepath[1] = Spline(glm::vec3(2, 0, 5), glm::vec3(3, -2, 5), glm::vec3(-0.25, 0.25, 5), glm::vec3(0, 0, 5), 5);
		//splinepath[0] = Spline(glm::vec3(-10.8, 5.1, 5.9), glm::vec3(-11, 7.55, 0.48), glm::vec3(-8.8, 4.4, -5), 5);
		splinepath[0] = Spline(glm::vec3(-5.76, 2.34, 4.85), glm::vec3(-8.78, 4.33, -2.9), glm::vec3(-3.02, 2.08, -5.54), 3);
		splinepath[1] = Spline(glm::vec3(-3.02, 2.08, -5.54), glm::vec3(-2.4, 1.09, 8), glm::vec3(1.34, 1.6, -3.3), glm::vec3(8.3, 7.7, -7.85), 5);

	}

	float randomFloat(int min, int max) {
		if (min > max)
			return randomFloat(max, min);
		if (min == max)
			return min;

		return (float)min + (rand() % max) + (float)(rand()) / (float)(RAND_MAX);
	}

	void initGeom(const std::string& resourceDirectory)
	{
		//EXAMPLE set up to read one shape from one obj file - convert to read several
		// Initialize mesh
		// Load geometry
 		// Some obj files contain material information.We'll ignore them for this assignment.
 		vector<tinyobj::shape_t> TOshapes;
 		vector<tinyobj::material_t> objMaterials;
 		string errStr;
		//load in the mesh and make the shape(s)
 		bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/sphereWTex.obj").c_str());
		if (!rc) {
			cerr << errStr << endl;
		} else {
			sphere = make_shared<Shape>();
			sphere->createShape(TOshapes[0]);
			sphere->measure();
			sphere->init();
		}
		//read out information stored in the shape about its size - something like this...
		//then do something with that information.....
		gMin.x = sphere->min.x;
		gMin.y = sphere->min.y;


		// load in wolf and make the shape(s)
		wolf = make_shared<Model>(resourceDirectory + "/wolf_objs/wolf_animations0001.obj"); // creating Model

		// Palm Tree
		palm = make_shared<Model>(resourceDirectory + "/palmtreeOBJ/palmtree.obj"); // creating Model

		// Treasure Chest
		chest = make_shared<Model>(resourceDirectory + "/chestlx_closed.obj"); // creating Model

		// Pirate Ship
		ship = make_shared<Model>(resourceDirectory + "/pirate_baot.obj"); // creating Model

		// Hier Model
		mesh = make_shared<Shape>();
		mesh->createShape(TOshapes[0]);
		mesh->measure();
		mesh->init();

		// Set random chest coords
		srand((unsigned) time(NULL));
		// X and Z randomly btwn 10 and 50
		float chest_x = randomFloat(-50, 50);
		float chest_y = -0.6;
		float chest_z = randomFloat(-50, 50);
		chest->setLocation(vec3(chest_x, chest_y, chest_z));
		// y rotation btwn 0 and 360
		chest->setRotationAngle(randomFloat(0, 360));


		// Pirate
		//skeleton_pirate = make_shared<Model>(resourceDirectory + "/skeleton-pirate/source/skeleton-pirate.obj"); // skele model
		skeleton_pirate = make_shared<Player>(resourceDirectory + "/skeleton-pirate/source/skeleton-pirate.obj"); // skele model
		//skeleton_pirate = make_shared<Player>(resourceDirectory + "/dog.obj"); // skele model
		skeleton_pirate->setLocation(vec3(0.3, -0.7, -4));
		skeleton_pirate->setScale(vec3(0.1, 0.1, 0.1));
		mycam.setPlayer(skeleton_pirate);
		mycam.pos = vec3(0.3, 2, -10);
		mycam.front = skeleton_pirate->getLocation();



		// Initialize dog mesh
		//vector<tinyobj::shape_t> TOshapesDog;
		//vector<tinyobj::material_t> objMaterialsDog;
		//load in the mesh and make the shape(s)
		//rc = tinyobj::LoadObj(TOshapesPalm, objMaterialsPalm, errStr, (resourceDirectory + "/palmtreeOBJ/palmtree.obj").c_str());
		//rc = tinyobj::LoadObj(TOshapesDog, objMaterialsDog, errStr, (resourceDirectory + "/dog.obj").c_str());
		//if (!rc) {
		//	cerr << errStr << endl;
		//}
		//else {
		//	dog = make_shared<Shape>();
		//	dog->createShape(TOshapesDog[0]);
		//	dog->measure();
		//	dog->init();
		//}
		////read out information stored in the shape about its size - something like this...
		////then do something with that information.....
		//gMin.x = dog->min.x;
		//gMin.y = dog->min.y;

		// Initialize bunny mesh.
		//vector<tinyobj::shape_t> TOshapesB;
 	//	vector<tinyobj::material_t> objMaterialsB;
		////load in the mesh and make the shape(s)
 	//	rc = tinyobj::LoadObj(TOshapesB, objMaterialsB, errStr, (resourceDirectory + "/bunnyNoNorm.obj").c_str());
 	//	//rc = tinyobj::LoadObj(TOshapesB, objMaterialsB, errStr, (resourceDirectory + "/bunny.obj").c_str());
		//if (!rc) {
		//	cerr << errStr << endl;
		//} else {
		//	
		//	theBunny = make_shared<Shape>();
		//	theBunny->createShape(TOshapesB[0]);
		//	theBunny->measure();
		//	theBunny->init();
		//}

		cocoStartTime = glfwGetTime();

		//code to load in the ground plane (CPU defined data passed to GPU)
		initGround();
	}

	//directly pass quad for the ground to the GPU
	void initGround() {

		float g_groundSize = 70;
		//float g_groundY = -0.25;
		float g_groundY = 0;

  		// A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
		float GrndPos[] = {
			-g_groundSize, g_groundY, -g_groundSize,
			-g_groundSize, g_groundY,  g_groundSize,
			g_groundSize, g_groundY,  g_groundSize,
			g_groundSize, g_groundY, -g_groundSize
		};

		float GrndNorm[] = {
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0
		};

		static GLfloat GrndTex[] = {
      		0, 0, // back
      		0, 30,
      		30, 30,
      		30, 0 };

      	unsigned short idx[] = {0, 1, 2, 0, 2, 3};

		//generate the ground VAO
      	glGenVertexArrays(1, &GroundVertexArrayID);
      	glBindVertexArray(GroundVertexArrayID);

      	g_GiboLen = 6;
      	glGenBuffers(1, &GrndBuffObj);
      	glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
      	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

      	glGenBuffers(1, &GrndNorBuffObj);
      	glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
      	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

      	glGenBuffers(1, &GrndTexBuffObj);
      	glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
      	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

      	glGenBuffers(1, &GIndxBuffObj);
     	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
      	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
      }

      //code to draw the ground plane
     void drawGround(shared_ptr<Program> curS) {
     	curS->bind();
     	glBindVertexArray(GroundVertexArrayID);
     	texture0->bind(curS->getUniform("Texture0"));
		//draw the ground plane 
  		SetModel(vec3(0, -1, 0), 0, 0, 1, curS);
  		glEnableVertexAttribArray(0);
  		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
  		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  		glEnableVertexAttribArray(1);
  		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
  		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  		glEnableVertexAttribArray(2);
  		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
  		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

   		// draw!
  		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
  		glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);

  		glDisableVertexAttribArray(0);
  		glDisableVertexAttribArray(1);
  		glDisableVertexAttribArray(2);
  		curS->unbind();
     }

     //helper function to pass material data to the GPU
	void SetMaterial(shared_ptr<Program> curS, int i) {

    	switch (i) {
    		case 0: // pink
    			/*glUniform3f(curS->getUniform("MatAmb"), 0.096, 0.046, 0.095);
    			glUniform3f(curS->getUniform("MatDif"), 0.96, 0.46, 0.95);
    			glUniform3f(curS->getUniform("MatSpec"), 0.45, 0.23, 0.45);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);*/
				glUniform3f(curS->getUniform("MatAmb"), 0.042, 0.074, 0.021);
    			glUniform3f(curS->getUniform("MatDif"), 0.42, 0.74, 0.21);
    			glUniform3f(curS->getUniform("MatSpec"), 0.21, 0.37, 0.10);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);
    		break;
    		case 1: // purple
    			/*glUniform3f(curS->getUniform("MatAmb"), 0.063, 0.038, 0.1);
    			glUniform3f(curS->getUniform("MatDif"), 0.63, 0.38, 1.0);
    			glUniform3f(curS->getUniform("MatSpec"), 0.3, 0.2, 0.5);
    			glUniform1f(curS->getUniform("MatShine"), 4.0);*/
				glUniform3f(curS->getUniform("MatAmb"), 0.043, 0.036, 0.036);
				glUniform3f(curS->getUniform("MatDif"), 0.43, 0.36, 0.36);
				glUniform3f(curS->getUniform("MatSpec"), 0.21, 0.18, 0.18);
				glUniform1f(curS->getUniform("MatShine"), 4.0);
    		break;
    		case 2: // blue
    			/*glUniform3f(curS->getUniform("MatAmb"), 0.004, 0.05, 0.09);
    			glUniform3f(curS->getUniform("MatDif"), 0.04, 0.5, 0.9);
    			glUniform3f(curS->getUniform("MatSpec"), 0.02, 0.25, 0.45);
    			glUniform1f(curS->getUniform("MatShine"), 27.9);*/
				glUniform3f(curS->getUniform("MatAmb"), 0.030, 0.046, 0.036);
				glUniform3f(curS->getUniform("MatDif"), 0.30, 0.46, 0.36);
				glUniform3f(curS->getUniform("MatSpec"), 0.15, 0.23, 0.18);
				glUniform1f(curS->getUniform("MatShine"), 27.9);
    		break;
  		}
	}

	/* helper function to set model trasnforms */
  	void SetModel(vec3 trans, float rotY, float rotX, float sc, shared_ptr<Program> curS) {
  		mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
  		mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, vec3(1, 0, 0));
  		mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, vec3(0, 1, 0));
  		mat4 ScaleS = glm::scale(glm::mat4(1.0f), vec3(sc));
  		mat4 ctm = Trans*RotX*RotY*ScaleS;
  		glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
  	}

	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   	}

   	/* code to draw waving hierarchical model */
   	void drawHierModel(shared_ptr<MatrixStack> Model) {
   		// draw hierarchical mesh - replace with your code if desired
		//Model->pushMatrix();
		//	Model->loadIdentity();
		//	Model->translate(vec3(gTrans, 0, 6));
		//	
		//	//draw the torso with these transforms
		//	Model->pushMatrix();
		//	  Model->scale(vec3(1.15, 1.35, 1.0));
		//	  setModel(prog, Model);
		//	  sphere->draw(prog);
		//	Model->popMatrix();
		//	
		//Model->popMatrix();

		// draw mesh 
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(vec3(hierX, hierY, hierZ));
			/* draw top cube - aka head */
			Model->pushMatrix();
				Model->translate(vec3(0, 1.4, 0));
				Model->scale(vec3(0.5, 0.5, 0.5));
				setModel(prog, Model);
				mesh->draw(prog);
			Model->popMatrix();
			//draw the torso with these transforms
			Model->pushMatrix();
			  Model->scale(vec3(1.25, 1.35, 1.25));
			  setModel(prog, Model);
			  mesh->draw(prog);
			Model->popMatrix();
			// draw the upper 'arm' - relative 
			//note you must change this to include 3 components!
			Model->pushMatrix();
			  //place at shoulder
			  Model->translate(vec3(0.8, 0.8, 0));
			  //rotate shoulder joint
			  Model->rotate(sTheta, vec3(0, 0, 1));
			  //move to shoulder joint
			  Model->translate(vec3(0.8, 0, 0));
	
			    //now draw lower arm - this is INCOMPLETE and you will add a 3rd component
			  	//right now this is in the SAME place as the upper arm
			  	Model->pushMatrix();
				  Model->translate(vec3(0.7, 0, 0));
				  Model->rotate(fTheta, vec3(0, 0, 1));
				  Model->translate(vec3(.7, 0, 0));

				  //hand
				  Model->pushMatrix();
				    Model->translate(vec3(0.4, 0, 0));
				    Model->rotate(hTheta, vec3(0, 0, 1));
				    Model->translate(vec3(0.4, 0, 0));
					Model->scale(vec3(.4, .3, .3));
					setModel(prog, Model);
					mesh->draw(prog);
				  Model->popMatrix();

			      Model->scale(vec3(0.7, 0.25, 0.25));
			  	  setModel(prog, Model);
			  	  mesh->draw(prog);
			  	Model->popMatrix();

			  //Do final scale ONLY to upper arm then draw
			  //non-uniform scale
			  Model->scale(vec3(0.8, 0.25, 0.25));
			  setModel(prog, Model);
			  mesh->draw(prog);
			Model->popMatrix();
		
		Model->popMatrix();


		// draw left arm 
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(vec3(hierX, hierY, hierZ));
			/* draw top cube - aka head */
			Model->pushMatrix();
				Model->translate(vec3(0, 1.4, 0));
				Model->scale(vec3(0.5, 0.5, 0.5));
				setModel(prog, Model);
				mesh->draw(prog);
			Model->popMatrix();
			//draw the torso with these transforms
			Model->pushMatrix();
				Model->scale(vec3(1.25, 1.35, 1.25));
				setModel(prog, Model);
				mesh->draw(prog);
			Model->popMatrix();
			// draw the upper 'arm' - relative 
			//note you must change this to include 3 components!
			Model->pushMatrix();
			//place at shoulder
				Model->translate(vec3(-0.8, 0.8, 0));
				//rotate shoulder joint
				Model->rotate(sTheta, vec3(0, 0, -1));
				//move to shoulder joint
				Model->translate(vec3(-0.8, 0, 0));

				//now draw lower arm - this is INCOMPLETE and you will add a 3rd component
				//right now this is in the SAME place as the upper arm
				Model->pushMatrix();
					Model->translate(vec3(-0.7, 0, 0));
					Model->rotate(fTheta, vec3(0, 0, -1));
					Model->translate(vec3(-.7, 0, 0));

					//hand
					Model->pushMatrix();
						Model->translate(vec3(-0.4, 0, 0));
						Model->rotate(hTheta, vec3(0, 0, -1));
						Model->translate(vec3(-0.4, 0, 0));
						Model->scale(vec3(.4, .3, .3));
						setModel(prog, Model);
						mesh->draw(prog);
					Model->popMatrix();

					Model->scale(vec3(0.7, 0.25, 0.25));
					setModel(prog, Model);
					mesh->draw(prog);
				Model->popMatrix(); 

			//Do final scale ONLY to upper arm then draw
			//non-uniform scale
				Model->scale(vec3(0.8, 0.25, 0.25));
				setModel(prog, Model);
				mesh->draw(prog);
			Model->popMatrix();

		Model->popMatrix();

   	}

	void updateUsingCameraPath(float frametime) {

		if (goCamera) {
			if (!splinepath[0].isDone()) {
				splinepath[0].update(frametime);
				mycam.pos = splinepath[0].getPosition();
			}	
			else {
				splinepath[1].update(frametime);
				mycam.pos = splinepath[1].getPosition();
			}
		}
	}

	void render(float frametime) {
		//double frametime = get_last_elapsed_time();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the matrix stack for Lab 6
		float aspect = width/(float)height;

		// check mouse position for camera
		cursorCallBack(windowManager->getHandle());

		// Create the matrix stacks - please leave these alone for now
		auto Projection = make_shared<MatrixStack>();
		//auto View = make_shared<MatrixStack>();
		mat4 View = mycam.process(frametime);
		auto Model = make_shared<MatrixStack>();

		//update the camera position
		updateUsingCameraPath(frametime);

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

		// sky shaders
		skyProg->bind();

		glm::mat4 ViewBox = glm::mat4(glm::mat3(View));

		glUniformMatrix4fv(skyProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(skyProg->getUniform("V"), 1, GL_FALSE, value_ptr(ViewBox));
		glUniformMatrix4fv(skyProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		//glUniform3f(texProg->getUniform("lightPos"), 2.0 + lightTrans, 2.0, 2.9);
		//glUniform1f(texProg->getUniform("MatShine"), 27.9);

		textureDaySky->bind(skyProg->getUniform("tex"));
		textureNightSky->bind(skyProg->getUniform("tex2"));
		//glUniform1f(skyProg->getUniform("day_night_ratio"), 0); 


		//draw big background sphere
		Model->pushMatrix();
		// inside sphere, so want normals pointing in
			//glUniform1i(skyProg->getUniform("flip"), 1);
			glUniform1f(skyProg->getUniform("day_night_ratio"), skyTheta);
			Model->loadIdentity();
			Model->scale(vec3(.5));
			setModel(skyProg, Model);

			glDisable(GL_DEPTH_TEST);
			sphere->draw(skyProg);
			glEnable(GL_DEPTH_TEST);

		Model->popMatrix();

		skyProg->unbind();

		// View is global translation along negative z for now
		//View->pushMatrix();
		//View->loadIdentity();
		////camera up and down
		//View->translate(vec3(0, gCamH, 0));
		////global rotate (the whole scene )
		//View->rotate(gRot, vec3(0, 1, 0));

		// Draw the scene
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		//glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View));
		//glUniform3f(prog->getUniform("lightPos"), 2.0+lightTrans, 2.0, 2.9);
		glUniform3f(prog->getUniform("lightPos"), 200.0+lightTrans, 200.0, 200.0);
		glUniform1i(prog->getUniform("flip"), 0); // flip normals if needed

		// draw the array of bunnies
		Model->pushMatrix();

		/*float sp = 3.0;
		float off = -3.5;
		  for (int i =0; i < 3; i++) {
		  	for (int j=0; j < 3; j++) {
			  Model->pushMatrix();
				Model->translate(vec3(off+sp*i, -1, off+sp*j));
				// Model->scale(vec3(0.85, 0.85, 0.85)); //bunny w/ normals
				Model->scale(vec3(8, 8, 8)); //bunny w/o normals
				SetMaterial(prog, (i+j)%3);
				glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
				theBunny->draw(prog);
			  Model->popMatrix();
			}
		  }
		Model->popMatrix(); */

		//Model->pushMatrix();
		//	Model->translate(vec3(-5, -1, 10));
		//	// Model->scale(vec3(0.85, 0.85, 0.85)); //bunny w/ normals
		//	Model->scale(vec3(8, 8, 8)); //bunny w/o normals
		//	SetMaterial(prog, materialType);
		//	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		//	theBunny->draw(prog);
		//Model->popMatrix();


		float sp = 3.0;
		float off = -3.5;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Model->pushMatrix();
					Model->translate(vec3(off + sp * i, -1, off + sp * j));
					// Model->scale(vec3(0.85, 0.85, 0.85)); //bunny w/ normals
					Model->scale(vec3(0.06, 0.06, 0.06)); //bunny w/o normals
					SetMaterial(prog, (i + j) % 3);
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
					palm->draw(prog);
				Model->popMatrix();
			}
		}
		Model->popMatrix();

		// Treasure Chest
		Model->pushMatrix(); // T R S
			Model->translate(chest->getLocation());
			Model->scale(vec3(0.4, 0.4, 0.4));
			Model->rotate(chest->getRotationAngle(), vec3(0, 1, 0));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			chest->draw(prog);
		Model->popMatrix();


		Model->pushMatrix(); // T R S
			Model->translate(vec3(60, -3, 80));
			Model->scale(vec3(1, 1, 1));
			Model->rotate(180, vec3(0, 1, 0));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			ship->draw(prog);
		Model->popMatrix();



		//use helper function that uses glm to create some transform matrices
		/* Model->pushMatrix(); // T R S
			Model->translate(vec3(-1, -1, -3));
			Model->rotate(0.2, vec3(0, 1, 0));
			Model->scale(vec3(1.2, 1.2, 1.2));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			wolf->draw(prog);
		Model->popMatrix(); */
			//setModel(prog, vec3(1.0, -1.0, .5), .2, 0, 1.2);

		// draw palm tree
		/*Model->pushMatrix(); // T R S
			Model->translate(vec3(-1, -1, -4));
			Model->scale(vec3(0.06, 0.06, 0.06));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			palm ->draw(prog);
		Model->popMatrix();

		//second palm tree (right)
		Model->pushMatrix(); // T R S
			Model->translate(vec3(2, -1, 0));
			Model->scale(vec3(0.06, 0.06, 0.06));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			palm ->draw(prog);
		Model->popMatrix();
		//setModel(prog, vec3(2, -1, 0), 0, 0, 0.06);

		//third palm tree (behind)
		Model->pushMatrix(); // T R S
			Model->translate(vec3(0, -1, -1.5));
			Model->scale(vec3(0.06, 0.06, 0.06));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			palm ->draw(prog);
		Model->popMatrix(); */

		// Call move function on pirate
		skeleton_pirate->move(frametime);

		//draw skele pirate
		Model->pushMatrix(); // T R S
			Model->translate(skeleton_pirate->getLocation());
			Model->scale(skeleton_pirate->getScale());
			Model->rotate(skeleton_pirate->getRotationAngle(), skeleton_pirate->getRotationAxis());
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(prog, Model);
			skeleton_pirate->draw(prog);
		Model->popMatrix();


		//draw the waving HM
		SetMaterial(prog, 1);
		SetMaterial(prog, materialType);
		drawHierModel(Model);

		prog->unbind();

		//switch shaders to the texture mapping shader and draw the ground
		texProg->bind();
		glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		//glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		//glUniform3f(texProg->getUniform("lightPos"), 2.0 + lightTrans, 2.0, 2.9);
		glUniform3f(texProg->getUniform("lightPos"), 200.0 + lightTrans, 200.0, 200.0);
		//glUniform1f(texProg->getUniform("MatShine"), 27.9);
		glUniform1i(texProg->getUniform("flip"), 1);
				
		textureWolf->bind(texProg->getUniform("Texture0"));
		Model->pushMatrix(); // T R S
			Model->translate(vec3(-1, -1, -3));
			Model->rotate(0.2, vec3(0, 1, 0));
			Model->scale(vec3(1.2, 1.2, 1.2));
			//glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			setModel(texProg, Model);
			wolf->draw(texProg);
		Model->popMatrix();

		glUniform1i(texProg->getUniform("flip"), 1);

		drawGround(texProg);


		// draw the dog
		/*texture0->bind(texProg->getUniform("Texture0"));
		Model->pushMatrix();
			Model->translate(vec3(2, 0.2, 1));
			// Model->scale(vec3(0.85, 0.85, 0.85)); //bunny w/ normals
			Model->scale(vec3(0.5, 0.5, 0.5)); //bunny w/o normals
			SetMaterial(texProg, 1);
			glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			dog->draw(texProg);
		Model->popMatrix(); */

		//draw the ground plane 

		texProg->unbind();
		
		//animation update example
		skyTheta = sin(0.1 * glfwGetTime());
		//skyTheta = sin(glfwGetTime());
		//sTheta = sin(glfwGetTime());
		//eTheta = std::max(0.0f, (float)sin(glfwGetTime()));
		//hTheta = std::max(0.0f, (float)cos(glfwGetTime()));

		// update animation vals
		sTheta = 0.2 * abs(sin(glfwGetTime()));
		fTheta = 0.7 * abs(sin(4 * glfwGetTime())) + 1;
		hTheta = 0.75 * abs(sin(4 * glfwGetTime())) - 0.25;

		// Pop matrix stacks.
		Projection->popMatrix();
		//View->popMatrix();

	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	auto lastTime = chrono::high_resolution_clock::now();

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{

		// save current time for next frame
		auto nextLastTime = chrono::high_resolution_clock::now();

		// get time since last frame
		deltaTime =
			chrono::duration_cast<std::chrono::microseconds>(
				chrono::high_resolution_clock::now() - lastTime)
			.count();
		// convert microseconds (weird) to seconds (less weird)
		deltaTime *= 0.000001;

		// reset lastTime so that we can calculate the deltaTime
		// on the next frame
		lastTime = nextLastTime;

		// Render scene.
		application->render(deltaTime);

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
