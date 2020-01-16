#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cassert>	//assert

// glad beforw glfw
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"

#include "common_matrices.hpp"
#include "obj_mesh_file_io.hpp"
#include "obj_mesh.hpp"
#include "mat4f.hpp"
#include "mat3f.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "shader_file_io.hpp"
#include "buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vbo_tools.hpp"
#include "texture.hpp"
#include "image.hpp"

using namespace math;
using namespace geometry;
using namespace opengl;

// GLOBAL Variables
Mat4f g_M = Mat4f::identity();
Mat4f g_V = Mat4f::identity();
Mat4f g_P = Mat4f::identity();

bool texturesEnabled = true;
int renderingMode = 1;  //which rendermode: wireframe, other shaders
int model = 1;  //which model to use

//mouse button callback vars
int pressed = 0;    //0= nothing clicked, 1=left click, 2=right click
float zoom = 1;

double xPosition;
double yPosition;

Vec3f lightPosition(1, 0, 0);
float lightRotationAmount = 10;

geometry::OBJMesh meshData;
std::string filePath = "../models/Nefertiti_Low/Nefertiti_Low.obj";

int projectionMode = 0; //0 means ortho, 1 means perspective

GLuint g_width = 1000, g_height = 1000;

// function declaration
using namespace std;

void setFrameBufferSize(GLFWwindow *window, int width, int height) {
  g_width = width;
  g_height = height;
  glViewport(0, 0, g_width, g_height);
  g_P = perspectiveProjection(30, float(g_width) / g_height, 0.01, 100.f);
}

static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {


    if(pressed == 1) {
        if(xPosition < xpos)
            g_M = rotateAboutYMatrix(xPosition - xpos) * g_M;
        if(xPosition > xpos)
            g_M = rotateAboutYMatrix(xPosition - xpos) * g_M;
        if(yPosition < ypos)
            g_M = rotateAboutXMatrix(yPosition - ypos) * g_M;
        if(yPosition > ypos)
            g_M = rotateAboutXMatrix(yPosition - ypos) * g_M;
    }
    if(pressed == 2) {
        if(xPosition < xpos) {
            lightPosition = rotateAroundAxis(lightPosition, Vec3f(0, 1, 0), (lightRotationAmount - xPosition)/100);

        }
        if(xPosition > xpos) {
            lightPosition = rotateAroundAxis(lightPosition, Vec3f(0, 1, 0), (lightRotationAmount + xPosition) /100);

        }
        if(yPosition < ypos) {
            lightPosition = rotateAroundAxis(lightPosition, Vec3f(1, 0, 0), (lightRotationAmount - yPosition) /100);

        }
        if(yPosition > ypos) {
            lightPosition = rotateAroundAxis(lightPosition, Vec3f(1, 0, 0), (lightRotationAmount + yPosition) /100);

        }
    }

    xPosition = xpos;
    yPosition = ypos;
}


void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        pressed = 1;
    }
    if(button ==  GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        pressed = 2;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        pressed = 0;
    }
    if(button ==  GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        pressed = 0;
    }
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    if(yOffset > 0) {
        g_M = uniformScaleMatrix(1.1) * g_M;
    } else {
        g_M = uniformScaleMatrix(1. / 1.1) * g_M;
    }

}

void setKeyboard(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {



  if (GLFW_KEY_LEFT == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutYMatrix(5) * g_M;
    }
  } else if (GLFW_KEY_RIGHT == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutYMatrix(-5) * g_M;
    }
  } else if (GLFW_KEY_UP == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = uniformScaleMatrix(1.1) * g_M;
    }
  } else if (GLFW_KEY_DOWN == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = uniformScaleMatrix(1. / 1.1) * g_M;
    }
  } else if (GLFW_KEY_W == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0, 0.1, 0) * g_M;
    }
  } else if (GLFW_KEY_S == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0, -0.1, 0) * g_M;
    }
  } else if (GLFW_KEY_D == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0.1, 0, 0) * g_M;
    }
  } else if (GLFW_KEY_A == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(-0.1, 0, 0) * g_M;
    }
  } else if (GLFW_KEY_ESCAPE == key) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  } else if (GLFW_KEY_1 == key) {
      if (GLFW_PRESS == action) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        cout<<"Render Mode: Wireframe\n";
        renderingMode = 1;
      }
  }

  else if (GLFW_KEY_2 == key) {
        if (GLFW_PRESS == action) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          cout<<"Render Mode: Flat Shading\n";
          renderingMode = 2;
        }
    }

  else if (GLFW_KEY_3 == key) {
        if (GLFW_PRESS == action) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          cout<<"Render Mode: Gouraud (Diffuse) Shading\n";
          renderingMode = 3;
        }
    }

  else if (GLFW_KEY_4 == key) {
        if (GLFW_PRESS == action) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          cout<<"Render Mode: Phong Shading\n";
          renderingMode = 4;
        }
    }
  else if(GLFW_KEY_O == key) {
       if(GLFW_PRESS == action) {
           projectionMode = 0;
           cout<<"Orthographic"<<endl;
       }
   } else if(GLFW_KEY_P == key) {
       if(GLFW_PRESS == action) {
           projectionMode = 1;
           cout<<"Perspective"<<endl;
       }
   }
  else if(GLFW_KEY_7 == key) {
         if(GLFW_PRESS == action) {
             model = 1;
             cout<<"Teapot Model"<<endl;
         }
     }
  else if(GLFW_KEY_8 == key) {
         if(GLFW_PRESS == action) {
             model = 2;
             cout<<"Spot Model"<<endl;
         }
     }
  else if(GLFW_KEY_9 == key) {
         if(GLFW_PRESS == action) {
             model = 3;
             cout<<"Nefertiti Low Poly Model"<<endl;
         }
     }
  else if(GLFW_KEY_0 == key) {
         if(GLFW_PRESS == action) {
             model = 4;
             cout<<"Nefertiti High Poly Model"<<endl;
         }
     }
}

// user defined alias
opengl::Program createShaderProgram(std::string const &vertexShaderFile,
                                    std::string const &fragmentShaderFile) {
  using namespace opengl;
  auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
  auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

  std::cout << "[Log] compiling program " << vertexShaderFile << ' '
            << fragmentShaderFile << '\n';
  return opengl::makeProgram(vertexShaderSource, fragmentShaderSource);
}

opengl::Program createShaderProgram(std::string const &vertexShaderFile,
                                    std::string const &geometryShaderFile,
                                    std::string const &fragmentShaderFile) {
  using namespace opengl;
  auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
  auto geometryShaderSource = loadShaderStringFromFile(geometryShaderFile);
  auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

  std::cout << "[Log] compiling program " << vertexShaderFile << ' '
            << geometryShaderFile << ' ' << fragmentShaderFile << '\n';
  return opengl::makeProgram(vertexShaderSource, geometryShaderSource,
                             fragmentShaderSource);
}

std::string glfwVersion() {
  std::ostringstream s("GLFW version: ", std::ios::in | std::ios::ate);
  // print version
  int glfwMajor, glfwMinor, glfwRevision;
  glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
  s << glfwMajor << '.' << glfwMinor << '.' << glfwRevision;
  return s.str();
}

GLFWwindow *initWindow() {
  GLFWwindow *window = nullptr;

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  std::cout << glfwVersion() << '\n';

  // set opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(1000,           // width
                            1000,           // height
                            "Assignment 2", // title
                            NULL,           // unused
                            NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSwapInterval(1); // vsync
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);

  //Polygon fill mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //WIREFRAME**********************************************************************

  // setup callbacks
  glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
  glfwSetKeyCallback(window, setKeyboard);
  glfwSetCursorPosCallback(window, cursorPositionCallback); //mouse pointer position
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //keep mouse pointer visible
  glfwSetMouseButtonCallback(window, mouseButtonCallback);  //mouse button control
  glfwSetScrollCallback(window, scrollCallback);

  return window;
}

void bindTexture(opengl::Texture const &texture, opengl::Program const &program, std::string s, GLuint activeTex) {
	assert(activeTex < 32);
	glActiveTexture(GL_TEXTURE0 + activeTex); // Activate texture in position "activeTex"
	texture.bind();		//Bind texture object to active texture
	
	//Set bound texture to sampler2D uniform in shader program in position "activeTex" 
	program.use();
	glUniform1i(program.uniformLocation(s), activeTex);	
}

int main() {
  GLFWwindow *window = initWindow();

  auto vao = makeVertexArrayObject();
  auto indexBuffer = makeBufferObject();
  auto vertexBuffer = makeBufferObject();
  GLuint totalIndices = 0;

  auto depthShader = createShaderProgram("../shaders/depth_vs.glsl",
                                        "../shaders/depth_fs.glsl");
  auto normalShader = createShaderProgram("../shaders/normal_vs.glsl",
                                        "../shaders/normal_fs.glsl");
  auto textureShader = createShaderProgram("../shaders/texture_vs.glsl",
                                        "../shaders/texture_gs.glsl",
                                        "../shaders/texture_fs.glsl");

  auto gouraudShader = createShaderProgram("../shaders/gouraud_vs.glsl",
                                           "../shaders/gouraud_fs.glsl");

  auto phongShader = createShaderProgram("../shaders/phong_vs.glsl",
                                         "../shaders/phong_fs.glsl");

  auto flatShader = createShaderProgram("../shaders/flat_vs.glsl",
                                        "../shaders/flat_gs.glsl",
                                        "../shaders/flat_fs.glsl");

  auto flatTextureShader = createShaderProgram("../shaders/flat_vstex.glsl",
                                               "../shaders/flat_gstex.glsl",
                                               "../shaders/flat_fstex.glsl");

  auto gouraudTextureShader = createShaderProgram("../shaders/gouraud_vstex.glsl",
                                           "../shaders/gouraud_fstex.glsl");

  auto phongTextureShader = createShaderProgram("../shaders/phong_vstex.glsl",
                                         "../shaders/phong_fstex.glsl");

  assert(depthShader && normalShader && gouraudShader && phongShader && flatShader && textureShader
         && flatTextureShader && gouraudTextureShader && phongTextureShader);	//Check console for program compilation error

  auto tex = generateTexture();

  {

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------


    //Uncomment your model of choice

    std::string filePath = "../models/Nefertiti_High/Nefertiti_High.obj";
    //std::string filePath = "../models/Nefertiti_Low/Nefertiti_Low.obj";
    //std::string filePath = "../models/spot/spot_triangulated.obj";
    //std::string filePath = "../models/teapot/teapot_triangulated.obj";
     


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

    if (!geometry::loadOBJMeshFromFile(filePath, meshData)) {
      std::cerr << "[Error] Cannot load .obj\n";
    }

	//Calculate vertex normals if they are not included in .obj file
	if (meshData.normals.size() == 0) {

        if(meshData.textureCoords.empty()) {
            auto normals =
                geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);

            auto vboData = opengl::makeConsistentVertexNormalIndices(meshData, normals);

            totalIndices =
                opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
        } else {
            auto normals =
                geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);

            auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData, normals);

            totalIndices =
                opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
        }

	}

	//Pass in vertex normals if they are already included in .obj file
	else {		
		if (meshData.textureCoords.empty()) {
			auto vboData = opengl::makeConsistentVertexNormalIndices(meshData);

			totalIndices =
				opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
		}
		else {
			auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData);

			totalIndices =
				opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
		}
	}

  } // ... mesh is deconstruced here (freeing memory)

  {

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

      //you only need to change these when you want to load a model with a texture.
      //If you don't want to use any textures, you don't even need to worry about these

      //auto image = raster::read_image_from_file("../models/spot/spot_texture.png");
      //auto image = raster::read_image_from_file("../models/Nefertiti_Low/COLOR_Low.jpg");
      auto image = raster::read_image_from_file("../models/Nefertiti_High/COLOR_High.jpg");

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	  glActiveTexture(GL_TEXTURE0);	
	  tex.bind();
      //4 below lines are configuration settings for how to actually sample the texture
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	  auto channels = GL_RGBA;
	  if (image.channels() == 3)
		  channels = GL_RGB;
	  
      //puts image on gpu with below settings
	  glTexImage2D(GL_TEXTURE_2D,  //target
        0,                 		//level (mip mapping)
		channels,         		//internalformat
		image.width(),    		//width
		image.height(),   		//height
		0,                		//border
		channels,           	//format
		GL_UNSIGNED_BYTE, 		//type
		image.data());			//data
	  
	  glGenerateMipmap(GL_TEXTURE_2D);
  }


  Vec3f viewPosition(0, 0, 3);

  g_V = lookAtMatrix(viewPosition,    // eye position
                     {0.f, 0.f, 0.f}, // look at
                     {0.f, 1.f, 0.f}  // up vector
                     );
  g_P = perspectiveProjection(30.,  // fov
                              1.f,  // ratio
                              0.1f, // distance to near plane
                              100.f // distance to far plane
                              );

      opengl::Program *program;
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
      //shading with textures
// /*
      texturesEnabled = true;
      flatTextureShader.use();
      setUniformVec3f(flatTextureShader.uniformLocation("lightPosition"), lightPosition);
      setUniformVec3f(flatTextureShader.uniformLocation("viewPosition"), viewPosition);
      program = &flatTextureShader;
// */


      //shading with no textures, model will be grey/white
 /*
      texturesEnabled = false;
      flatShader.use();
      setUniformVec3f(flatShader.uniformLocation("lightPosition"), lightPosition);
      setUniformVec3f(flatShader.uniformLocation("viewPosition"), viewPosition);
      program = &flatShader;
 */

//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************

  while (!glfwWindowShouldClose(window)) {

      //Load texture to OpenGL
      bindTexture(tex, *program, "tex", 0);



    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    program->use();

    //flat
    if(renderingMode == 2 && texturesEnabled) {
        flatTextureShader.use();
        setUniformVec3f(flatTextureShader.uniformLocation("lightPosition"), lightPosition);
        setUniformVec3f(flatTextureShader.uniformLocation("viewPosition"), viewPosition);
        opengl::Program *program = &flatTextureShader;
    }
    //gouraud
    else if(renderingMode == 3 && texturesEnabled) {
        gouraudTextureShader.use();
        setUniformVec3f(gouraudTextureShader.uniformLocation("lightPosition"), lightPosition);
        opengl::Program *program = &gouraudTextureShader;
    }
    //phong
    else if(renderingMode == 4 && texturesEnabled) {
        phongTextureShader.use();
        setUniformVec3f(phongTextureShader.uniformLocation("lightPosition"), lightPosition);
        setUniformVec3f(phongTextureShader.uniformLocation("viewPosition"), viewPosition);
        opengl::Program *program = &phongTextureShader;
    }
    //flat
    else if(renderingMode == 2 && !texturesEnabled) {
        flatShader.use();
        setUniformVec3f(flatShader.uniformLocation("lightPosition"), lightPosition);
        setUniformVec3f(flatShader.uniformLocation("viewPosition"), viewPosition);
        opengl::Program *program = &flatShader;
    }
    //gouraud
    else if(renderingMode == 3 && !texturesEnabled) {
        gouraudShader.use();
        setUniformVec3f(gouraudShader.uniformLocation("lightPosition"), lightPosition);
        opengl::Program *program = &gouraudShader;
    }
    //shading
    else if(renderingMode == 4 && !texturesEnabled) {
        phongShader.use();
        setUniformVec3f(phongShader.uniformLocation("lightPosition"), lightPosition);
        setUniformVec3f(phongShader.uniformLocation("viewPosition"), viewPosition);
        opengl::Program *program = &phongShader;
    }


    vao.bind();
    setUniformMat4f(program->uniformLocation("model"), g_M, true);
    setUniformMat4f(program->uniformLocation("view"), g_V, true);
    setUniformMat4f(program->uniformLocation("projection"), g_P, true);



    if(projectionMode == 0){  //ortho
            g_P = orthographicProjection( -1, 1, 1, -1, -3, 6);
        } else {  //perspective
            g_P = perspectiveProjection(30.,  // fov
                                        1.f,  // ratio
                                        0.1f, // distance to near plane
                                        100.f // distance to far plane
                                        );
        }

    glDrawElements(GL_TRIANGLES,
                   totalIndices,    // # of triangles * 3
                   GL_UNSIGNED_INT, // type of indices
                   (void *)0        // offset
                   );

    glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
    glfwPollEvents();        // will process event queue and carry on
  }

  // cleaup window, and glfw before exit
  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
