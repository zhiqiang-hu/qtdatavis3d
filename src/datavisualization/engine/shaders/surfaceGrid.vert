attribute highp vec3 vertexPosition_mdl;
uniform highp mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition_mdl, 1.0);
}
