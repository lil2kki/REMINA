#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform float u_time;
uniform float u_darkness; // Контроль темноты (0.0 - 1.0)
uniform float u_desaturation = 0.95; // Контроль насыщенности (0.0 - 1.0)

// Улучшенная функция случайности
float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 78.233))) * 43758.5453);
}

// Псевдо-шум Перлина для более органичных эффектов
float noise(vec2 p) {
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u * u * (3.0 - 2.0 * u);
    
    float res = mix(
        mix(rand(ip), rand(ip + vec2(1.0, 0.0)), u.x),
        mix(rand(ip + vec2(0.0, 1.0)), rand(ip + vec2(1.0, 1.0)), u.x),
        u.y
    );
    return res;
}

// Функция снижения насыщенности
vec3 desaturate(vec3 color, float amount) {
    float luma = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(color, vec3(luma), amount);
}

// Функция затемнения с сохранением деталей в тенях
vec3 applyDarkness(vec3 color, float darkness) {
    // Нелинейное затемнение для сохранения деталей
    float adjustedDarkness = darkness * 0.7;
    color *= (1.0 - adjustedDarkness);
    
    // Добавляем смещение в тёмные тона
    color = mix(color, color * 0.3, darkness * 0.5);
    
    return color;
}

// Эффект статического шума для фона
vec3 addStaticNoise(vec2 uv, float time, float intensity) {
    float staticNoise = rand(uv * 1000.0 + time);
    staticNoise = (staticNoise - 0.5) * 2.0 * intensity;
    return vec3(staticNoise);
}

// Эффект "кровавых" разводов
vec3 addBloodStains(vec2 uv, float time) {
    float bloodPattern = 0.0;
    
    // Создаём несколько "разливов"
    for(int i = 0; i < 3; i++) {
        vec2 center = vec2(0.3 + float(i) * 0.2, 0.5);
        float dist = distance(uv, center);
        float blood = smoothstep(0.3, 0.0, dist);
        blood *= sin(time * 0.5 + float(i)) * 0.5 + 0.5;
        bloodPattern += blood * 0.3;
    }
    
    // Красный оттенок с варьирующейся интенсивностью
    float pulse = sin(time * 0.7) * 0.3 + 0.7;
    return vec3(bloodPattern * 0.8, 0.0, 0.0) * pulse;
}

void main() {
    vec2 uv = v_texCoord;
    
    // Параметры по умолчанию, если uniform не установлены
    float darkness = u_darkness > 0.0 ? u_darkness : 0.5;
    float desaturation = u_desaturation > 0.0 ? u_desaturation : 0.6;
    
    // Глитч-эффекты с мрачной модификацией
    float blockSize = 30.0 + sin(u_time * 0.3) * 10.0;
    float block = floor(uv.y * blockSize);
    
    // Дополнительный мрачный фактор для эффектов
    float horrorFactor = 0.5 + darkness * 0.5;
    
    // Глитч спадение (более резкое и редкое)
    float timeQuantized = floor(u_time * 2.0) * 0.5;
    float blockRand = rand(vec2(block, timeQuantized));
    
    // Сильный глитч происходит реже для большего драматизма
    float strongGlitch = step(0.93, blockRand);
    float mediumGlitch = step(0.88, blockRand) * (1.0 - strongGlitch);
    
    // Сдвиг блоков с разной интенсивностью
    float shiftAmount = 0.0;
    shiftAmount += strongGlitch * 0.15 * rand(vec2(u_time, 1.0));
    shiftAmount += mediumGlitch * 0.06 * rand(vec2(u_time, 2.0));
    uv.x += shiftAmount;
    
    // Вертикальные разрывы с хаотичностью
    if (strongGlitch > 0.5 || (mediumGlitch > 0.5 && rand(vec2(uv.x, u_time)) > 0.7)) {
        float breakIntensity = (sin(u_time * 8.0 + block * 0.5) * 0.5 + 0.5) * horrorFactor;
        uv.y += (rand(vec2(block, u_time)) - 0.5) * 0.08 * breakIntensity;
    }
    
    // Получаем исходный цвет
    vec4 color = texture2D(CC_Texture0, uv);
    
    // Резкое мерцание с мрачным оттенком
    float timeFast = u_time * 15.0;
    float flickerRand = rand(vec2(timeFast, uv.y * 10.0));
    float flicker = mix(0.7, 1.2, step(0.05, flickerRand));
    color.rgb *= flicker;
    
    // Инвертирование в сильных глитчах (менее частое)
    if (strongGlitch > 0.5 && rand(vec2(u_time, uv.x)) > 0.5) {
        color.rgb = 1.0 - color.rgb;
        // Добавляем красный оттенок к инвертированным областям
        color.r += 0.3 * horrorFactor;
    }
    
    // Черные полосы с градиентными краями
    if (blockRand > 0.85 && blockRand < 0.89) {
        float stripeWidth = 0.1;
        float stripePos = fract(uv.y * 5.0 + u_time * 0.1);
        float stripe = smoothstep(0.0, stripeWidth, stripePos) * 
                      smoothstep(1.0, 1.0 - stripeWidth, stripePos);
        color.rgb *= mix(0.1, 1.0, stripe);
    }
    
    // VHS артефакты с зелёным оттенком для атмосферы наблюдения
    float scanlineFreq = 700.0 + sin(u_time * 0.5) * 100.0;
    float scanline = sin(uv.y * scanlineFreq + u_time * 8.0) * 0.03;
    // Зелёный оттенок сканирующих линий
    color.rgb += vec3(scanline * 0.3, scanline * 0.8, scanline * 0.2);
    
    // Дополнительные мрачные эффекты
    // 1. Статический шум в тенях
    vec3 staticEffect = addStaticNoise(uv, u_time, 0.03 * horrorFactor);
    color.rgb += staticEffect * darkness;
    
    // 2. Кровавые разводы (появляются периодически)
    float bloodChance = sin(u_time * 0.3) * 0.5 + 0.5;
    if (bloodChance > 0.8) {
        vec3 bloodEffect = addBloodStains(uv, u_time);
        color.rgb = mix(color.rgb, color.rgb + bloodEffect, 0.3 * horrorFactor);
    }
    
    // 3. Виньетирование для фокуса и атмосферы
    float vignette = distance(uv, vec2(0.5, 0.5));
    vignette = smoothstep(0.3, 1.0, vignette);
    color.rgb *= 1.0 - vignette * 0.7 * darkness;
    
    // 4. Цветовая коррекция для мрачности
    // Снижение насыщенности
    color.rgb = desaturate(color.rgb, desaturation);
    
    // Затемнение
    color.rgb = applyDarkness(color.rgb, darkness);
    
    // Сдвиг в холодные/синие тона для хоррор-атмосферы
    color.b += 0.1 * darkness;
    color.g *= 0.9;
    
    // Добавляем общее затемнение (луминесценция)
    float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    float darkenByLuminance = smoothstep(0.3, 0.1, luminance);
    color.rgb *= mix(1.0, 0.8, darkenByLuminance * darkness);
    
    // Финальная регулировка контраста для "выжимания" деталей
    float contrast = 1.2;
    color.rgb = (color.rgb - 0.5) * contrast + 0.5;
    
    // Гарантируем, что цвета не выйдут за пределы
    color.rgb = clamp(color.rgb, 0.0, 1.0);
    
    gl_FragColor = color * v_fragmentColor;
}