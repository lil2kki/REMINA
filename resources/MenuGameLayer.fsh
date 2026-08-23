#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_texture;
uniform float u_time;
uniform float u_mouseX;
uniform float u_mouseY;
uniform vec2 u_mouse;
uniform float u_pulse1;
uniform float u_pulse2;
uniform float u_pulse3;
uniform vec3 u_pulse;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main() {
}