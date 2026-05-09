#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Triangle {float r;float g;float b; float x1; float x2; float x3; float y1; float y2; float y3; GLuint VAO, VBO;};
double numClicks = 0;
std::vector<Triangle> triangles;
Triangle t;

GLuint VAO, VBO, shaderProgram;
int windowWidth = 800;
int windowHeight = 800;

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2)
{
    GLfloat vertices[] = {x0, y0, 0.0f,
                          x1, y1, 0.0f,
                          x2, y2, 0.0f};
 
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
 
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
 
    return vao;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float x = (2.0f * mouseX) / windowWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / windowHeight;

        numClicks ++;

        if (numClicks == 1){
            t.x1 = x;
            t.y1 = y;
        }if (numClicks == 2) {
            t.x2 = x;
            t.y2 = y;
        }if (numClicks == 3) {
            t.x3 = x;
            t.y3 = y;
        }

        if (numClicks == 3)
        {
            t.r = (float)rand() / RAND_MAX;
            t.g = (float)rand() / RAND_MAX;
            t.b = (float)rand() / RAND_MAX;

            t.VAO = createTriangle(t.x1, t.y1, t.x2, t.y2, t.x3, t.y3);
            triangles.push_back(t);
            numClicks = 0;
            t = {}; 
        }
        
    }
}

GLuint createShaderProgram() {
    const char* vertexShaderSource ="#version 330 core\n""layout (location = 0) in vec3 aPos;\n""uniform vec2 offset;\n""void main() {\n""   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);\n""}\n";

    const char* fragmentShaderSource ="#version 330 core\n""out vec4 FragColor;\n""uniform vec3 inColor;\n""void main() {\n""   FragColor = vec4(inColor, 1.0);\n""}\n";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}

int main() {
    srand(time(0));
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Triangulos", 0, 0);
    glfwMakeContextCurrent(window);

    glewInit();

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    shaderProgram = createShaderProgram();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        for (int i = 0; i < triangles.size(); i++) {
            //glUniform2f(glGetUniformLocation(shaderProgram, "offset"), triangles[i].x1, triangles[i].y1);
            glUniform3f(glGetUniformLocation(shaderProgram, "inColor"), triangles[i].r, triangles[i].g, triangles[i].b);
            glBindVertexArray(triangles[i].VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
