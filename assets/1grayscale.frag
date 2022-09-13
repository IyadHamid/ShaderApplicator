uniform sampler2D texture;
uniform vec2 resolution;

void main() {
    vec2 coords = gl_FragCoord.st/resolution;
    vec4 pixel = texture2D(texture, coords);
    vec3 gray = vec3((pixel.r + pixel.g + pixel.b) / 3.0);

    gl_FragColor = vec4(gray, 1.0);
}