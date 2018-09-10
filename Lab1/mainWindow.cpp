// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fileop/fileLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void update() {
}

int main() {
	if(!glfwInit()) {
    	return -3;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    return -2;
	}   
	glViewport(0, 0, 800 * 2, 600 * 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int  success;
	char infoLog[512];

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, FileLoader("simpleShader.vert"), NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -4;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, FileLoader("simpleShader.frag"), NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -4;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    	std::cout << "ERROR::PROGRAM::LINK::LINK_FAILED\n" << infoLog << std::endl;
		return -4;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);  

	float vertices[] = {
	     0.5f,  0.5f, 0.0f,  // top right
	     0.5f, -0.5f, 0.0f,  // bottom right
	    -0.5f, -0.5f, 0.0f,  // bottom left
	    -0.5f,  0.5f, 0.0f   // top left 
	};

	float teapotVertices[] = {0, 0, 0.488037, 0.00390625, 0.0421881, 0.476326, 0.00390625, -0.0421881, 0.476326, 0.0107422, 0, 0.575333, 0.0125, 0.0562508, 0.450561, 0.0125, -0.0562508, 0.450561, 0.0195312, 0, 0.413654, 0.0210938, 0.0421881, 0.424797, 0.0210938, -0.0421881, 0.424797, 0.025, 0, 0.413086, 0.03875, 0.19625, 0.488037, 0.03875, -0.19625, 0.488037, 0.0390625, 0, 0.66803, 0.0486597, 0.192034, 0.575333, 0.0486597, -0.192034, 0.575333, 0.0567676, 0.188584, 0.413654, 0.0567676, -0.188584, 0.413654, 0.0625, 0, 0.358795, 0.0747852, 0.180918, 0.66803, 0.0747852, -0.180918, 0.66803, 0.0791016, 0, 0.764481, 0.0964063, 0.171719, 0.358795, 0.0964063, -0.171719, 0.358795, 0.1, 0, 0.769043, 0.103906, 0.0421881, 0.777779, 0.103906, -0.0421881, 0.777779, 0.105469, 0, 0.32156, 0.111721, 0.165203, 0.764481, 0.111721, -0.165203, 0.764481, 0.1125, 0.0562508, 0.796997, 0.1125, -0.0562508, 0.796997, 0.121094, 0.0421881, 0.816215, 0.121094, -0.0421881, 0.816215, 0.125, 0, 0.300049, 0.125, 0, 0.824951, 0.125, 0, 0.863037, 0.136045, 0.154853, 0.32156, 0.136045, -0.154853, 0.32156, 0.137695, 0, 0.881027, 0.145, 0.355, 0.488037, 0.145, -0.355, 0.488037, 0.149219, 0, 0.887024, 0.15, 0, 0.863037, 0.152627, 0.347373, 0.575333, 0.152627, -0.347373, 0.575333, 0.154062, 0.147187, 0.300049, 0.154062, 0.147187, 0.863037, 0.154062, -0.147187, 0.300049, 0.154062, -0.147187, 0.863037, 0.154883, 0, 0.881027, 0.158867, 0.341133, 0.413654, 0.158867, -0.341133, 0.413654, 0.165774, 0.142205, 0.881027, 0.165774, -0.142204, 0.881027, 0.172734, 0.327266, 0.66803, 0.172734, -0.327266, 0.66803, 0.175, 0, 0.863037, 0.176404, 0.137682, 0.887024, 0.176404, -0.137681, 0.887024, 0.177125, 0.137375, 0.863037, 0.177125, -0.137375, 0.863037, 0.181629, 0.135459, 0.881027, 0.181629, -0.135458, 0.881027, 0.189375, 0.310625, 0.358795, 0.189375, -0.310625, 0.358795, 0.200187, 0.127562, 0.863037, 0.200187, -0.127562, 0.863037, 0.201162, 0.298838, 0.764481, 0.201162, -0.298838, 0.764481, 0.210938, 0, 0.884945, 0.219883, 0.280117, 0.32156, 0.219883, -0.280117, 0.32156, 0.23334, 0.113457, 0.884945, 0.23334, -0.113457, 0.884945, 0.23375, 0.26625, 0.300049, 0.23375, 0.26625, 0.863037, 0.23375, -0.26625, 0.300049, 0.23375, -0.26625, 0.863037, 0.242764, 0.257238, 0.881027, 0.242764, -0.257235, 0.881027, 0.250945, 0.249056, 0.887024, 0.250945, -0.249054, 0.887024, 0.2515, 0.2485, 0.863037, 0.2515, -0.2485, 0.863037, 0.254967, 0.245034, 0.881027, 0.254967, -0.245033, 0.881027, 0.26925, 0.23075, 0.863037, 0.26925, -0.23075, 0.863037, 0.29375, 0, 0.900055, 0.294766, 0.205234, 0.884945, 0.294766, -0.205234, 0.884945, 0.30375, 0.46125, 0.488037, 0.30375, -0.46125, 0.488037, 0.307966, 0.45134, 0.575333, 0.307966, -0.45134, 0.575333, 0.309734, 0.080953, 0.900055, 0.309734, -0.080953, 0.900055, 0.311416, 0.443233, 0.413654, 0.311416, -0.443233, 0.413654, 0.319082, 0.425215, 0.66803, 0.319082, -0.425215, 0.66803, 0.328281, 0.403594, 0.358795, 0.328281, -0.403594, 0.358795, 0.334797, 0.388279, 0.764481, 0.334797, -0.388279, 0.764481, 0.345146, 0.363955, 0.32156, 0.345146, -0.363955, 0.32156, 0.352812, 0.345937, 0.300049, 0.352812, 0.345937, 0.863037, 0.352812, -0.345937, 0.300049, 0.352812, -0.345937, 0.863037, 0.353562, 0.146438, 0.900055, 0.353562, -0.146438, 0.900055, 0.357795, 0.334229, 0.881027, 0.357795, -0.334224, 0.881027, 0.362318, 0.323599, 0.887024, 0.362318, -0.323595, 0.887024, 0.362625, 0.322875, 0.863037, 0.362625, -0.322875, 0.863037, 0.364542, 0.318373, 0.881027, 0.364542, -0.318371, 0.881027, 0.372437, 0.299813, 0.863037, 0.372437, -0.299813, 0.863037, 0.385938, 0, 0.915394, 0.386543, 0.266661, 0.884945, 0.386543, -0.266661, 0.884945, 0.394777, 0.0447694, 0.915394, 0.394777, -0.0447694, 0.915394, 0.414844, 0, 1.03776, 0.41875, 0, 1.00793, 0.419016, 0.0809842, 0.915394, 0.419016, -0.0809842, 0.915394, 0.419047, 0.190266, 0.900055, 0.419047, -0.190266, 0.900055, 0.421414, 0.0335127, 1.03776, 0.421414, -0.0335127, 1.03776, 0.425021, 0.0319696, 1.00793, 0.425021, -0.0319696, 1.00793, 0.43946, 0.0605401, 1.03776, 0.43946, -0.0605401, 1.03776, 0.442242, 0.0577578, 1.00793, 0.442242, -0.0577578, 1.00793, 0.45, 0, 0.937988, 0.450781, 0, 0.971153, 0.453875, 0.0196248, 0.937988, 0.453875, -0.0196248, 0.937988, 0.454586, 0.0193479, 0.971153, 0.454586, -0.0193479, 0.971153, 0.45523, 0.105222, 0.915394, 0.45523, -0.105222, 0.915394, 0.4645, 0.0354995, 0.937988, 0.4645, -0.0354995, 0.937988, 0.465028, 0.0349715, 0.971153, 0.465028, -0.0349715, 0.971153, 0.466487, 0.0785866, 1.03776, 0.466487, -0.0785866, 1.03776, 0.46803, 0.0749796, 1.00793, 0.46803, -0.0749796, 1.00793, 0.480375, 0.0461242, 0.937988, 0.480375, -0.0461242, 0.937988, 0.480652, 0.0454139, 0.971153, 0.480652, -0.0454139, 0.971153, 0.5, 0, 1.05005, 0.5, 0.0492185, 0.971153, 0.5, 0.049999, 0.937988, 0.5, 0.0812503, 1.00793, 0.5, 0.0851567, 1.03776, 0.5, 0.114062, 0.915394, 0.5, 0.20625, 0.900055, 0.5, 0.289063, 0.884945, 0.5, 0.325001, 0.863037, 0.5, 0.34512, 0.881027, 0.5, 0.35, 0.863037, 0.5, 0.350785, 0.887024, 0.5, 0.362308, 0.881027, 0.5, 0.375, 0.300049, 0.5, 0.375, 0.863037, 0.5, 0.394531, 0.32156, 0.5, 0.420898, 0.764481, 0.5, 0.4375, 0.358795, 0.5, 0.460938, 0.66803, 0.5, 0.480469, 0.413654, 0.5, 0.489258, 0.575333, 0.5, 0.5, 0.488037, 0.5, -0.0492185, 0.971153, 0.5, -0.049999, 0.937988, 0.5, -0.0812503, 1.00793, 0.5, -0.0851567, 1.03776, 0.5, -0.114062, 0.915394, 0.5, -0.20625, 0.900055, 0.5, -0.289063, 0.884945, 0.5, -0.325001, 0.863037, 0.5, -0.345118, 0.881027, 0.5, -0.35, 0.863037, 0.5, -0.35078, 0.887024, 0.5, -0.362303, 0.881027, 0.5, -0.375, 0.300049, 0.5, -0.375, 0.863037, 0.5, -0.394531, 0.32156, 0.5, -0.420898, 0.764481, 0.5, -0.4375, 0.358795, 0.5, -0.460938, 0.66803, 0.5, -0.480469, 0.413654, 0.5, -0.489258, 0.575333, 0.5, -0.5, 0.488037, 0.519348, 0.0454139, 0.971153, 0.519348, -0.0454139, 0.971153, 0.519625, 0.0461242, 0.937988, 0.519625, -0.0461242, 0.937988, 0.53197, 0.0749796, 1.00793, 0.53197, -0.0749796, 1.00793, 0.533513, 0.0785866, 1.03776, 0.533513, -0.0785866, 1.03776, 0.534972, 0.0349715, 0.971153, 0.534972, -0.0349715, 0.971153, 0.5355, 0.0354995, 0.937988, 0.5355, -0.0354995, 0.937988, 0.54477, 0.105222, 0.915394, 0.54477, -0.105222, 0.915394, 0.545414, 0.0193479, 0.971153, 0.545414, -0.0193479, 0.971153, 0.546125, 0.0196248, 0.937988, 0.546125, -0.0196248, 0.937988, 0.549219, 0, 0.971153, 0.55, 0, 0.937988, 0.557758, 0.0577578, 1.00793, 0.557758, -0.0577578, 1.00793, 0.56054, 0.0605401, 1.03776, 0.56054, -0.0605401, 1.03776, 0.574979, 0.0319696, 1.00793, 0.574979, -0.0319696, 1.00793, 0.578586, 0.0335127, 1.03776, 0.578586, -0.0335127, 1.03776, 0.580953, 0.190266, 0.900055, 0.580953, -0.190266, 0.900055, 0.580984, 0.0809842, 0.915394, 0.580984, -0.0809842, 0.915394, 0.58125, 0, 1.00793, 0.585156, 0, 1.03776, 0.605223, 0.0447694, 0.915394, 0.605223, -0.0447694, 0.915394, 0.613457, 0.266661, 0.884945, 0.613457, -0.266661, 0.884945, 0.614062, 0, 0.915394, 0.627562, 0.299813, 0.863037, 0.627562, -0.299813, 0.863037, 0.635459, 0.318373, 0.881027, 0.635459, -0.318371, 0.881027, 0.637375, 0.322875, 0.863037, 0.637375, -0.322875, 0.863037, 0.637682, 0.323599, 0.887024, 0.637682, -0.323595, 0.887024, 0.642205, 0.334229, 0.881027, 0.642205, -0.334224, 0.881027, 0.646437, 0.146438, 0.900055, 0.646437, -0.146438, 0.900055, 0.647187, 0.345937, 0.300049, 0.647187, 0.345937, 0.863037, 0.647187, -0.345937, 0.300049, 0.647187, -0.345937, 0.863037, 0.654853, 0.363955, 0.32156, 0.654853, -0.363955, 0.32156, 0.665203, 0.388279, 0.764481, 0.665203, -0.388279, 0.764481, 0.671719, 0.403594, 0.358795, 0.671719, -0.403594, 0.358795, 0.680918, 0.425215, 0.66803, 0.680918, -0.425215, 0.66803, 0.688584, 0.443233, 0.413654, 0.688584, -0.443233, 0.413654, 0.690266, 0.080953, 0.900055, 0.690266, -0.080953, 0.900055, 0.692034, 0.45134, 0.575333, 0.692034, -0.45134, 0.575333, 0.69625, 0.46125, 0.488037, 0.69625, -0.46125, 0.488037, 0.705234, 0.205234, 0.884945, 0.705234, -0.205234, 0.884945, 0.70625, 0, 0.900055, 0.73075, 0.23075, 0.863037, 0.73075, -0.23075, 0.863037, 0.745033, 0.245034, 0.881027, 0.745033, -0.245033, 0.881027, 0.7485, 0.2485, 0.863037, 0.7485, -0.2485, 0.863037, 0.749055, 0.249056, 0.887024, 0.749055, -0.249054, 0.887024, 0.757236, 0.257238, 0.881027, 0.757236, -0.257235, 0.881027, 0.76625, 0.26625, 0.300049, 0.76625, 0.26625, 0.863037, 0.76625, -0.26625, 0.300049, 0.76625, -0.26625, 0.863037, 0.76666, 0.113457, 0.884945, 0.76666, -0.113457, 0.884945, 0.780117, 0.280117, 0.32156, 0.780117, -0.280117, 0.32156, 0.789062, 0, 0.884945, 0.798838, 0.298838, 0.764481, 0.798838, -0.298838, 0.764481, 0.799812, 0.127562, 0.863037, 0.799812, -0.127562, 0.863037, 0.810625, 0.310625, 0.358795, 0.810625, -0.310625, 0.358795, 0.818371, 0.135459, 0.881027, 0.818371, -0.135458, 0.881027, 0.822875, 0.137375, 0.863037, 0.822875, -0.137375, 0.863037, 0.823596, 0.137682, 0.887024, 0.823596, -0.137681, 0.887024, 0.825, 0, 0.863037, 0.827266, 0.327266, 0.66803, 0.827266, -0.327266, 0.66803, 0.834226, 0.142205, 0.881027, 0.834226, -0.142204, 0.881027, 0.841133, 0.341133, 0.413654, 0.841133, -0.341133, 0.413654, 0.845117, 0, 0.881027, 0.845937, 0.147187, 0.300049, 0.845937, 0.147187, 0.863037, 0.845937, -0.147187, 0.300049, 0.845937, -0.147187, 0.863037, 0.847373, 0.347373, 0.575333, 0.847373, -0.347373, 0.575333, 0.85, 0, 0.863037, 0.850781, 0, 0.887024, 0.855, 0.355, 0.488037, 0.855, -0.355, 0.488037, 0.862305, 0, 0.881027, 0.863955, 0.154853, 0.32156, 0.863955, -0.154853, 0.32156, 0.875, 0, 0.300049, 0.875, 0, 0.863037, 0.888279, 0.165203, 0.764481, 0.888279, -0.165203, 0.764481, 0.894531, 0, 0.32156, 0.903594, 0.171719, 0.358795, 0.903594, -0.171719, 0.358795, 0.920898, 0, 0.764481, 0.925, 0, 0.413086, 0.925, 0, 0.618896, 0.925, 0.0928131, 0.445244, 0.925, 0.0928131, 0.586738, 0.925, 0.123751, 0.515991, 0.925, -0.0928131, 0.445244, 0.925, -0.0928131, 0.586738, 0.925, -0.123751, 0.515991, 0.925215, 0.180918, 0.66803, 0.925215, -0.180918, 0.66803, 0.9375, 0, 0.358795, 0.943232, 0.188584, 0.413654, 0.943232, -0.188584, 0.413654, 0.95134, 0.192034, 0.575333, 0.95134, -0.192034, 0.575333, 0.960938, 0, 0.66803, 0.96125, 0.19625, 0.488037, 0.96125, -0.19625, 0.488037, 0.980469, 0, 0.413654, 0.989258, 0, 0.575333, 1, 0, 0.488037, 1.04492, 0, 0.646503, 1.05408, 0.0838042, 0.622637, 1.05408, -0.0838042, 0.622637, 1.07422, 0.111739, 0.570131, 1.07422, -0.111739, 0.570131, 1.09436, 0.0838042, 0.517625, 1.09436, -0.0838042, 0.517625, 1.09687, 0, 0.71286, 1.10352, 0, 0.493759, 1.10859, 0.0639847, 0.698941, 1.10859, -0.0639847, 0.698941, 1.12539, 0, 0.79327, 1.13437, 0.0853129, 0.66832, 1.13437, -0.0853129, 0.66832, 1.13967, 0.0441651, 0.788218, 1.13967, -0.0441651, 0.788218, 1.16016, 0.0639847, 0.637698, 1.16016, -0.0639847, 0.637698, 1.17109, 0.0588868, 0.777105, 1.17109, -0.0588868, 0.777105, 1.17188, 0, 0.623779, 1.175, 0, 0.863037, 1.19297, 0, 0.8732, 1.19844, 0.0351562, 0.863037, 1.19844, -0.0351562, 0.863037, 1.2, 0, 0.863037, 1.20251, 0.0441651, 0.765992, 1.20251, -0.0441651, 0.765992, 1.20625, 0, 0.876587, 1.21016, 0, 0.8732, 1.21563, 0.0210938, 0.863037, 1.21563, -0.0210938, 0.863037, 1.2168, 0, 0.76094, 1.21807, 0.032959, 0.873741, 1.21807, -0.032959, 0.873741, 1.22981, 0.028125, 0.87746, 1.22981, -0.028125, 0.87746, 1.23016, 0.023291, 0.873967, 1.23016, -0.023291, 0.873967, 1.25, 0.028125, 0.863037, 1.25, 0.046875, 0.863037, 1.25, -0.028125, 0.863037, 1.25, -0.046875, 0.863037, 1.27329, 0.0439453, 0.874933, 1.27329, -0.0439453, 0.874933, 1.27417, 0.0310547, 0.875654, 1.27417, -0.0310547, 0.875654, 1.28164, 0.0375, 0.879379, 1.28164, -0.0375, 0.879379, 1.28437, 0.0210938, 0.863037, 1.28437, -0.0210938, 0.863037, 1.3, 0, 0.863037, 1.30156, 0.0351562, 0.863037, 1.30156, -0.0351562, 0.863037, 1.31818, 0.023291, 0.877342, 1.31818, -0.023291, 0.877342, 1.325, 0, 0.863037, 1.32851, 0.032959, 0.876125, 1.32851, -0.032959, 0.876125, 1.33347, 0.028125, 0.881299, 1.33347, -0.028125, 0.881299, 1.33818, 0, 0.878109, 1.35361, 0, 0.876667, 1.35703, 0, 0.882171, -0.0167969, 0, 0.768166, -0.0202759, 0.0421881, 0.776765, -0.0202759, -0.0421881, 0.776765, -0.0279297, 0.0562508, 0.795683, -0.0279297, -0.0562508, 0.795683, -0.0355835, 0.0421881, 0.814601, -0.0355835, -0.0421881, 0.814601, -0.0390625, 0, 0.8232, -0.0800781, 0, 0.538757, -0.0827332, 0.0421881, 0.529498, -0.0827332, -0.0421881, 0.529498, -0.0885742, 0.0562508, 0.509128, -0.0885742, -0.0562508, 0.509128, -0.0944153, 0.0421881, 0.488759, -0.0944153, -0.0421881, 0.488759, -0.0970703, 0, 0.4795, -0.103125, 0, 0.762024, -0.111426, 0.0421881, 0.769668, -0.111426, -0.0421881, 0.769668, -0.129688, 0.0562508, 0.786484, -0.129688, -0.0562508, 0.786484, -0.134375, 0, 0.600098, -0.141943, 0.0421881, 0.592611, -0.141943, -0.0421881, 0.592611, -0.147949, 0.0421881, 0.8033, -0.147949, -0.0421881, 0.8033, -0.15625, 0, 0.810943, -0.156641, 0, 0.745354, -0.158594, 0.0562508, 0.576141, -0.158594, -0.0562508, 0.576141, -0.165234, 0, 0.661621, -0.167566, 0.0421881, 0.750404, -0.167566, -0.0421881, 0.750404, -0.175, 0, 0.712891, -0.175244, 0.0421881, 0.559671, -0.175244, -0.0421881, 0.559671, -0.175885, 0.0421881, 0.656723, -0.175885, -0.0421881, 0.656723, -0.182812, 0, 0.552185, -0.186719, 0.0421881, 0.712891, -0.186719, -0.0421881, 0.712891, -0.191602, 0.0562508, 0.761515, -0.191602, -0.0562508, 0.761515, -0.199316, 0.0562508, 0.645947, -0.199316, -0.0562508, 0.645947, -0.2125, 0.0562508, 0.712891, -0.2125, -0.0562508, 0.712891, -0.215637, 0.0421881, 0.772625, -0.215637, -0.0421881, 0.772625, -0.222748, 0.0421881, 0.63517, -0.222748, -0.0421881, 0.63517, -0.226562, 0, 0.777676, -0.233398, 0, 0.630272, -0.238281, 0.0421881, 0.712891, -0.238281, -0.0421881, 0.712891, -0.25, 0, 0.712891};
	unsigned int indices[] = {  // note that we start from 0!
	    0, 1, 3,   // first triangle
	    1, 2, 3    // second triangle
	};  

	unsigned int teapotIndices[] = {324, 306, 304, 304, 306, 317, 306, 283, 281, 281, 283, 304, 283, 248, 246, 246, 248, 281, 248, 172, 171, 171, 172, 246, 317, 304, 308, 308, 304, 325, 304, 281, 285, 285, 281, 308, 281, 246, 250, 250, 246, 285, 246, 171, 173, 173, 171, 250, 325, 308, 313, 313, 308, 328, 308, 285, 287, 287, 285, 313, 285, 250, 252, 252, 250, 287, 250, 173, 174, 174, 173, 252, 328, 313, 319, 319, 313, 332, 313, 287, 290, 290, 287, 319, 287, 252, 257, 257, 252, 290, 252, 174, 176, 176, 174, 257, 172, 117, 119, 119, 117, 171, 117, 82, 84, 84, 82, 119, 82, 59, 61, 61, 59, 84, 59, 42, 49, 49, 42, 61, 171, 119, 115, 115, 119, 173, 119, 84, 80, 80, 84, 115, 84, 61, 57, 57, 61, 80, 61, 49, 41, 41, 49, 57, 173, 115, 113, 113, 115, 174, 115, 80, 78, 78, 80, 113, 80, 57, 52, 52, 57, 78, 57, 41, 38, 38, 41, 52, 174, 113, 108, 108, 113, 176, 113, 78, 75, 75, 78, 108, 78, 52, 46, 46, 52, 75, 52, 38, 35, 35, 38, 46, 42, 60, 62, 62, 60, 49, 60, 83, 85, 85, 83, 62, 83, 118, 120, 120, 118, 85, 118, 193, 192, 192, 193, 120, 49, 62, 58, 58, 62, 41, 62, 85, 81, 81, 85, 58, 85, 120, 116, 116, 120, 81, 120, 192, 194, 194, 192, 116, 41, 58, 53, 53, 58, 38, 58, 81, 79, 79, 81, 53, 81, 116, 114, 114, 116, 79, 116, 194, 195, 195, 194, 114, 38, 53, 48, 48, 53, 35, 53, 79, 77, 77, 79, 48, 79, 114, 110, 110, 114, 77, 114, 195, 197, 197, 195, 110, 193, 249, 247, 247, 249, 192, 249, 284, 282, 282, 284, 247, 284, 307, 305, 305, 307, 282, 307, 324, 317, 317, 324, 305, 192, 247, 251, 251, 247, 194, 247, 282, 286, 286, 282, 251, 282, 305, 309, 309, 305, 286, 305, 317, 325, 325, 317, 309, 194, 251, 253, 253, 251, 195, 251, 286, 288, 288, 286, 253, 286, 309, 314, 314, 309, 288, 309, 325, 328, 328, 325, 314, 195, 253, 259, 259, 253, 197, 253, 288, 292, 292, 288, 259, 288, 314, 321, 321, 314, 292, 314, 328, 332, 332, 328, 321, 332, 319, 333, 333, 319, 338, 319, 290, 298, 298, 290, 333, 290, 257, 262, 262, 257, 298, 257, 176, 178, 178, 176, 262, 338, 333, 347, 347, 333, 354, 333, 298, 311, 311, 298, 347, 298, 262, 266, 266, 262, 311, 262, 178, 180, 180, 178, 266, 354, 347, 352, 352, 347, 358, 347, 311, 322, 322, 311, 352, 311, 266, 272, 272, 266, 322, 266, 180, 182, 182, 180, 272, 358, 352, 355, 355, 352, 359, 352, 322, 326, 326, 322, 355, 322, 272, 274, 274, 272, 326, 272, 182, 183, 183, 182, 274, 176, 108, 103, 103, 108, 178, 108, 75, 67, 67, 75, 103, 75, 46, 27, 27, 46, 67, 46, 35, 20, 20, 35, 27, 178, 103, 99, 99, 103, 180, 103, 67, 54, 54, 67, 99, 67, 27, 18, 18, 27, 54, 27, 20, 12, 12, 20, 18, 180, 99, 93, 93, 99, 182, 99, 54, 43, 43, 54, 93, 54, 18, 13, 13, 18, 43, 18, 12, 3, 3, 12, 13, 182, 93, 91, 91, 93, 183, 93, 43, 39, 39, 43, 91, 43, 13, 10, 10, 13, 39, 13, 3, 0, 0, 3, 10, 35, 48, 28, 28, 48, 20, 48, 77, 68, 68, 77, 28, 77, 110, 104, 104, 110, 68, 110, 197, 199, 199, 197, 104, 20, 28, 19, 19, 28, 12, 28, 68, 55, 55, 68, 19, 68, 104, 100, 100, 104, 55, 104, 199, 201, 201, 199, 100, 12, 19, 14, 14, 19, 3, 19, 55, 44, 44, 55, 14, 55, 100, 94, 94, 100, 44, 100, 201, 203, 203, 201, 94, 3, 14, 11, 11, 14, 0, 14, 44, 40, 40, 44, 11, 44, 94, 92, 92, 94, 40, 94, 203, 204, 204, 203, 92, 197, 259, 263, 263, 259, 199, 259, 292, 299, 299, 292, 263, 292, 321, 334, 334, 321, 299, 321, 332, 338, 338, 332, 334, 199, 263, 267, 267, 263, 201, 263, 299, 312, 312, 299, 267, 299, 334, 348, 348, 334, 312, 334, 338, 354, 354, 338, 348, 201, 267, 273, 273, 267, 203, 267, 312, 323, 323, 312, 273, 312, 348, 353, 353, 348, 323, 348, 354, 358, 358, 354, 353, 203, 273, 275, 275, 273, 204, 273, 323, 327, 327, 323, 275, 323, 353, 356, 356, 353, 327, 353, 358, 359, 359, 358, 356, 359, 355, 350, 350, 355, 357, 355, 326, 315, 315, 326, 350, 326, 274, 268, 268, 274, 315, 274, 183, 181, 181, 183, 268, 357, 350, 336, 336, 350, 349, 350, 315, 302, 302, 315, 336, 315, 268, 264, 264, 268, 302, 268, 181, 179, 179, 181, 264, 349, 336, 329, 329, 336, 335, 336, 302, 295, 295, 302, 329, 302, 264, 260, 260, 264, 295, 264, 179, 177, 177, 179, 260, 335, 329, 318, 318, 329, 331, 329, 295, 289, 289, 295, 318, 295, 260, 256, 256, 260, 289, 260, 177, 175, 175, 177, 256, 183, 91, 97, 97, 91, 181, 91, 39, 50, 50, 39, 97, 39, 10, 15, 15, 10, 50, 10, 0, 6, 6, 0, 15, 181, 97, 101, 101, 97, 179, 97, 50, 63, 63, 50, 101, 50, 15, 21, 21, 15, 63, 15, 6, 17, 17, 6, 21, 179, 101, 105, 105, 101, 177, 101, 63, 70, 70, 63, 105, 63, 21, 36, 36, 21, 70, 21, 17, 26, 26, 17, 36, 177, 105, 107, 107, 105, 175, 105, 70, 74, 74, 70, 107, 70, 36, 45, 45, 36, 74, 36, 26, 33, 33, 26, 45, 0, 11, 16, 16, 11, 6, 11, 40, 51, 51, 40, 16, 40, 92, 98, 98, 92, 51, 92, 204, 202, 202, 204, 98, 6, 16, 22, 22, 16, 17, 16, 51, 64, 64, 51, 22, 51, 98, 102, 102, 98, 64, 98, 202, 200, 200, 202, 102, 17, 22, 37, 37, 22, 26, 22, 64, 71, 71, 64, 37, 64, 102, 106, 106, 102, 71, 102, 200, 198, 198, 200, 106, 26, 37, 47, 47, 37, 33, 37, 71, 76, 76, 71, 47, 71, 106, 109, 109, 106, 76, 106, 198, 196, 196, 198, 109, 204, 275, 269, 269, 275, 202, 275, 327, 316, 316, 327, 269, 327, 356, 351, 351, 356, 316, 356, 359, 357, 357, 359, 351, 202, 269, 265, 265, 269, 200, 269, 316, 303, 303, 316, 265, 316, 351, 337, 337, 351, 303, 351, 357, 349, 349, 357, 337, 200, 265, 261, 261, 265, 198, 265, 303, 296, 296, 303, 261, 303, 337, 330, 330, 337, 296, 337, 349, 335, 335, 349, 330, 198, 261, 258, 258, 261, 196, 261, 296, 291, 291, 296, 258, 296, 330, 320, 320, 330, 291, 330, 335, 331, 331, 335, 320, 23, 24, 425, 425, 24, 424, 24, 29, 427, 427, 29, 425, 29, 31, 429, 429, 31, 427, 31, 34, 431, 431, 34, 429, 424, 425, 441, 441, 425, 440, 425, 427, 443, 443, 427, 441, 427, 429, 448, 448, 429, 443, 429, 431, 450, 450, 431, 448, 440, 441, 455, 455, 441, 451, 441, 443, 465, 465, 443, 455, 443, 448, 471, 471, 448, 465, 448, 450, 475, 475, 450, 471, 451, 455, 463, 463, 455, 457, 455, 465, 469, 469, 465, 463, 465, 471, 477, 477, 471, 469, 471, 475, 479, 479, 475, 477, 34, 32, 430, 430, 32, 431, 32, 30, 428, 428, 30, 430, 30, 25, 426, 426, 25, 428, 25, 23, 424, 424, 23, 426, 431, 430, 449, 449, 430, 450, 430, 428, 444, 444, 428, 449, 428, 426, 442, 442, 426, 444, 426, 424, 440, 440, 424, 442, 450, 449, 472, 472, 449, 475, 449, 444, 466, 466, 444, 472, 444, 442, 456, 456, 442, 466, 442, 440, 451, 451, 440, 456, 475, 472, 478, 478, 472, 479, 472, 466, 470, 470, 466, 478, 466, 456, 464, 464, 456, 470, 456, 451, 457, 457, 451, 464, 457, 463, 460, 460, 463, 454, 463, 469, 467, 467, 469, 460, 469, 477, 473, 473, 477, 467, 477, 479, 476, 476, 479, 473, 454, 460, 446, 446, 460, 445, 460, 467, 452, 452, 467, 446, 467, 473, 458, 458, 473, 452, 473, 476, 462, 462, 476, 458, 445, 446, 433, 433, 446, 432, 446, 452, 435, 435, 452, 433, 452, 458, 437, 437, 458, 435, 458, 462, 439, 439, 462, 437, 432, 433, 1, 1, 433, 0, 433, 435, 4, 4, 435, 1, 435, 437, 7, 7, 437, 4, 437, 439, 9, 9, 439, 7, 479, 478, 474, 474, 478, 476, 478, 470, 468, 468, 470, 474, 470, 464, 461, 461, 464, 468, 464, 457, 454, 454, 457, 461, 476, 474, 459, 459, 474, 462, 474, 468, 453, 453, 468, 459, 468, 461, 447, 447, 461, 453, 461, 454, 445, 445, 454, 447, 462, 459, 438, 438, 459, 439, 459, 453, 436, 436, 453, 438, 453, 447, 434, 434, 447, 436, 447, 445, 432, 432, 445, 434, 439, 438, 8, 8, 438, 9, 438, 436, 5, 5, 436, 8, 436, 434, 2, 2, 434, 5, 434, 432, 0, 0, 432, 2, 340, 342, 361, 361, 342, 360, 342, 343, 363, 363, 343, 361, 343, 341, 365, 365, 341, 363, 341, 339, 368, 368, 339, 365, 360, 361, 369, 369, 361, 367, 361, 363, 372, 372, 363, 369, 363, 365, 376, 376, 365, 372, 365, 368, 380, 380, 368, 376, 367, 369, 374, 374, 369, 371, 369, 372, 378, 378, 372, 374, 372, 376, 386, 386, 376, 378, 376, 380, 392, 392, 380, 386, 371, 374, 383, 383, 374, 381, 374, 378, 400, 400, 378, 383, 378, 386, 412, 412, 386, 400, 386, 392, 416, 416, 392, 412, 339, 344, 366, 366, 344, 368, 344, 346, 364, 364, 346, 366, 346, 345, 362, 362, 345, 364, 345, 340, 360, 360, 340, 362, 368, 366, 377, 377, 366, 380, 366, 364, 373, 373, 364, 377, 364, 362, 370, 370, 362, 373, 362, 360, 367, 367, 360, 370, 380, 377, 387, 387, 377, 392, 377, 373, 379, 379, 373, 387, 373, 370, 375, 375, 370, 379, 370, 367, 371, 371, 367, 375, 392, 387, 413, 413, 387, 416, 387, 379, 402, 402, 379, 413, 379, 375, 384, 384, 375, 402, 375, 371, 381, 381, 371, 384, 381, 383, 393, 393, 383, 382, 383, 400, 403, 403, 400, 393, 400, 412, 417, 417, 412, 403, 412, 416, 422, 422, 416, 417, 382, 393, 395, 395, 393, 388, 393, 403, 407, 407, 403, 395, 403, 417, 419, 419, 417, 407, 417, 422, 423, 423, 422, 419, 388, 395, 397, 397, 395, 389, 395, 407, 405, 405, 407, 397, 407, 419, 414, 414, 419, 405, 419, 423, 421, 421, 423, 414, 389, 397, 390, 390, 397, 385, 397, 405, 399, 399, 405, 390, 405, 414, 409, 409, 414, 399, 414, 421, 411, 411, 421, 409, 416, 413, 418, 418, 413, 422, 413, 402, 404, 404, 402, 418, 402, 384, 394, 394, 384, 404, 384, 381, 382, 382, 381, 394, 422, 418, 420, 420, 418, 423, 418, 404, 408, 408, 404, 420, 404, 394, 396, 396, 394, 408, 394, 382, 388, 388, 382, 396, 423, 420, 415, 415, 420, 421, 420, 408, 406, 406, 408, 415, 408, 396, 398, 398, 396, 406, 396, 388, 389, 389, 388, 398, 421, 415, 410, 410, 415, 411, 415, 406, 401, 401, 406, 410, 406, 398, 391, 391, 398, 401, 398, 389, 385, 385, 389, 391, 162, 231, 238, 162, 227, 231, 162, 211, 227, 162, 166, 211, 238, 231, 229, 229, 231, 237, 231, 227, 225, 225, 227, 229, 227, 211, 209, 209, 211, 225, 211, 166, 165, 165, 166, 209, 237, 229, 219, 219, 229, 223, 229, 225, 213, 213, 225, 219, 225, 209, 205, 205, 209, 213, 209, 165, 163, 163, 165, 205, 223, 219, 221, 221, 219, 224, 219, 213, 215, 215, 213, 221, 213, 205, 207, 207, 205, 215, 205, 163, 164, 164, 163, 207, 162, 154, 166, 162, 138, 154, 162, 134, 138, 162, 128, 134, 166, 154, 156, 156, 154, 165, 154, 138, 140, 140, 138, 156, 138, 134, 136, 136, 134, 140, 134, 128, 129, 129, 128, 136, 165, 156, 160, 160, 156, 163, 156, 140, 152, 152, 140, 160, 140, 136, 146, 146, 136, 152, 136, 129, 143, 143, 129, 146, 163, 160, 158, 158, 160, 164, 160, 152, 150, 150, 152, 158, 152, 146, 144, 144, 146, 150, 146, 143, 142, 142, 143, 144, 162, 135, 128, 162, 139, 135, 162, 155, 139, 162, 187, 155, 128, 135, 137, 137, 135, 129, 135, 139, 141, 141, 139, 137, 139, 155, 157, 157, 155, 141, 155, 187, 186, 186, 187, 157, 129, 137, 147, 147, 137, 143, 137, 141, 153, 153, 141, 147, 141, 157, 161, 161, 157, 153, 157, 186, 184, 184, 186, 161, 143, 147, 145, 145, 147, 142, 147, 153, 151, 151, 153, 145, 153, 161, 159, 159, 161, 151, 161, 184, 185, 185, 184, 159, 162, 212, 187, 162, 228, 212, 162, 232, 228, 162, 238, 232, 187, 212, 210, 210, 212, 186, 212, 228, 226, 226, 228, 210, 228, 232, 230, 230, 232, 226, 232, 238, 237, 237, 238, 230, 186, 210, 206, 206, 210, 184, 210, 226, 214, 214, 226, 206, 226, 230, 220, 220, 230, 214, 230, 237, 223, 223, 237, 220, 184, 206, 208, 208, 206, 185, 206, 214, 216, 216, 214, 208, 214, 220, 222, 222, 220, 216, 220, 223, 224, 224, 223, 222, 224, 221, 239, 239, 221, 243, 221, 215, 235, 235, 215, 239, 215, 207, 217, 217, 207, 235, 207, 164, 167, 167, 164, 217, 243, 239, 270, 270, 239, 278, 239, 235, 254, 254, 235, 270, 235, 217, 233, 233, 217, 254, 217, 167, 168, 168, 167, 233, 278, 270, 293, 293, 270, 297, 270, 254, 276, 276, 254, 293, 254, 233, 241, 241, 233, 276, 233, 168, 169, 169, 168, 241, 297, 293, 300, 300, 293, 310, 293, 276, 279, 279, 276, 300, 276, 241, 244, 244, 241, 279, 241, 169, 170, 170, 169, 244, 164, 158, 148, 148, 158, 167, 158, 150, 130, 130, 150, 148, 150, 144, 126, 126, 144, 130, 144, 142, 123, 123, 142, 126, 167, 148, 132, 132, 148, 168, 148, 130, 111, 111, 130, 132, 130, 126, 95, 95, 126, 111, 126, 123, 88, 88, 123, 95, 168, 132, 124, 124, 132, 169, 132, 111, 89, 89, 111, 124, 111, 95, 72, 72, 95, 89, 95, 88, 69, 69, 88, 72, 169, 124, 121, 121, 124, 170, 124, 89, 86, 86, 89, 121, 89, 72, 65, 65, 72, 86, 72, 69, 56, 56, 69, 65, 142, 145, 127, 127, 145, 123, 145, 151, 131, 131, 151, 127, 151, 159, 149, 149, 159, 131, 159, 185, 188, 188, 185, 149, 123, 127, 96, 96, 127, 88, 127, 131, 112, 112, 131, 96, 131, 149, 133, 133, 149, 112, 149, 188, 189, 189, 188, 133, 88, 96, 73, 73, 96, 69, 96, 112, 90, 90, 112, 73, 112, 133, 125, 125, 133, 90, 133, 189, 190, 190, 189, 125, 69, 73, 66, 66, 73, 56, 73, 90, 87, 87, 90, 66, 90, 125, 122, 122, 125, 87, 125, 190, 191, 191, 190, 122, 185, 208, 218, 218, 208, 188, 208, 216, 236, 236, 216, 218, 216, 222, 240, 240, 222, 236, 222, 224, 243, 243, 224, 240, 188, 218, 234, 234, 218, 189, 218, 236, 255, 255, 236, 234, 236, 240, 271, 271, 240, 255, 240, 243, 278, 278, 243, 271, 189, 234, 242, 242, 234, 190, 234, 255, 277, 277, 255, 242, 255, 271, 294, 294, 271, 277, 271, 278, 297, 297, 278, 294, 190, 242, 245, 245, 242, 191, 242, 277, 280, 280, 277, 245, 277, 294, 301, 301, 294, 280, 294, 297, 310, 310, 297, 301};

	unsigned int VAO, teapotVao;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &teapotVao);  
	glBindVertexArray(VAO);
	unsigned int VBO, teapotVbo;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &teapotVbo);
	unsigned int EBO, teapotEbo;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &teapotEbo);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgram);

	glBindVertexArray(teapotVao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(teapotVertices), teapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgram);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// // glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, sizeof(teapotIndices), GL_UNSIGNED_INT, 0);

	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}

	glfwTerminate();

	return 0;
}