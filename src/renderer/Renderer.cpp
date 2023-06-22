#include "Renderer.h"

#include "stb_image.h"

#include <iostream>
#include <array>

// Shader headers
#include "GLM\gtc\matrix_transform.hpp"
#include "GLM\gtc\type_ptr.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

const unsigned int MAX_TEXTURES = 32;
const unsigned int MAX_QUADS = 2000;
const unsigned int MAX_VERTICES = MAX_QUADS * 4;
const unsigned int MAX_INDICES = MAX_QUADS * 6;
const float MAX_ZOOM = 20.0f;
const float MIN_ZOOM = 4.0f;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoord;
	float texID;
    glm::vec4 colour;
};
struct RendererData
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;

	uint32_t indicesCount = 0;

	Vertex* vertexBuffer = nullptr;
	Vertex* vertexBufferPtr = nullptr;

    unsigned int textures[MAX_TEXTURES];
    int numOfTextures = 0;
};
struct CameraData
{
    float zoom = 6.0f;
    const float zoomSense = 2.0f;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};
class Shader
{
public:
    Shader()
    {
        m_ID = 0;
    }
    void Init(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read files buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            // convert to string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR: Shader file no successfully read: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // compile
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);
        checkCompileErrors(m_ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Use()
    {
        glUseProgram(m_ID);
    }
    void SetMat4(const std::string& name, glm::mat4 value)
    {
        GLint location = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }

    unsigned int m_ID;
    
private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

static RendererData s_RData;
static CameraData s_CData;
static GLFWwindow* s_Window = nullptr;
static Shader s_Shader;

//----------------------------------------------------------------------------
//----------------------------- Renderer -------------------------------------
//----------------------------------------------------------------------------

// Constructor and sub-routines.
Renderer2D::Renderer2D()
{
    Init();
}
void Renderer2D::Init()
{
    InitWindow();
    InitBuffers();
    s_RData.vertexBuffer = new Vertex[MAX_VERTICES];
    s_RData.vertexBufferPtr = s_RData.vertexBuffer;
    s_Shader.Init("res/shaders/V_Simple.shader", "res/shaders/F_Simple.shader");
}
void Renderer2D::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    s_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer", NULL, NULL);
    if (s_Window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(s_Window);
    glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR);

    // Load OpenGL function pointers.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}
void Renderer2D::InitBuffers()
{
    glGenVertexArrays(1, &s_RData.vao);
    glBindVertexArray(s_RData.vao);

    glGenBuffers(1, &s_RData.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_RData.vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, colour));

    uint32_t indices[MAX_INDICES];
    uint32_t offset = 0;
    for (int i = 0; i < MAX_INDICES; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }
    glGenBuffers(1, &s_RData.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RData.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// Rendering specific.
void Renderer2D::StartBatch()
{
    s_RData.vertexBufferPtr = s_RData.vertexBuffer;
    s_RData.indicesCount = 0;
}
void Renderer2D::SubmitBatch()
{
    float boxWidth = ((float)SCR_WIDTH) / s_CData.zoom;
    float boxHeight = ((float)SCR_HEIGHT) / s_CData.zoom;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), s_CData.position);
    glm::mat4 viewMatrix = glm::inverse(transform);
    glm::mat4 projMatrix = glm::ortho(-boxWidth, boxWidth, -boxHeight, boxHeight, -1.0f, 1.0f);
    
    s_Shader.Use();
    s_Shader.SetMat4("view", viewMatrix);
    s_Shader.SetMat4("projection", projMatrix);

    auto loc = glGetUniformLocation(s_Shader.m_ID, "u_Textures");
    int samplers[MAX_TEXTURES];
    for (int i = 0; i < MAX_TEXTURES; i++) samplers[i] = i;
    glUniform1iv(loc, MAX_TEXTURES, samplers);
    for (unsigned int slot = 0; slot < s_RData.numOfTextures; slot++)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, s_RData.textures[slot]);
    }

    GLsizeiptr size = (uint8_t*)s_RData.vertexBufferPtr - (uint8_t*)s_RData.vertexBuffer;
    glBindVertexArray(s_RData.vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_RData.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RData.ibo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_RData.vertexBuffer);
    glDrawElements(GL_TRIANGLES, s_RData.indicesCount, GL_UNSIGNED_INT, nullptr);
}
void Renderer2D::AddQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& texCoords, const glm::vec4& colour, float texIndex)
{
    s_RData.vertexBufferPtr->position[0] = position.x;
    s_RData.vertexBufferPtr->position[1] = position.y;
    s_RData.vertexBufferPtr->texCoord[0] = texCoords[0];
    s_RData.vertexBufferPtr->texCoord[1] = texCoords[1];
    s_RData.vertexBufferPtr->texID = texIndex;
    s_RData.vertexBufferPtr->colour[0] = colour[0];
    s_RData.vertexBufferPtr->colour[1] = colour[1];
    s_RData.vertexBufferPtr->colour[2] = colour[2];
    s_RData.vertexBufferPtr->colour[3] = colour[3];
    s_RData.vertexBufferPtr++;
   
    s_RData.vertexBufferPtr->position[0] = position.x + size.x;
    s_RData.vertexBufferPtr->position[1] = position.y;
    s_RData.vertexBufferPtr->texCoord[0] = texCoords[2];
    s_RData.vertexBufferPtr->texCoord[1] = texCoords[1];
    s_RData.vertexBufferPtr->texID = texIndex;
    s_RData.vertexBufferPtr->colour[0] = colour[0];
    s_RData.vertexBufferPtr->colour[1] = colour[1];
    s_RData.vertexBufferPtr->colour[2] = colour[2];
    s_RData.vertexBufferPtr->colour[3] = colour[3];
    s_RData.vertexBufferPtr++;

    s_RData.vertexBufferPtr->position[0] = position.x + size.x;
    s_RData.vertexBufferPtr->position[1] = position.y + size.y;
    s_RData.vertexBufferPtr->texCoord[0] = texCoords[2];
    s_RData.vertexBufferPtr->texCoord[1] = texCoords[3];
    s_RData.vertexBufferPtr->texID = texIndex;
    s_RData.vertexBufferPtr->colour[0] = colour[0];
    s_RData.vertexBufferPtr->colour[1] = colour[1];
    s_RData.vertexBufferPtr->colour[2] = colour[2];
    s_RData.vertexBufferPtr->colour[3] = colour[3];
    s_RData.vertexBufferPtr++;

    s_RData.vertexBufferPtr->position[0] = position.x;
    s_RData.vertexBufferPtr->position[1] = position.y + size.y;
    s_RData.vertexBufferPtr->texCoord[0] = texCoords[0];
    s_RData.vertexBufferPtr->texCoord[1] = texCoords[3];
    s_RData.vertexBufferPtr->texID = texIndex;
    s_RData.vertexBufferPtr->colour[0] = colour[0];
    s_RData.vertexBufferPtr->colour[1] = colour[1];
    s_RData.vertexBufferPtr->colour[2] = colour[2];
    s_RData.vertexBufferPtr->colour[3] = colour[3];
    s_RData.vertexBufferPtr++;

    s_RData.indicesCount += 6;
    if (s_RData.indicesCount == MAX_INDICES) Flush();
}
void Renderer2D::AddQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour)
{
    AddQuad(position, size, glm::vec4(0.0f), colour, -1.0f);
}
void Renderer2D::AddQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& texCoords, float texIndex)
{
    AddQuad(position, size, texCoords, glm::vec4(0.0f), texIndex);
}
void Renderer2D::Flush() 
{
    SubmitBatch();
    StartBatch();
}
void Renderer2D::ClearScreen(glm::vec4 clearColour)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
}
float Renderer2D::LoadTexture(const char* filePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "loaded image" << std::endl;
    }
    else 
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float texIndex = (float) s_RData.numOfTextures;
    if (s_RData.numOfTextures < MAX_TEXTURES)
    {
        s_RData.textures[s_RData.numOfTextures] = texture;
        s_RData.numOfTextures++;
    }
    else return texIndex--;
    return texIndex;
}

// Window and Camera.
GLFWwindow* Renderer2D::GetWindow()
{
	return s_Window;
}
void Renderer2D::SetCameraPosition(glm::vec2& position)
{
    s_CData.position.x = position.x;
    s_CData.position.y = position.y;
}
void Renderer2D::MoveCamera(float dx, float dy)
{
    s_CData.position.x += dx;
    s_CData.position.y += dy;
}
void Renderer2D::ZoomCamera(float zoom)
{
    s_CData.zoom += zoom * s_CData.zoomSense;
    if (s_CData.zoom > MAX_ZOOM) s_CData.zoom = MAX_ZOOM;
    if (s_CData.zoom < MIN_ZOOM) s_CData.zoom = MIN_ZOOM;
}