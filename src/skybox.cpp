#include "skybox.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


SkyBox::SkyBox(const glm::vec3& bColor, const glm::vec3& tColor, Shader* skyboxShader)
{
    this->bottomColor = bColor;
    this->topColor = tColor;
    this->skyboxShader = skyboxShader;

    VAO = 0;
    VBO = 0;
    EBO = 0;
    createMesh();
    setupShader();
}

void SkyBox::render(Camera& camera)
{
    glUseProgram(skyboxShader->getProgramHandle());

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glUseProgram(0);
}

void SkyBox::createMesh()
{
    if (VAO != 0) return;

	constexpr float vertices[] =
	{
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f
	};

    constexpr int indices[] =
    {
        0, 1, 3,
        3, 1, 2,
        4, 5, 7,
        7, 5, 6,
        8, 9, 11,
        11, 9, 10,
        12, 13, 15,
        15, 13, 14,
        16, 17, 19,
        19, 17, 18,
        20, 21, 23,
        23, 21, 22
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
}

void SkyBox::setupShader()
{
    glUseProgram(skyboxShader->getProgramHandle());
    unsigned int topColorLoc = glGetUniformLocation(skyboxShader->getProgramHandle(), "topSkyColor");
    unsigned int bottomColorLoc = glGetUniformLocation(skyboxShader->getProgramHandle(), "bottomSkyColor");
    projectionLoc = glGetUniformLocation(skyboxShader->getProgramHandle(), "projection");
    viewLoc = glGetUniformLocation(skyboxShader->getProgramHandle(), "view");


    glUniform3fv(topColorLoc, 1, glm::value_ptr(topColor));
    glUniform3fv(bottomColorLoc, 1, glm::value_ptr(bottomColor));

    glUseProgram(0);
}
