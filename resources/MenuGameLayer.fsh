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
    vec2 uv = vec2(v_texCoord.x, 1.0 - v_texCoord.y);
    
    float pulse = u_pulse1 * 0.5 + u_pulse2 * 0.3 + u_pulse3 * 0.2;
    float glitchIntensity = clamp(pulse * 2.0, 0.2, 1.0);
    
    float strip = floor(uv.y / 0.01);
    float stripShift = random(vec2(strip, u_time * 0.1)) * 0.005;
    float enableShift = step(0.7, random(vec2(strip, floor(u_time * 5.0))));
    uv.x += stripShift * enableShift * glitchIntensity;
    
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    vec4 color = texture2D(u_texture, uv) * v_color;
    
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    float desatFactor = 1.0 - pulse * 0.1;
    color.rgb = mix(color.rgb, vec3(gray), desatFactor);
    
    float darken = 0.3 - pulse * 0.2;
    color.rgb *= darken;
    
    float edgeDist = min(min(uv.x + 0.12, 1.12 - uv.x), min(1.0, 1.0));
    float edgeDark = 1.0 - smoothstep(0.0, 0.45, edgeDist);
    
    float blockX = floor(uv.x * 20.0 + u_time * 4.0);
    float blockY = floor(uv.y * 10.0);
    float noiseEdge = random(vec2(blockX, blockY)) * pulse;
    
    edgeDark += noiseEdge * edgeDark * 0.5;
    edgeDark = clamp(edgeDark, 0.0, 1.0);
    
    color.rgb = mix(color.rgb, vec3(0.0), edgeDark * 1.0);
    
    color.rgb = (color.rgb - 0.5) * 1.2 + 0.5;
    
    float noise = random(uv + u_time) * 0.1;
    color.rgb += noise * pulse;
    
    gl_FragColor = color;
}