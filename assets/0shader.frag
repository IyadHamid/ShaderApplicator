uniform sampler2D texture;
uniform vec2 resolution;

void main() {
    vec2 coords = gl_FragCoord.st/resolution;
    vec4 pixel = texture2D(texture, coords);

    gl_FragColor = pixel;
}