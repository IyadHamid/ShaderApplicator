uniform sampler2D texture;
uniform vec2 resolution;

void main() {
    vec2 coords = gl_FragCoord.st/resolution;

    gl_FragColor = vec4(coords, 0.0, 1.0);
}