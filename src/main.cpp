#include <iostream>
#include "ext/gl_core_3_3.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "configfile.h"
#include "messagebox.h"

class String
{
    private:
        const char* stringData;
    public:
        String(const char* cstr) : stringData(cstr) {}
        const char* str() const
        {
            return stringData;
        }
};

class Shader
{
    private:
        GLuint vertID, fragID, progID;
        GLuint createShader(GLenum shaderType, const char* source)
        {
            // Tutorial code from http://www.arcsynthesis.org/gltut
            // Copyright © 2012 Jason L. McKesson
            // Variable names changed take custom parameters and work with glLoadGen func_cpp functions
            GLuint shader = gl::CreateShader(shaderType);
            gl::ShaderSource(shader, 1, &source, NULL);
            gl::CompileShader(shader);
            GLint status;
            gl::GetShaderiv(shader, gl::COMPILE_STATUS, &status);
            if (status == gl::FALSE_)
            {
               GLint infoLogLength;
               gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &infoLogLength);

               GLchar *strInfoLog = new GLchar[infoLogLength + 1];
               gl::GetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

               const char *strShaderType = NULL;
               switch(shaderType)
               {
               case gl::VERTEX_SHADER: strShaderType = "vertex"; break;
               case gl::GEOMETRY_SHADER: strShaderType = "geometry"; break;
               case gl::FRAGMENT_SHADER: strShaderType = "fragment"; break;
               }

               fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
               delete[] strInfoLog;
            }
            return shader;
        }
    public:
        Shader(const File& vert, const File& frag)
        {
            // Create both shaders
            vertID = createShader(gl::VERTEX_SHADER, vert.string().c_str());
            fragID = createShader(gl::FRAGMENT_SHADER, frag.string().c_str());

            // Create and link a program
            progID = gl::CreateProgram();
            gl::AttachShader(progID, vertID);
            gl::AttachShader(progID, fragID);
            gl::LinkProgram(progID);
            gl::DetachShader(progID, vertID);
            gl::DetachShader(progID, fragID);
        }
        const int getProgram() {return progID;}
};

const float triangle[] = {
    // Positions
    -1.0, -1.0,  0.0,  1.0,
     1.0, -1.0,  0.0,  1.0,
     0.0,  1.0,  0.0,  1.0,
    // Colors
     1.0,  0.0,  0.0,  1.0,
     0.0,  1.0,  0.0,  1.0,
     0.0,  0.0,  1.0,  1.0
};

class TestScene
{
    private:
        Shader shader;
        GLuint VBO_id;
        GLuint VAO_id;
        glm::vec2 offset;
        GLuint offsetLocation;
        double deltaTime;
        double currentFrame;
        double lastFrame;
    public:
        TestScene() : shader("Shaders/UnlitGeneric.vert", "Shaders/UnlitGeneric.frag"), offset(-2, 0), lastFrame(glfwGetTime())
        {
            gl::ClearColor(0, 0, 0, 1);
            gl::ClearDepth(1);
            gl::Enable(gl::DEPTH_TEST);
            gl::Viewport(0, 0, 1280, 720);

            // Set up an offset uniform for the shader
            offsetLocation = gl::GetUniformLocation(shader.getProgram(), "offset");

            // Generate a VBO and VAO
            gl::GenBuffers(1, &VBO_id);
            gl::GenVertexArrays(1, &VAO_id);
            // Bind them
            gl::BindBuffer(gl::ARRAY_BUFFER, VBO_id);
            gl::BindVertexArray(VAO_id);
            // Set the buffer data
            gl::BufferData(gl::ARRAY_BUFFER, sizeof(triangle), triangle, gl::STATIC_DRAW);
            // Enable the first two vertex attributes
            gl::EnableVertexAttribArray(0);
            gl::EnableVertexAttribArray(1);
            // Attribute index, 4 values per position, inform they're floats, unknown, space between values, first value
            gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 0, 0);
            gl::VertexAttribPointer(1, 4, gl::FLOAT, gl::FALSE_, 0, (void*) 48);
            // And clean up
            gl::DisableVertexAttribArray(0);
            gl::DisableVertexAttribArray(1);
            gl::BindBuffer(gl::ARRAY_BUFFER, 0);
            gl::BindVertexArray(0);
        }
        void draw()
        {
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Set the shader program
            gl::UseProgram(shader.getProgram());
            // Give it the offset value
            if (offset.x > 2)
            {
                offset.x = -2;
            }
            gl::Uniform2f(offsetLocation, offset.x += (0.5 * deltaTime), offset.y);
            // Bind the vertex array
            gl::BindVertexArray(VAO_id);
            gl::EnableVertexAttribArray(0);
            gl::EnableVertexAttribArray(1);
            // Draw the values
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
            // And unbind the vertex array
            gl::BindVertexArray(0);
        }
};

void cbfun_windowResized(GLFWwindow* window, int width, int height)
{
    gl::Viewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    PHYSFS_init(argv[0]);
    setRootPath("../Data");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* Window = glfwCreateWindow(1280, 720, "GL App", NULL, NULL);
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1);
    if (Window == nullptr || !gl::sys::LoadFunctions())
    {
        MessageBoxError("Fatal Error", "Could not initialize an OpenGL context\nMake sure your computer supports OpenGL 3.3 and drivers are updated");
        return -1;
    }
    glfwSetWindowSizeCallback(Window, cbfun_windowResized);
    fprintf(stdout, "OpenGL version: %s\nDisplay device: %s\nVendor: %s\n", gl::GetString(gl::VERSION), gl::GetString(gl::RENDERER), gl::GetString(gl::VENDOR));

    TestScene scene;
    bool runGame = true;
    while (runGame)
    {
        glfwPollEvents();
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS or glfwWindowShouldClose(Window))
        {
            runGame = false;
        }
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        scene.draw();
        glfwSwapBuffers(Window);
    }
    glfwDestroyWindow(Window);
    return 0;
}
