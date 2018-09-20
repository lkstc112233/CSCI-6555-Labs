#include "Object.h"

#include <glm/glm.hpp>

Object3D::Object3D(const Model &modeli)
    : model(modeli), transform(1.0f)
{
	if (modeli.getDimensions() != 3) {
		throw "INCAPABLE_DIMENSION";
	}
}

void Object3D::setTransformMatrix(const glm::mat4& transformi) {
	transform = transformi;
}

void Object3D::draw(ShaderProgram& shader) {
	shader.setMatrix("transform", transform);
	model.draw(shader);
}

Object2D::Object2D(const Model &modeli)
    : model(modeli), transform(1.0f), z(0)
{
	if (modeli.getDimensions() != 2) {
		throw "INCAPABLE_DIMENSION";
	}
}

void Object2D::setTransformMatrix(const glm::mat3& transformi) {
	transform = transformi;
}

void Object2D::setZ(float zi) {
	z = zi;
}

void Object2D::draw(ShaderProgram& shader) {
	shader.setMatrix("transform", transform);
	shader.setValue("z", z);
	model.draw(shader);
}
