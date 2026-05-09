#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    GLuint VAO, VBO;
    float vertices[] = {x0, y0, 0.0f,x1, y1, 0.0f,x2, y2, 0.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return VAO;
}

GLuint createShaderProgram() {
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    return shaderProgram;
}

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "5 Triangulos", 0, 0);
    glfwMakeContextCurrent(window);

    glewInit();

    GLuint shaderProgram = createShaderProgram();

    GLuint triangulos[5];
    triangulos[0] = createTriangle(-0.9f, -0.8f, -0.5f, -0.8f, -0.7f, -0.4f);
    triangulos[1] = createTriangle(-0.4f, -0.8f,  0.0f, -0.8f, -0.2f, -0.4f);
    triangulos[2] = createTriangle( 0.1f, -0.8f,  0.5f, -0.8f,  0.3f, -0.4f);
    triangulos[3] = createTriangle(-0.65f, 0.0f, -0.25f, 0.0f, -0.45f, 0.4f);
    triangulos[4] = createTriangle( 0.25f, 0.0f,  0.65f, 0.0f,  0.45f, 0.4f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        for (int i = 0; i < 5; i++) {
            glBindVertexArray(triangulos[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
