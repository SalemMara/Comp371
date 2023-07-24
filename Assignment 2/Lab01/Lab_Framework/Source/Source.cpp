// Salem Maraouch
// 40166837
//

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>


using namespace glm;
using namespace std;

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

int xAxisVertexArrayObject();

int zAxisVertexArrayObject();

int yAxisVertexArrayObject();

int armVertexArrayObject();

int handVertexArrayObject();

int handleVertexArrayObject();

int base1VertexArrayObject();

int base2VertexArrayObject();

int mid1VertexArrayObject();

int mid2VertexArrayObject();

int top1VertexArrayObject();

int top2VertexArrayObject();

bool initContext();

GLFWwindow * window = NULL;

int main(int argc, char*argv[])
{
	if (!initContext()) return -1;

	// Black background
	glClearColor(0.498f, 0.556f, 0.498f, 1.0f);

	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders();

	// We can set the shader once, since we have only one
	glUseProgram(shaderProgram);

	// Camera parameters for view transform
	////vec3 cameraPosition(5.0f, 8.0f, 15.0f);
	////vec3 cameraLookAt(10.0f, 0.0f, -1.0f);
	////vec3 cameraUp(0.0f, 20.0f, 0.0f);

	vec3 cameraPosition(5.0f, 8.0f, 15.0f);
	vec3 cameraLookAt(10.0f, 0.0f, -1.0f);
	vec3 cameraUp(0.0f, 20.0f, 0.0f);

	/*vec3 cameraPosition(20.0f, 20.0f, 90.0f);
	vec3 cameraLookAt(100.0f, 0.0f, 50.0f);
	vec3 cameraUp(0.0f, 20.0f, 0.0f);*/

	// Other camera parameters
	float cameraSpeed = 5.0f;
	float cameraFastSpeed = 2 * cameraSpeed;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

	float spinningCubeAngle = 0.0f;

	float fov = 90.0f;
	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(glm::radians(fov),            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	vec3 center = vec3(0, 0, 0); 
	// Set initial view matrix
	//mat4 viewMatrix = lookAt(cameraPosition,  // eye
	//	cameraPosition + cameraLookAt,  // center
	//	cameraUp); // up
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		center,  // center
		cameraUp); // up

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	// Define and upload geometry to the GPU here ...
	int xVao = xAxisVertexArrayObject();
	int zVao = zAxisVertexArrayObject();
	int yVao = yAxisVertexArrayObject();
	int armVao = armVertexArrayObject();
	int handVao = handVertexArrayObject();
	int handleVao = handleVertexArrayObject();
	int base1Vao = base1VertexArrayObject();
	int base2Vao = base2VertexArrayObject();
	int mid1Vao = mid1VertexArrayObject();
	int mid2Vao = mid2VertexArrayObject();
	int top1Vao = top1VertexArrayObject();
	int top2Vao = top2VertexArrayObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once before the Game Loop
	// Enable Backface culling
	//glEnable(GL_CULL_FACE);

	// @TODO 1 - Enable Depth Test
	// ...
	glEnable(GL_DEPTH_TEST);

	// translate arm
	float tA1 = 50.0f;
	float tA2 = 3.7f;
	float tA3 = 50.0f;

	// translate hand
	float tH1 = 51.0f;
	float tH2 = 5.7f;
	float tH3 = 50.0f;

	// translate handle
	float tHle1 = 51.0f;
	float tHle2 = 7.4;
	float tHle3 = 50.0f;

	// translate base1
	float tb1 = 51.45f;
	float tb2 = 9.3f;
	float tb3 = 50.0f;

	// translate base2
	float tba1 = 50.5f;
	float tba2 = 9.3;
	float tba3 = 50.0f;

	// translate mid1
	float tm1 = 50.0f;
	float tm2 = 11.0f;
	float tm3 = 50.0f;

	// translate mid2
	float tmi1 = 52.0f;
	float tmi2 = 11.0f;
	float tmi3 = 50.0f;
	
	// translate top1
	float tt1 = 51.45f;
	float tt2 = 12.8f;
	float tt3 = 50.0f;

	// translate top2
	float tto1 = 50.5f;
	float tto2 = 12.8f;
	float tto3 = 50.0f;

	// scale arm
	float sA1 = 0.5f;
	float sA2 = 3.0f;
	float sA3 = 0.5f;

	// scale hand
	float sH1 = 0.5f;
	float sH2 = 2.0f;
	float sH3 = 0.5f;

	// scale handle
	float sHle1 = 0.2f;
	float sHle2 = 3.0f;
	float sHle3 = 0.2f;

	// scale base1
	float sb1 = 0.2f;
	float sb2 = 1.5f;
	float sb3 = 0.2f;

	// scale base2
	float sba1 = 0.2f;
	float sba2 = 1.5;
	float sba3 = 0.2f;

	// scale mid1
	float sm1 = 0.2f;
	float sm2 = 2.5f;
	float sm3 = 0.2f;

	// scale mid2
	float smi1 = 0.2f;
	float smi2 = 2.5f;
	float smi3 = 0.2f;

	// scale top1
	float st1 = 0.2f;
	float st2 = 1.5f;
	float st3 = 0.2f;

	// scale top2
	float sto1 = 0.2f;
	float sto2 = 1.5f;
	float sto3 = 0.2f;

	// rotate arm
	float rA1 = 45.0f;
	float rA2 = 0.0f;
	float rA3 = -180.0f;

	// rotate hand
	float rH1 = 0.0f;
	float rH2 = 20.0f;
	float rH3 = 0.0f;

	// rotate handle
	float rHle1 = 0.0f;
	float rHle2 = 20.0f;
	float rHle3 = 0.0f;

	// rotate base1
	float rb1 = 45.0f;
	float rb2 = 0.0f;
	float rb3 = -190.0f;

	// rotate base2
	float rba1 = 45.0f;
	float rba2 = 0.0;
	float rba3 = 190.0f;

	// rotate mid1
	float rm1 = 0.0f;
	float rm2 = 20.0f;
	float rm3 = 0.0f;

	// rotate mid2
	float rmi1 = 0.0f;
	float rmi2 = 20.0f;
	float rmi3 = 0.0f;

	// rotate top1
	float rt1 = 45.0f;
	float rt2 = 0.0f;
	float rt3 = 190.0f;

	// rotate top2
	float rto1 = 45.0f;
	float rto2 = 0.0f;
	float rto3 = -190.0f;
	float s = 1.0; 

	float angle = 0.0f; 

	float randomX = 0.0f;
	float randomZ = 0.0f;
		//rand() / static_cast<float>(RAND_MAX);
	 //std::cout << random << std::endl;

	static GLenum renderType = GL_TRIANGLES;


	// Entering Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor

		// @TODO 1 - Clear Depth Buffer Bit as well
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw geometry
		glBindVertexArray(xVao);
		
		//creating the x-axis
		mat4 xAxisWorldMatrix = translate(mat4(10.0f), vec3(2.5f, 0.00f,0.0f)) * scale(mat4(10.0f), vec3(5, 0.1f, 0.1f));
		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xAxisWorldMatrix[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//creating the z-axis
		glBindVertexArray(zVao);

		
		mat4 zAxisWorldMatrix = translate(mat4(10.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &zAxisWorldMatrix[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//creating the y-axis
		glBindVertexArray(yVao);

		
		mat4 yAxisWorldMatrix = translate(mat4(10.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5, 0.1f));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yAxisWorldMatrix[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//creating arm model
		glBindVertexArray(armVao);

		mat4 new_rot = rotate(mat4(1.0f), glm::radians(angle), vec3(0.0f, 1.0f, 0.0f));

		//mat4 armWorldMatrix = translate(mat4(10.0f), vec3(tA1, tA2, tA3)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(45.0f, 0.0f, -180.0f)) * scale(mat4(1.0f), vec3(sA1, sA2, sA3));
		mat4 armWorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tA1) + randomX, (s*tA2), (s*tA3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rA1, rA2, rA3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sA1, sA2, sA3));
		//mat4 armWorldMatrix = translate(mat4(10.0f), vec3((s*tA1) + randomX, (angle*(s*tA2)), (s*tA3) + randomZ)) * new_rot * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sA1, sA2, sA3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &armWorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//creating hand model
		glBindVertexArray(handVao);
		// scale hand	
		
		//mat4 handWorldMatrix = translate(mat4(10.0f), vec3(tH1, tH2, tH3)) * rotate(mat4(1.0f), glm::radians(90.0f), vec3(0.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(sH1, sH2, sH3));
		mat4 handWorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tH1)+randomX, (s*tH2), (s*tH3)+ randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rH1, rH2, rH3)) * scale(mat4(1.0f), vec3(s, s, s)) *glm::scale(mat4(1.0f), vec3(sH1, sH2, sH3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &handWorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//create handle model
		glBindVertexArray(handleVao);

		
		mat4 handleWorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tHle1) + randomX, (s*tHle2), (s*tHle3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rHle1, rHle2, rHle3)) * scale(mat4(1.0f),vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sHle1, sHle2, sHle3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &handleWorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//create base1 model
		glBindVertexArray(base1Vao);

		
		mat4 base1WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tb1) + randomX, (s*tb2), (s*tb3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rb1, rb2, rb3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sb1, sb2, sb3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &base1WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//create base2 model
		glBindVertexArray(base2Vao);
		
		mat4 base2WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tba1) + randomX, (s*tba2), (s*tba3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rba1, rba2, rba3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sba1, sba2, sba3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &base2WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//glDrawArrays(renderType, 0, 36); // 36 vertices, starting at index 0

		// create mid1 model
		glBindVertexArray(mid1Vao);

		mat4 mid1WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tm1) + randomX, (s*tm2), (s*tm3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rm1, rm2, rm3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sm1, sm2, sm3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mid1WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		// create mid2 model
		glBindVertexArray(mid2Vao);

		mat4 mid2WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tmi1) + randomX, (s*tm2), (s*tm3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rmi1, rmi2, rmi3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(smi1, smi2, smi3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mid2WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//create top1 model
		glBindVertexArray(top1Vao);


		mat4 top1WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tt1) + randomX, (s*tt2), (s*tt3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rt1, rt2, rt3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(st1, st2, st3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &top1WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);

		//create top2 model
		glBindVertexArray(top2Vao);


		mat4 top2WorldMatrix = new_rot * translate(mat4(10.0f), vec3((s*tto1) + randomX, (s*tto2), (s*tto3) + randomZ)) * rotate(mat4(1.0f), glm::radians(45.0f), vec3(rto1, rto2, rto3)) * scale(mat4(1.0f), vec3(s, s, s)) * glm::scale(mat4(1.0f), vec3(sto1, sto2, sto3));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &top2WorldMatrix[0][0]);

		glDrawArrays(renderType, 0, 36);


		//creating the ground lines
		mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(-50.0f, -0.01f, -50.0f)) * scale(mat4(1.0f), vec3(100, 0.5f, 100));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);

		// Draw grid on the raquet
		int slices = 99; // Number of subdivisions in the grid
		vector<glm::vec3> vertices;
		vector<glm::uvec4> indices; // Store the indices that define the grid lines.

		// for loop to generate vertices along the x axis
		// For each loop, it calculates the position of the vertex based on the loop index and the total number of slices.
		for (int i = 0; i <= slices; i++) {
			float x = static_cast<float>(i) / static_cast<float>(slices);
			float y = 0.0f;
			vertices.push_back(glm::vec3(x, y, 0.0f));
			vertices.push_back(glm::vec3(x, y, 1.0f));
		}

		// for loop to generate vertices along the z axis
		for (int i = 0; i <= slices; i++) {
			float z = static_cast<float>(i) / static_cast<float>(slices);
			float y = 0.0f;
			vertices.push_back(glm::vec3(0.0f, y, z));
			vertices.push_back(glm::vec3(1.0f, y, z));
		}

		// This loop is responsible for creating horizontal lines (rows) in the grid.
		int numVerticesPerRow = slices + 1;

		for (int i = 0; i < slices; ++i) {
			int currentIndex = i * 2; // calculates the starting index for each row. Since each row requires two vertices
									  // to draw a line segment, the currentIndex is incremented by 2 for each iteration of the outer loop.

			// This loop is responsible for creating vertical lines (columns) in the grid.
			for (int j = 0; j < slices; ++j) {
				int topLeft = currentIndex + j * numVerticesPerRow;	// calculates the index for the top-left vertex of the current cell (square) in the grid.
				int topRight = topLeft + 1;	// calculates the index for the top-right vertex of the current cell.
				int bottomLeft = topLeft + numVerticesPerRow;	//  calculates the index for the bottom-left vertex of the current cell.
				int bottomRight = bottomLeft + 1;	// calculates the index for the bottom-right vertex of the current cell.

				// the line segment of the cell is drawn. uvec4 Represents the indices of the four vertices required to draw two line segments that form the edges of the cell.
				indices.push_back(glm::uvec4(topLeft, topRight, bottomRight, bottomLeft));
			}
		}

		// gridVao will be used to store the handle (ID) of the vertex array object for the grid.
		GLuint gridVao;
		glGenVertexArrays(1, &gridVao);	// This generates one vertex array object and stores its ID in the variable gridVao.
		glBindVertexArray(gridVao);	// This binds the newly created vertex array object gridVao

		// gridVbo will be used to store the handle (ID) of the vertex buffer object (VBO) for the grid's vertex data.
		GLuint gridVbo;
		glGenBuffers(1, &gridVbo);	// This generates one vertex buffer object and stores its ID in the variable gridVbo
		glBindBuffer(GL_ARRAY_BUFFER, gridVbo);	// This binds the newly created VBO gridVbo to the GL_ARRAY_BUFFER target, meaning it will be used to store vertex attribute data.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);	// This sends the vertex data stored in the vertices vector to the GPU.
		glEnableVertexAttribArray(0);	// This enables the vertex attribute at index 0 in the vertex shader (the attribute is the position attribute)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);	// It sets up the format of the position attribute

		// gridIbo will be used to store the handle(ID) of the index buffer object(IBO) for the grid's indices.
		GLuint gridIbo;
		glGenBuffers(1, &gridIbo);	// This generates one index buffer object and stores its ID in the variable gridIbo.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIbo);	// it will be used to store index data.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);	// This sends the index data stored in the indices vector to the GPU.
		
		glBindVertexArray(gridVao);	//This binds the vertex array object gridVao
		glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()) * 4, GL_UNSIGNED_INT, nullptr);	//  renders the grid using the data from the VAO and IBO. 

		
		// Spinning cube at camera position
		spinningCubeAngle += 180.0f * dt;
		/*
		// @TODO 7 - Draw in view space for first person camera
		if (cameraFirstPerson) {
			mat4 spinningCubeWorldMatrix = mat4(1.0);
			mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
				rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
				scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &spinningCubeViewMatrix[0][0]);
		}
		else {
			// In third person view, let's draw the spinning cube in world space, like any other models
			mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
				rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
				scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
		}

		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/
		glBindVertexArray(0);
		

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {	//translate whole model right
			tA1 += 0.1;
			tH1 += 0.1;
			tHle1 += 0.1f;
			tb1 += 0.1f;
			tba1 += 0.1f;
			tm1 += 0.1f;
			tmi1 += 0.1f;
			tt1 += 0.1f;
			tto1 += 0.1f;
		}
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { //translate whole model left
			tA1 -= 0.1;
			tH1 -= 0.1;
			tHle1 -= 0.1f;
			tb1 -= 0.1f;
			tba1 -= 0.1f;
			tm1 -= 0.1f;
			tmi1 -= 0.1f;
			tt1 -= 0.1f;
			tto1 -= 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {	//translate whole model up
			tA2 += 0.1;
			tH2 += 0.1;
			tHle2 += 0.1f;
			tb2 += 0.1f;
			tba2 += 0.1f;
			tm2 += 0.1f;
			tmi2 += 0.1f;
			tt2 += 0.1f;
			tto2 += 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { //translate whole model down
			tA2 -= 0.1;
			tH2 -= 0.1;
			tHle2 -= 0.1f;
			tb2 -= 0.1f;
			tba2 -= 0.1f;
			tm2 -= 0.1f;
			tmi2 -= 0.1f;
			tt2 -= 0.1f;
			tto2 -= 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {	//scale whole model up
			sA1 += 0.01;
			sH1 += 0.01;
			sHle1 += 0.01f;
			sb1 += 0.01f;
			sba1 += 0.01f;
			sm1 += 0.01f;
			smi1 += 0.01f;
			st1 += 0.01f;
			sto1 += 0.01f;

			// 
			s += 0.01f; 
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) { //scale whole model down
			sA1 -= 0.01;
			sH1 -= 0.01;
			sHle1 -= 0.01f;
			sb1 -= 0.01f;
			sba1 -= 0.01f;
			sm1 -= 0.01f;
			smi1 -= 0.01f;
			st1 -= 0.01f;
			sto1 -= 0.01f;

			s -= 0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {	//rotate right arm around y axis
			//rA2 += 5;
			//rH2 += 5;
			//rHle2 += 5;
			//rb2 += 5;
			//rba2 += 5;
			//rm2 += 5;
			//rmi2 += 5;
			//rt2 += 5;
			//rto2 += 5;

			angle += 5;
		}

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {	//rotate left arm around y axis
			//rA2 -= 5;
			//rH2 -= 5;
			//rHle2 -= 5;
			//rb2 -= 5;
			//rba2 -= 5;
			//rm2 -= 5;
			//rmi2 -= 5;
			//rt2 -= 5;
			//rto2 -= 5;

			angle -= 5;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {	//rotate right hand around y axis
			//rH2 += 5;
			//rHle2 += 5;
			//rb2 += 5;
			//rba2 += 5;
			//rm2 += 5;
			//rmi2 += 5;
			//rt2 += 5;
			//rto2 += 5;

			angle += 5;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {	//rotate left hand around y axis
			//rH2 -= 5;
			//rHle2 -= 5;
			//rb2 -= 5;
			//rba2 -= 5;
			//rm2 -= 5;
			//rmi2 -= 5;
			//rt2 -= 5;
			//rto2 -= 5;

			angle -= 5;
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			renderType = GL_POINTS;
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			renderType = GL_LINES;
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			renderType = GL_TRIANGLES;
		}

		float maxDistance = 50.0f;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

			randomX = maxDistance / 2.0f - (rand() % static_cast<int>(maxDistance));
			randomZ = maxDistance / 2.0f - (rand() % static_cast<int>(maxDistance));
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera down
		{
			cameraFirstPerson = true;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera up
		{
			cameraFirstPerson = true;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			fov += 0.20;
			// Set projection matrix for shader, this won't change
			projectionMatrix = glm::perspective(glm::radians(fov),            // field of view in degrees
				800.0f / 600.0f,  // aspect ratio
				0.01f, 100.0f);   // near and far (near > 0)
		}

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			fov -= 0.20;
			// Set projection matrix for shader, this won't change
			projectionMatrix = glm::perspective(glm::radians(fov),            // field of view in degrees
				800.0f / 600.0f,  // aspect ratio
				0.01f, 100.0f);   // near and far (near > 0)
		}

		//// Pan Camera right
		//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		//	pan += 10;
		//}

		//// Pan Camera left
		//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		//	pan -= 10;
		//}

		GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 60.0f;
		cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
		cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);

		// @TODO 5 = use camera lookat and side vectors to update positions with ASDW
		// adjust code below
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition.x -= currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition.x += currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera down
		{
			cameraPosition.y -= currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera up
		{
			cameraPosition.y += currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // bring camera back to initial position
		{
			cameraPosition = vec3(-5.0f, 8.0f, 15.0f);
		}

		mat4 viewMatrix = mat4(1.0);

		if (cameraFirstPerson) {
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			float radius = 5.0f;
			glm::vec3 position = cameraPosition - radius * cameraLookAt;
			viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
		}

		GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	
	}


	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

const char* getVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}

const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		" uniform vec3 color ; "
		"void main()"
		"{"
		"  FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f); "
		"}";

	//FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f) ; 
}
// FragColor = vevec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
int compileAndLinkShaders()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int xAxisVertexArrayObject()
{
	vec3 xVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), //left - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), // far - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right - red
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top - red
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(xVertexArray), xVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int zAxisVertexArrayObject() {

	vec3 zVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), //left - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), // far - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // bottom - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // near - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // right - red
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // top - red
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zVertexArray), zVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int yAxisVertexArrayObject() {

	vec3 yVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), //left - green
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), // far - green
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // bottom - green
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // right - green
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // top - green
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(yVertexArray), yVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int armVertexArrayObject()
{
	vec3 armVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f), //left - beige
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f), // far - beige
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // bottom - beige
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // near - beige
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // right - beige
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // top - beige
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(armVertexArray), armVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int handVertexArrayObject()
{
	vec3 armVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f), //left - beige
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f), // far - beige
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // bottom - beige
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // near - beige
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // right - beige
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f), // top - beige
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(0.93f, 0.86f, 0.61f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(0.93f, 0.86f, 0.61f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(armVertexArray), armVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int handleVertexArrayObject()
{
	vec3 handleVertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), // far - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // bottom - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // near - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // right - red
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // top - red
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(handleVertexArray), handleVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int base1VertexArrayObject()
{
	vec3 base1VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), //left - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), // far - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right - white
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top - white
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base1VertexArray), base1VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int base2VertexArrayObject()
{
	vec3 base2VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), //left - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), // far - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right - white
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top - white
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base2VertexArray), base2VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int mid1VertexArrayObject()
{
	vec3 mid1VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), // far - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // bottom - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // near - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // right - red
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // top - red
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mid1VertexArray), mid1VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int mid2VertexArrayObject()
{
	vec3 mid2VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), // far - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // bottom - red
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // near - red
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // right - red
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // top - red
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mid2VertexArray), mid2VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int top1VertexArrayObject()
{
	vec3 top1VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), //left - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), // far - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right - white
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top - white
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(top1VertexArray), top1VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int top2VertexArrayObject()
{
	vec3 top2VertexArray[] = { // position,
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), //left - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), // far - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom - white
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near - white
		glm::vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right - white
		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top - white
		glm::vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(top2VertexArray), top2VertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}


bool initContext() {     // Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 1024x768
	window = glfwCreateWindow(1024, 768, "Comp - 371 Summer 2023", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// @TODO 3 - Disable mouse cursor
	// ...
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}
