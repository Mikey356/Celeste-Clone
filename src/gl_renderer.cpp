#include "gl_renderer.h"
// #############################################################################
//                           OpenGL Constants
// #############################################################################
//const char* TEXTURE_PATH = "assets/textures/TEXTURE_ATLAS.png";


// #############################################################################
//                           OpenGL Structs
// #############################################################################

struct GLContext
{
    GLuint programID;
};

// #############################################################################
//                           OpenGL Globals
// #############################################################################
static GLContext glContext;

// #############################################################################
//                           OpenGL Functions
// #############################################################################
static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                         GLsizei length, const GLchar* message, const void* user)
{
  if(severity == GL_DEBUG_SEVERITY_LOW || 
     severity == GL_DEBUG_SEVERITY_MEDIUM ||
     severity == GL_DEBUG_SEVERITY_HIGH)
  {
    SM_ASSERT(false, "OpenGL Error: %s", message);
  }
  else
  {
    SM_TRACE((char*)message);
  }
}
