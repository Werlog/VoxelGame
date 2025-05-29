#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"

class SkyBox
{
public:

	SkyBox(const glm::vec3& bColor, const glm::vec3& tColor, Shader* skyboxShader);

	void render(Camera& camera);
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int projectionLoc;
	unsigned int viewLoc;

	Shader* skyboxShader;

	glm::vec3 bottomColor;
	glm::vec3 topColor;

	void createMesh();
	void setupShader();
};
