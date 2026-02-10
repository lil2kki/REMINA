#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_time;

float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}

float digitalNoise(vec2 uv) {
    float n = noise(floor(uv * 100.0) + vec2(u_time * 5.0, 0.0));
    return step(0.9, n);
}

void main() {
    vec2 uv = v_texCoord;
    vec4 color = texture2D(u_texture, uv);
    
    // Резкий глитч-сдвиг
    if (mod(u_time * 1.0, 3.0) < 0.1) {
        float offset = noise(vec2(u_time, uv.y)) * 0.15;
        color = texture2D(u_texture, vec2(uv.x + offset, uv.y));
    }
    
    // Бинарный распад (только черный/белый)
    if (mod(u_time * 1.0, 1.0) > 0.5) {
        float bw = (color.r + color.g + color.b) / 3.0;
        bw = step(0.5, bw);
        color.rgb = vec3(bw);
    }
    
    // Добавление цифрового шума
    if (noise(uv + vec2(u_time, 0.0)) > 0.7) {
        color.rgb += digitalNoise(uv) * 0.8;
        color.rgb = clamp(color.rgb, 0.0, 1.0);
    }
    
    // Мерцание
    //float flicker = sin(u_time * 30.0) * 0.9 + 0.9;
    //color.rgb *= flicker;
    
    // Резкие черные полосы
    float scanline = sin(uv.y * 300.0 + u_time * 10.0);
    if (abs(scanline) > 0.99) {
        color.rgb = vec3(0.0);
    }
    
    gl_FragColor = color;
}