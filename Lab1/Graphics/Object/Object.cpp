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
