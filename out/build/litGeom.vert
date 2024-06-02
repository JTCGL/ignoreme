#version 300 es
uniform mat4 MVP;
uniform mat4 MIT;
uniform vec4 colorin;
uniform vec3 lightDir;
in vec3 vPos;
in vec3 vNorm;
out vec4 color;
out vec3 norm;

void main() {
	vec4 normWorld = vec4(vNorm, 0.0);
	normWorld = MIT * normWorld;
	norm = normalize(normWorld.xyz);
	gl_Position = MVP * vec4(vPos, 1.0);
	color.rgb = colorin.rgb * max(0.0, dot(normWorld.xyz,lightDir) );
	color.a=1.0;
}
